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
double LandmarkDetection::centroidCCorelation(Image refImage, Image sceneImage,
		QString lmPath, QString slmPath, Point &center) {

	vector<Point> orgLandmarks = refImage.readLandmarksFile(
			slmPath.toStdString().c_str());
	vector<Point> eLandmarks = crossCorrelation(refImage, sceneImage, lmPath,
			500);

	Point mBary, eBary;
	double mCentroid, eCentroid;
	double distance = measureDistance(orgLandmarks, eLandmarks, mCentroid,
			eCentroid, mBary, eBary);

	qDebug() << "(" << mBary.x << "," << mBary.y << ")" << "(" << eBary.x << ","
			<< eBary.y << ")" << "\t" << mCentroid << "\t" << eCentroid << "\t"
			<< abs(mCentroid - eCentroid);

	return distance;
}
void LandmarkDetection::centroidCCorelations(Image refImage, QString lmPath,
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
		vector<Point> orgLandmarks = sceneImage.readLandmarksFile(
				spath.toStdString().c_str());
		vector<Point> eLandmarks = crossCorrelation(refImage, sceneImage,
				lmPath, 500);
		of << scenename.toStdString().c_str() << ".JPG" << "\t";
		qDebug() << scenename;
		Point mBary, eBary;
		double mCentroid, eCentroid;
		double distance = measureDistance(orgLandmarks, eLandmarks, mCentroid,
				eCentroid, mBary, eBary);

		of << "(" << mBary.x << "," << mBary.y << ")" << "\t" << "(" << eBary.x
				<< "," << eBary.y << ")" << "\t";

		of << mCentroid << "\t" << eCentroid << "\t" << distance << "\n";
	}
	of.close();
}
double LandmarkDetection::measureDistance(vector<Point> orgLandmarks,
		vector<Point> esLandmarks, double &mCentroid, double &eCentroid,
		Point &mBary, Point &eBary) {
	int totalX = 0, totalxE = 0;
	int totalY = 0, totalyE = 0;
	for (size_t j = 0; j < orgLandmarks.size(); j++) {
		Point lm = orgLandmarks.at(j);
		totalX += lm.x;
		totalY += lm.y;

		Point elm = esLandmarks.at(j);
		totalxE += elm.x;
		totalyE += elm.y;
	}
	//Point center;
	mBary.x = totalX / 18;
	mBary.y = totalY / 18;

	//Point baryE;
	eBary.x = totalxE / 18;
	eBary.y = totalyE / 18;

	double totalM = 0;
	double totalS = 0;
	for (size_t k = 0; k < orgLandmarks.size(); k++) {
		Point lm = orgLandmarks.at(k);
		Line line(lm, mBary);
		totalM += (line.length() * line.length());

		Line sline(eBary, Point(esLandmarks.at(k).x, esLandmarks.at(k).y));
		totalS += (sline.length() * sline.length());
	}
	mCentroid = sqrt(totalM);
	eCentroid = sqrt(totalS);
	double subtraction = abs(mCentroid - eCentroid);
	return subtraction;
}
Mat LandmarkDetection::rotateImage(Mat source, double angle) {
	Mat dest = Mat::zeros(source.size(), source.type());
	Point2f pt(dest.cols / 2, dest.rows / 2);
	Mat rotate = getRotationMatrix2D(pt, angle, 1);
	warpAffine(source, dest, rotate, source.size());
	return dest;
}
Mat LandmarkDetection::matchingTemplate(Image refImage, Image sceneImage,
		QString lmPath, int templSize, int sceneSize, double angleDiff,
		vector<Point> &mcResult) {
	vector<Point> refLandmarks = refImage.readLandmarksFile(
			lmPath.toStdString());
	Mat sMatrix = sceneImage.getMatrixImage();
	Mat mMatrix = refImage.getMatrixImage();
	Mat sRotateImg = rotateImage(sMatrix, angleDiff);
	PHoughTransform pht;
	vector<Point> esLandmarks = pht.estimateLandmarks(refImage, sceneImage,
			lmPath.toStdString(), angleDiff);
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
		mcResult.push_back(iLocation + maxLoc + tDistance);
	}
	Mat result(sRotateImg.clone());
	for (size_t j = 0; j < mcResult.size(); j++) {
		circle(result, mcResult.at(j), 5, Scalar(0, 255, 255), 2, 8);
		circle(result,
				Point(sceneLandmarks.at(j).x,
						sMatrix.rows - sceneLandmarks.at(j).y), 7,
				Scalar(0, 0, 255), 2, 8);
	}
	return result;
}
void LandmarkDetection::centroidMatchings(Image refImage, QString lmPath,
		QString folderImages, QString folderlandmarks) {
	QFileInfoList files = Image::readFolder(folderImages);
	ofstream of("/home/linh/Desktop/cross/centroidMatching.PHG");
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name);
		int index2 = sceneImage.getFileName().lastIndexOf("/");
		QString scenename = sceneImage.getFileName().mid(index2 + 1,
				sceneImage.getFileName().length() - index2 - 5);
		QString spath = folderlandmarks + "/" + scenename + ".TPS";
		vector<Point> orgLandmarks = sceneImage.readLandmarksFile(
				spath.toStdString().c_str());
		vector<Point> mcResult;
		double angle;
		matchingTemplate(refImage,sceneImage,lmPath,200,1400,angle,mcResult);
		of << scenename.toStdString().c_str() << ".JPG" << "\t";
		qDebug() << scenename;
		Point mBary, eBary;
		double mCentroid, eCentroid;
		double distance = measureDistance(orgLandmarks, mcResult, mCentroid,
				eCentroid, mBary, eBary);

		of << "(" << mBary.x << "," << mBary.y << ")" << "\t" << "(" << eBary.x
				<< "," << eBary.y << ")" << "\t";

		of << mCentroid << "\t" << eCentroid << "\t" << distance << "\n";
	}
	of.close();
}
void LandmarkDetection::matchingDirectory(Image refImage, QString folderImages,
		QString lmPath, QString savePath, int templSize, int sceneSize,
		double angleDiff) {
	QFileInfoList files = Image::readFolder(folderImages);
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name);

		int index2 = sceneImage.getFileName().lastIndexOf("/");
		QString scenename = sceneImage.getFileName().mid(index2 + 1,
				sceneImage.getFileName().length() - index2 - 5);
		qDebug() << scenename;
		vector<Point> mcresult;
		Mat result = matchingTemplate(refImage, sceneImage, lmPath, templSize,
				sceneSize, angleDiff, mcresult);
		QString saveImg = savePath + "/"
				+ _name.mid(index2 + 1, _name.length() - index2 - 5).replace(
						" ", "") + ".JPG";
		imwrite(saveImg.toStdString().c_str(), result);
		if (i == 10)
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
