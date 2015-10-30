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
	Mat temp(matImage.clone());
	if (landmark.x <= 0 || landmark.y <= 0)
		return temp;
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

Point LandmarkDetection::matCrossCorrelation(Mat templ, Mat scene) {
	int width = scene.cols - templ.cols + 1;
	int height = scene.rows - templ.rows + 1;
	if (width > 0 && height > 0) {
		Mat result(width, height, CV_32FC1);
		cv::matchTemplate(scene, templ, result, CV_TM_CCORR_NORMED);
		double maxValue, minValue;
		Point maxLoc, minLoc;
		cv::minMaxLoc(result, &minValue, &maxValue, &minLoc, &maxLoc, Mat());
		return maxLoc;
	}
	return Point(0, 0);
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
		Point maxLoc = matCrossCorrelation(templ, img);
		sceneLandmarks.push_back(iLocation + maxLoc + tDistance);
	}
	return sceneLandmarks;
}

void LandmarkDetection::cCorrelationByDirectory(Image refImage,
		QString folderpath, QString savepath, QString lmPath) {

	QFileInfoList files = Image::readImagesFolder(folderpath);

	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name);

		cv::Mat result(sceneImage.getMatrixImage().clone());
		vector<Point> landmarks = crossCorrelation(refImage, sceneImage, lmPath,
				400);
		QString scenename = sceneImage.getName();
		qDebug() << scenename;

		for (size_t i = 0; i < landmarks.size(); i++) {
			Point lm = landmarks.at(i);
			circle(result, Point(lm.x, lm.y), 5, Scalar(0, 0, 255), 2, 4);
		}

		QString saveImg = savepath + "/" + scenename + ".JPG";
		imwrite(saveImg.toStdString().c_str(), result);
	}
	//meanDiff.close();
}
double LandmarkDetection::centroidCCorrelation(Image refImage, Image sceneImage,
		QString lmPath, int size, Point &ebary) {

	/*QString scenename = sceneImage.getName();
	 qDebug() << scenename;
	 QString slmPath = "/home/linh/Desktop/landmarks/" + scenename + ".TPS";
	 vector<Point> orgLandmarks = refImage.readLandmarksFile(
	 slmPath.toStdString().c_str());*/

	vector<Point> eLandmarks = crossCorrelation(refImage, sceneImage, lmPath,
			size);
	double eCentroid;
	eCentroid = measureDistance(eLandmarks, ebary);

	return eCentroid;
}
void LandmarkDetection::centroidCCorrelations(Image refImage, QString lmPath,
		QString folderImages, QString folderlandmarks) {

	QFileInfoList files = Image::readImagesFolder(folderImages);
	ofstream of("results/cc_ecentroid.PHG");
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name);

		QString scenename = sceneImage.getName();
		QString spath = folderlandmarks + "/" + scenename + ".TPS";
		vector<Point> orgLandmarks = sceneImage.readLandmarksFile(
				spath.toStdString().c_str());
		vector<Point> eLandmarks = crossCorrelation(refImage, sceneImage,
				lmPath, 400);
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
void LandmarkDetection::centroidCCorrelations(Image refImage, QString lmPath,
		QString folderImages) {
	qDebug() << "Compute centroid by correlation";
	QFileInfoList files = Image::readImagesFolder(folderImages);
	ofstream of("results/cc_ecentroid.PHG");
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name);

		QString scenename = sceneImage.getName();
		vector<Point> eLandmarks = crossCorrelation(refImage, sceneImage,
				lmPath, 400);
		of << scenename.toStdString().c_str() << ".JPG" << "\t";
		qDebug() << scenename;
		Point eBary;
		double eCentroid;
		eCentroid = measureDistance(eLandmarks, eBary);
		of << "(" << eBary.x << "," << eBary.y << ")" << "\t";
		of << eCentroid << "\n";
	}
	of.close();
}
double LandmarkDetection::measureDistance(vector<Point> landmarks,
		Point &bary) {

	int totalX = 0, totalY = 0;
	size_t lmSize = landmarks.size();
	for (size_t j = 0; j < landmarks.size(); j++) {
		Point lm = landmarks.at(j);
		totalX += lm.x;
		totalY += lm.y;
	}
	if (lmSize > 0) {
		bary.x = totalX / lmSize;
		bary.y = totalY / lmSize;
	} else {
		bary.x = 0;
		bary.y = 0;
	}
	double totalDistance = 0;
	for (size_t j = 0; j < landmarks.size(); j++) {
		Point lm = landmarks.at(j);
		Line line(lm, bary);
		totalDistance += (line.length() * line.length());
	}
	return sqrt(totalDistance);
}
double LandmarkDetection::measureDistance(vector<Point> orgLandmarks,
		vector<Point> esLandmarks, double &mCentroid, double &eCentroid,
		Point &mBary, Point &eBary) {
	mCentroid = measureDistance(orgLandmarks, mBary);
	eCentroid = measureDistance(esLandmarks, eBary);
	double subtraction = abs(mCentroid - eCentroid);
	return subtraction;
}
Mat LandmarkDetection::rotateImage(Mat source, double angle, Point center) {
	if (angle > 0)
		angle = -angle;
	Mat dest = Mat::zeros(source.size(), source.type());
	//Point2f pt(dest.cols / 2, dest.rows / 2);
	Mat rotate = getRotationMatrix2D(center, angle, 1);
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

	PHoughTransform pht;
	Point ePoint;
	vector<Point> esLandmarks = pht.estimateLandmarks(refImage, sceneImage,
			lmPath.toStdString(), angleDiff, ePoint);

	//QString scenename = sceneImage.getName();
	//QString spath = "/home/linh/Desktop/mg/landmarks/" + scenename + ".TPS";
	//vector<Point> sceneLandmarks = sceneImage.readLandmarksFile(
	//		spath.toStdString().c_str());

	Mat sRotateImg = rotateImage(sMatrix, angleDiff, ePoint);
	for (size_t i = 0; i < esLandmarks.size(); i++) {
		Point lm(refLandmarks.at(i).x, mMatrix.rows - refLandmarks.at(i).y);
		Point tLocation, tDistance, iLocation, iDistance;
		Mat templ = createTemplate(mMatrix, lm, templSize, tLocation,
				tDistance);
		Mat sceneM = createTemplate(sRotateImg, esLandmarks.at(i), sceneSize,
				iLocation, iDistance);
		Point maxLoc = matCrossCorrelation(templ, sceneM);
		mcResult.push_back(iLocation + maxLoc + tDistance);
	}
	Mat sDisplay(sMatrix.clone());

	Mat result(rotateImage(sDisplay, angleDiff, ePoint).clone());
	//Mat result(sRotateImg.clone());
	for (size_t j = 0; j < mcResult.size(); j++) {
		circle(result, mcResult.at(j), 5, Scalar(0, 255, 255), 2, 8);
	}
	return result;
}

void LandmarkDetection::matchingDirectory(Image refImage, QString folderImages,
		QString lmPath, QString savePath, int templSize, int sceneSize,
		double angleDiff) {
	QFileInfoList files = Image::readImagesFolder(folderImages);
	QString sceneLMPath = "/home/linh/Desktop/mg/landmarks";
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name);

		QString scenename = sceneImage.getName();
		qDebug() << scenename;

		vector<Point> mcresult;
		Mat result = matchingTemplate(refImage, sceneImage, lmPath, templSize,
				sceneSize, angleDiff, mcresult);
		QString saveImg = savePath + "/" + scenename + ".JPG";
		imwrite(saveImg.toStdString().c_str(), result);

		QString spath = sceneLMPath + "/" + scenename + ".TPS";
		vector<Point> orgLandmarks = sceneImage.readLandmarksFile(
				spath.toStdString().c_str());

		QString estTPS(savePath + "/E " + scenename + ".TPS");
		ofstream of(estTPS.toStdString().c_str());
		of << "LM = " << mcresult.size() << "\n";

		QString diffEMTPS(savePath + "/DiffEM " + scenename + ".TPS");
		ofstream diffFile(diffEMTPS.toStdString().c_str());
		diffFile << "IMAGE = " << scenename.toStdString().c_str() << ".JPG"
				<< "\n";

		qDebug() << orgLandmarks.size() << ", " << mcresult.size();
		for (size_t k = 0; k < mcresult.size(); k++) {
			Point esPoint = mcresult.at(k);
			of << esPoint.x << " " << esPoint.y << "\n";

			Point orgPoint = orgLandmarks.at(k);
			diffFile << orgPoint.x << " " << orgPoint.y << "\t" << esPoint.x
					<< " " << esPoint.y << "\t";
			Line ldistance(esPoint, orgPoint);
			diffFile << ldistance.length() << "\n";
		}
		diffFile.close();
		of << "IMAGE = " << scenename.toStdString().c_str() << ".JPG";
		of.close();
		if (i == 293)
			break;
	}
}

double LandmarkDetection::centroidMatching(Image refImage, Image sceneImage,
		QString lmPath, int templSize, int sceneSize, Point &ebary) {
	double angleDiff, eCentroid;
	vector<Point> mcResult;
	matchingTemplate(refImage, sceneImage, lmPath, templSize, sceneSize,
			angleDiff, mcResult);
	eCentroid = measureDistance(mcResult, ebary);
	return eCentroid;
}

void LandmarkDetection::centroidMatchingDirectory(Image refImage,
		QString lmPath, QString folderImages, QString folderlandmarks,
		int templSize, int sceneSize) {
	QFileInfoList files = Image::readImagesFolder(folderImages);
	ofstream of("results/ECentroidMatching.PHG");
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name);

		QString scenename = sceneImage.getName();
		QString spath = folderlandmarks + "/" + scenename + ".TPS";
		vector<Point> orgLandmarks = sceneImage.readLandmarksFile(
				spath.toStdString().c_str());
		vector<Point> mcResult;
		double angle;
		matchingTemplate(refImage, sceneImage, lmPath, templSize, sceneSize,
				angle, mcResult);
		of << scenename.toStdString().c_str() << ".JPG" << "\t";
		qDebug() << scenename;
		Point mBary, eBary;
		double mCentroid, eCentroid;
		double distance = measureDistance(orgLandmarks, mcResult, mCentroid,
				eCentroid, mBary, eBary);

		of << "(" << mBary.x << "," << mBary.y << ")" << "\t" << "(" << eBary.x
				<< "," << eBary.y << ")" << "\t";

		of << mCentroid << "\t" << eCentroid << "\t" << distance << "\n";
		qDebug() << "value: (" << mBary.x << "," << mBary.y << ")" << "\t"
				<< "(" << eBary.x << "," << eBary.y << ")" << "\t" << mCentroid
				<< "\t" << eCentroid << "\t" << distance;
	}
	of.close();
}

void LandmarkDetection::centroidMatchingDirectory(Image refImage,
		QString lmPath, QString folderImages, int templSize, int sceneSize) {
	QFileInfoList files = Image::readImagesFolder(folderImages);
	ofstream of("results/ECentroidMatching.PHG");
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name);

		QString scenename = sceneImage.getName();
		vector<Point> mcResult;
		double angle;
		matchingTemplate(refImage, sceneImage, lmPath, templSize, sceneSize,
				angle, mcResult);
		of << scenename.toStdString().c_str() << ".JPG" << "\t";
		qDebug() << scenename;
		Point eBary(0, 0);
		double eCentroid = 0;
		eCentroid = measureDistance(mcResult, eBary);

		of << "(" << eBary.x << "," << eBary.y << ")" << "\t";

		of << eCentroid << "\n";
		qDebug() << "value: (" << eBary.x << "," << eBary.y << ")" << "\t"
				<< eCentroid;
	}
	of.close();
}

} /* namespace impls_2015 */
