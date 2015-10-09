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

Mat LandmarkDetection::createTemplate(Image image, Point landmark, int tsize,
		Point &location, Point &distance) {
	Mat matImg = image.getMatrixImage();
	int lx = (landmark.x - tsize / 2) < 0 ? 0 : (landmark.x - tsize / 2);
	int ly = (landmark.y - tsize / 2) < 0 ? 0 : (landmark.y - tsize / 2);
	location.x = lx;
	location.y = ly;
	distance.x = landmark.x - lx;
	distance.y = landmark.y - ly;
	int lx2 =
			(landmark.x + tsize / 2) > matImg.cols ?
					matImg.cols : (landmark.x + tsize / 2);
	int ly2 =
			(landmark.y + tsize / 2) > matImg.rows ?
					matImg.rows : (landmark.y + tsize / 2);

	Mat refTemplate(matImg, Rect(lx, ly, lx2 - lx, ly2 - ly));
	return refTemplate;
}

vector<Point> LandmarkDetection::crossCorrelation(Image refImage,
		Image sceneImage, QString lmPath) {
	vector<Point> refLandmarks = refImage.readLandmarksFile(lmPath);
	vector<Point> sceneLandmarks;
	Point tLocation, tDistance, iLocation, iDistance;
	for (size_t i = 0; i < refLandmarks.size(); i++) {
		Point temp = refLandmarks.at(i);
		Point lm(temp.x, refImage.getMatrixImage().rows - temp.y);

		Mat templ = createTemplate(refImage, lm, 50, tLocation, tDistance);
		Mat img = sceneImage.getMatrixImage();//createTemplate(sceneImage, lm, 1000, iLocation, iDistance);

		Mat result(img.cols - templ.cols + 1, img.rows - templ.rows + 1,
				CV_32FC1);
		cv::matchTemplate(img, templ, result, CV_TM_CCORR_NORMED);
		normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
		double maxValue, minValue;
		Point maxLoc, minLoc;
		cv::minMaxLoc(result, &minValue, &maxValue, &minLoc, &maxLoc, Mat());
		sceneLandmarks.push_back(iLocation + maxLoc + tDistance);
		/*rectangle(img, maxLoc,
		 cv::Point(maxLoc.x + templ.cols, maxLoc.y + templ.rows),
		 Scalar(0, 0, 255), 2, 8);*/
	}
	return sceneLandmarks;
}
void LandmarkDetection::landmarksByDirectory(Image refImage, QString path,
		QString savepath, QString lmPath) {
	QDir qdir;
	qdir.setPath(path);
	qdir.setFilter(QDir::Files);
	QStringList filterNames;
	filterNames << "*.JPG";
	qdir.setNameFilters(QStringList("*.JPG"));
	QFileInfoList files = qdir.entryInfoList();

	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name);

		vector<Point> landmarks = crossCorrelation(refImage, sceneImage,
				lmPath);
		cv::Mat result(sceneImage.getMatrixImage().clone());
		for (size_t i = 0; i < landmarks.size(); i++) {
			Point lm = landmarks.at(i);
			circle(result, Point(lm.x, lm.y), 5, Scalar(0, 0, 255), 2, 4);
		}

		qDebug() << _name;
		int index2 = _name.lastIndexOf("/");
		QString scenename = savepath + "/"
				+ _name.mid(index2 + 1, _name.length() - index2 - 5).replace(
						" ", "") + ".JPG";
		imwrite(scenename.toStdString().c_str(),result);
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
