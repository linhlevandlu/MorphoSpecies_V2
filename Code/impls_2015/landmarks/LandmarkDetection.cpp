/*
 * LandmarkDetection.cpp
 *
 *  Created on: Aug 21, 2015
 *  Image processing for morphometrics (IPM) Version 2
 *	Copyright (C) 2015 LE Van Linh (linhlevandlu@gmail.com)
 *
 *	This program is free software: you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License as published by
 * 	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include "LandmarkDetection.h"

namespace impls_2015 {

/**
 * Constructor the landmarks detection
 */
LandmarkDetection::LandmarkDetection() {
}

LandmarkDetection::~LandmarkDetection() {
	// TODO Auto-generated destructor stub
}
/**
 * Set the noise when find the landmarks
 * @parameter: noise - the noise
 */
void LandmarkDetection::setNoise(double noise) {
	this->noise = noise;
}

Mat LandmarkDetection::createTemplate(Mat matImage, Point landmark, int tsize,
		Point &location, Point &distance) {
	int lx = (landmark.x - tsize / 2) < 0 ? 0 : (landmark.x - tsize / 2);
	int ly = (landmark.y - tsize / 2) < 0 ? 0 : (landmark.y - tsize / 2);
	location.x = lx;
	location.y = ly;
	distance.x = landmark.x - lx;
	distance.y = landmark.y - ly;
	int lx2 =
			(landmark.x + tsize / 2) > matImage.cols ?
					matImage.cols : (landmark.x + tsize / 2);
	int ly2 =
			(landmark.y + tsize / 2) > matImage.rows ?
					matImage.rows : (landmark.y + tsize / 2);

	Mat refTemplate(matImage, Rect(lx, ly, lx2 - lx, ly2 - ly));
	return refTemplate;
}

vector<Point> LandmarkDetection::crossCorrelation(Image refImage,
		Image sceneImage, QString lmPath, int templSize) {
	vector<Point> refLandmarks = refImage.readLandmarksFile(
			lmPath.toStdString().c_str());
	vector<Point> sceneLandmarks;
	Point tLocation, tDistance, iLocation, iDistance;
	for (size_t i = 0; i < refLandmarks.size(); i++) {
		Point temp = refLandmarks.at(i);
		Point lm(temp.x, refImage.getMatrixImage().rows - temp.y);

		Mat templ = createTemplate(refImage.getMatrixImage(), lm, templSize,
				tLocation, tDistance);
		Mat img = sceneImage.getMatrixImage();
		Point maxLoc = crossCorrelation(templ, img);
		sceneLandmarks.push_back(iLocation + maxLoc + tDistance);
	}
	return sceneLandmarks;
}
Point LandmarkDetection::crossCorrelation(Mat templ, Mat scene) {
	Mat result(scene.cols - templ.cols + 1, scene.rows - templ.rows + 1,
			CV_32FC1);
	cv::matchTemplate(scene, templ, result, CV_TM_CCOEFF_NORMED);
	double maxValue, minValue;
	Point maxLoc, minLoc;
	cv::minMaxLoc(result, &minValue, &maxValue, &minLoc, &maxLoc, Mat());
	return maxLoc;
}

void LandmarkDetection::landmarksByDirectory(Image refImage, QString path,
		QString savepath, QString lmPath) {

	QFileInfoList files = Image::readFolder(path);

	QString meanDiffPath = savepath + "/meanDiff.TPS";
	ofstream meanDiff(meanDiffPath.toStdString().c_str());

	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name);

		cv::Mat result(sceneImage.getMatrixImage().clone());
		vector<Point> landmarks = crossCorrelation(refImage, sceneImage, lmPath,
				500);

		int index2 = sceneImage.getFileName().lastIndexOf("/");
		QString scenename = sceneImage.getFileName().mid(index2 + 1,
				sceneImage.getFileName().length() - index2 - 5);
		qDebug() << scenename;
		QString spath = "/home/linh/Desktop/landmarks/" + scenename + ".TPS";
		vector<Point> sceneLandmarks = sceneImage.readLandmarksFile(
				spath.toStdString().c_str());
		QString newspath = savepath + "/" + scenename + ".TPS";

		ofstream lmFile(newspath.toStdString().c_str());

		double totalDiff = 0;
		for (size_t i = 0; i < landmarks.size(); i++) {
			Point lm = landmarks.at(i);
			stringstream ss;
			ss << i;

			circle(result, Point(lm.x, lm.y), 5, Scalar(0, 0, 255), 2, 4);
			cv::putText(result, ss.str(), Point(lm.x, lm.y),
					FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255), 1, 8);

			Point orglm = sceneLandmarks.at(i);
			circle(result,
					Point(orglm.x, sceneImage.getMatrixImage().rows - orglm.y),
					5, Scalar(0, 255, 0), 2, 4);
			cv::putText(result, ss.str(),
					Point(orglm.x, sceneImage.getMatrixImage().rows - orglm.y),
					FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0), 1, 8);
			Line diff(orglm,
					Point(lm.x, sceneImage.getMatrixImage().rows - lm.y));

			lmFile << "(" << orglm.x << "," << orglm.y << ")" << "\t" << "("
					<< lm.x << "," << sceneImage.getMatrixImage().rows - lm.y
					<< ")" << "\t" << diff.length() << "\n";
			totalDiff += diff.length();
		}
		lmFile.close();
		meanDiff << scenename.toStdString().c_str() << ".JPG " << "\t"
				<< (totalDiff / 18) << "\n";

		QString saveImg = savepath + "/"
				+ _name.mid(index2 + 1, _name.length() - index2 - 5).replace(
						" ", "") + ".JPG";
		imwrite(saveImg.toStdString().c_str(), result);
	}
	meanDiff.close();
}
double LandmarkDetection::centroid(Image refImage, Image sceneImage,
		QString lmPath, QString slmPath, Point &center) {

	vector<Point> sceneLandmarks = crossCorrelation(refImage, sceneImage,
			lmPath, 500);
	vector<Point> refLandmarks = refImage.readLandmarksFile(
			slmPath.toStdString().c_str());
	int totalX = 0, totalxE = 0;
	int totalY = 0, totalyE = 0;
	for (size_t i = 0; i < refLandmarks.size(); i++) {
		Point lm = refLandmarks.at(i);
		totalX += lm.x;
		totalY += lm.y;

		Point elm = sceneLandmarks.at(i);
		totalxE += elm.x;
		totalyE += (sceneImage.getMatrixImage().rows - elm.y);
	}

	center.x = totalX / 18;
	center.y = totalY / 18;

	Point baryE;
	baryE.x = totalxE / 18;
	baryE.y = totalyE / 18;

	double totalM = 0;
	double totalS = 0;
	for (size_t i = 0; i < refLandmarks.size(); i++) {
		Point lm = refLandmarks.at(i);
		Line line(lm, center);
		totalM += (line.length() * line.length());

		Line sline(baryE,
				Point(sceneLandmarks.at(i).x,
						sceneImage.getMatrixImage().rows
								- sceneLandmarks.at(i).y));
		totalS += (sline.length() * sline.length());
	}
	double resultM = sqrt(totalM);
	double resultE = sqrt(totalS);

	qDebug() << "(" << center.x << "," << center.y << ")" << "(" << baryE.x
			<< "," << baryE.y << ")" << "\t" << resultM << "\t" << resultE
			<< "\t" << abs(resultM - resultE);

	return resultM;
}
void LandmarkDetection::centroidFolder(Image refImage, QString lmPath,
		QString folderImages, QString folderlandmarks) {

	QFileInfoList files = Image::readFolder(folderImages);
	ofstream of("/home/linh/Desktop/cross/centroid.PHG");
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name);
		int index2 = sceneImage.getFileName().lastIndexOf("/");
		QString scenename = sceneImage.getFileName().mid(index2 + 1,
				sceneImage.getFileName().length() - index2 - 5);
		QString spath = folderlandmarks + "/" + scenename + ".TPS";
		vector<Point> mLandmarks = sceneImage.readLandmarksFile(
				spath.toStdString().c_str());
		vector<Point> eLandmarks = crossCorrelation(refImage, sceneImage,
				lmPath, 500);
		of << scenename.toStdString().c_str() << ".JPG" << "\t";
		qDebug() << scenename;
		int totalX = 0, totalxE = 0;
		int totalY = 0, totalyE = 0;
		for (size_t j = 0; j < mLandmarks.size(); j++) {
			Point lm = mLandmarks.at(j);
			totalX += lm.x;
			totalY += lm.y;

			Point elm = eLandmarks.at(j);
			totalxE += elm.x;
			totalyE += (sceneImage.getMatrixImage().rows - elm.y);
		}
		Point center;
		center.x = totalX / 18;
		center.y = totalY / 18;

		Point baryE;
		baryE.x = totalxE / 18;
		baryE.y = totalyE / 18;

		of << "(" << center.x << "," << center.y << ")" << "\t" << "("
				<< baryE.x << "," << baryE.y << ")" << "\t";

		double totalM = 0;
		double totalS = 0;
		for (size_t k = 0; k < mLandmarks.size(); k++) {
			Point lm = mLandmarks.at(k);
			Line line(lm, center);
			totalM += (line.length() * line.length());

			Line sline(baryE,
					Point(eLandmarks.at(k).x,
							sceneImage.getMatrixImage().rows
									- eLandmarks.at(k).y));
			totalS += (sline.length() * sline.length());
		}
		double resultM = sqrt(totalM);
		double resultE = sqrt(totalS);
		double subtraction = abs(resultM - resultE);
		of << resultM << "\t" << resultE << "\t" << subtraction << "\n";
	}
	of.close();
}

Mat LandmarkDetection::rotateImage(Mat source, double angle) {
	Mat dest = Mat::zeros(source.size(), source.type());
	Point2f pt(dest.cols / 2, dest.rows / 2);
	Mat rotate = getRotationMatrix2D(pt, angle, 1);
	warpAffine(source, dest, rotate, source.size());
	return dest;
}
Mat LandmarkDetection::matchingTemplate(Image refImage, Image sceneImage,
		QString lmPath, int templSize, int sceneSize, double angleDiff) {
	vector<Point> refLandmarks = refImage.readLandmarksFile(
			lmPath.toStdString());
	Mat sMatrix = sceneImage.getMatrixImage();
	Mat mMatrix = refImage.getMatrixImage();
	Mat sRotateImg = rotateImage(sMatrix, angleDiff);
	PHoughTransform pht;
	vector<Point> esLandmarks = pht.estimateLandmarks(refImage, sceneImage,
			lmPath.toStdString(), angleDiff);
	vector<Point> lmResult;
	int index2 = sceneImage.getFileName().lastIndexOf("/");
	QString scenename = sceneImage.getFileName().mid(index2 + 1,
			sceneImage.getFileName().length() - index2 - 5);
	qDebug() << scenename;
	QString spath = "/home/linh/Desktop/landmarks/" + scenename + ".TPS";
	vector<Point> sceneLandmarks = sceneImage.readLandmarksFile(
			spath.toStdString().c_str());
	for (size_t i = 0; i < refLandmarks.size(); i++) {
		Point lm(refLandmarks.at(i).x, mMatrix.rows - refLandmarks.at(i).y);
		Point tLocation, tDistance, iLocation, iDistance;
		Mat templ = createTemplate(mMatrix, lm, templSize, tLocation,
				tDistance);
		Mat sceneM = createTemplate(sRotateImg, esLandmarks.at(i), sceneSize,
				iLocation, iDistance);
		Point maxLoc = crossCorrelation(templ, sceneM);
		lmResult.push_back(iLocation + maxLoc + tDistance);
	}
	Mat result(sRotateImg.clone());
	for (size_t j = 0; j < lmResult.size(); j++) {
		circle(result, lmResult.at(j), 5, Scalar(0, 255, 255), 2, 8);
		circle(result,
				Point(sceneLandmarks.at(j).x,
						sMatrix.rows - sceneLandmarks.at(j).y), 7,
				Scalar(0, 0, 255), 2, 8);
	}
	return result;
}

void LandmarkDetection::matchingDirectory(Image refImage, QString folderImages,
		QString lmPath, QString savePath, int templSize, int sceneSize, double angleDiff) {
	QFileInfoList files = Image::readFolder(folderImages);
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name);

		int index2 = sceneImage.getFileName().lastIndexOf("/");
		QString scenename = sceneImage.getFileName().mid(index2 + 1,
				sceneImage.getFileName().length() - index2 - 5);
		qDebug() << scenename;
		Mat result = matchingTemplate(refImage,sceneImage,lmPath,templSize,sceneSize,angleDiff);
		QString saveImg = savePath + "/"
				+ _name.mid(index2 + 1, _name.length() - index2 - 5).replace(
						" ", "") + ".JPG";
		imwrite(saveImg.toStdString().c_str(), result);
		if(i == 10)
			break;
	}
}

/**
 * Get the landmarks of image
 * @return: the list of landmarks in image
 */
vector<Landmark> LandmarkDetection::getLandmarks(Image image) {
	vector<Landmark> landmarks;
	return landmarks;
}

} /* namespace impls_2015 */
