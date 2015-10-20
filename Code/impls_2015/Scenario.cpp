/*
 * Scenario.cpp
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

#include "Scenario.h"

namespace impls_2015 {
/**
 * Constructor
 */
Scenario::Scenario() {

}

Scenario::~Scenario() {

}
/*vector<Line> Scenario::segment(Image image) {
 EdgeSegmentation* edSegment =
 dynamic_cast<EdgeSegmentation *>(this->extraction);
 vector<Edge> listEdges = edSegment->getEdges(image); // get step edges from image
 vector<Line> appLines = edSegment->lineSegment(listEdges);
 return appLines;
 }*/
vector<Line> Scenario::edgeSegmentation(Image image, cv::Mat &result) {
	vector<Line> appLines;
	EdgeSegmentation sgMethod;
	appLines = sgMethod.lineSegment(image);

	Mat temp(image.getMatrixImage().size(), CV_8UC3, cv::Scalar(0, 0, 0));
	temp.copyTo(result);
	result = sgMethod.rePresentation(result, appLines);
	return appLines;
}

void Scenario::edgeSegmentationDirectory(QString inputFolder,
		QString saveFolder) {
	EdgeSegmentation edgeSegment;
	edgeSegment.segmentDirectory(inputFolder, saveFolder);
}
vector<LocalHistogram> Scenario::pairwiseHistogram(Image image,
		LocalHistogram::AccuracyPGH angleAcc, int columns, cv::Mat &result) {
	GeometricHistogram geomHistogram;
	vector<LocalHistogram> pghHist = geomHistogram.shapeHistogram(image,
			angleAcc, columns, result);

	return pghHist;
}
void Scenario::pairwiseHistogramDirectory(QString folderPath,
		LocalHistogram::AccuracyPGH angleAcc, int colums) {
	qDebug() << "Pairwise for directory: ";
	GeometricHistogram geomHistogram;

	vector<LocalHistogram::AccuracyPGH> angleAccs;
	angleAccs.push_back(LocalHistogram::HaftDegree);
	angleAccs.push_back(LocalHistogram::Degree);
	angleAccs.push_back(LocalHistogram::TwoTimeDegree);
	angleAccs.push_back(LocalHistogram::FourTimeDegree);
	angleAccs.push_back(LocalHistogram::SixTimeDegree);

	vector<int> columns;
	columns.push_back(250);
	columns.push_back(500);
	columns.push_back(1000);
	for (size_t t = 0; t < angleAccs.size(); t++) {
		LocalHistogram::AccuracyPGH acc = angleAccs.at(t);
		for (size_t k = 0; k < columns.size(); k++) {
			int cl = columns.at(k);
			geomHistogram.pairwiseHistogramDirectory(folderPath, acc, cl);
		}
	}
}

double Scenario::pghMatching(Image refImage, Image sceneImage,
		GeometricHistogram::MatchingMethod matching,
		LocalHistogram::AccuracyPGH angleAcc, int distanceAcc) {
	GeometricHistogram geomHistogram;
	return geomHistogram.pghHistogramMatching(refImage, sceneImage, matching,
			angleAcc, distanceAcc);
}

void Scenario::matchingDirectory(Image refImage, QString directoryPath,
		GeometricHistogram::MatchingMethod matching,
		LocalHistogram::AccuracyPGH angleAcc, int distanceAcc) {
	qDebug() << "Matching directory";
	GeometricHistogram geomHistogram;
	geomHistogram.pghHistogramDirectoryMatching(refImage, directoryPath,
			matching, angleAcc, distanceAcc);
}

void Scenario::matchingDirectory(QString directoryPath,
		GeometricHistogram::MatchingMethod matching,
		LocalHistogram::AccuracyPGH angleAcc, int distanceAcc) {
	qDebug() << "Matching directory";
	GeometricHistogram geomHistogram;
	geomHistogram.phgHistogramDirMatching(directoryPath, matching, angleAcc,
			distanceAcc);
}

void Scenario::probabilisticHoughTransform(vector<Line> refLines,
		vector<Line> sceneLines, QString lmPath, int width, int height) {
	PHoughTransform pht;

	pht.test(refLines, sceneLines, lmPath, width, height);
}

Mat Scenario::probabilisticHoughTransform(Image refImage, Image sceneImage,
		QString reflmPath) {
	PHoughTransform pht;
	return pht.phtPresentation(refImage, sceneImage, reflmPath.toStdString());
}
void Scenario::phtDirectory(Image refImage, QString reflmPath, QString sceneDir,
		QString scenelmDir, QString saveDir) {
	PHoughTransform pht;
	pht.phtDirectory(refImage, reflmPath, sceneDir, scenelmDir, saveDir);
}

/**
 * Detect the landmarks on image automatically
 * @return: the image contains the landmarks
 */
vector<Point> Scenario::landmarksByCrossCorelation(Image image, QString lpath,
		Image sceneImage) {

	vector<Point> landmarks;
	LandmarkDetection lmdetection;
	landmarks = lmdetection.crossCorrelation(image, sceneImage,lpath,200);

	// xac dinh centroid point
	/*Point point;
	 int index2 = sceneImage.getFileName().lastIndexOf("/");
	 QString scenename = sceneImage.getFileName().mid(index2 + 1,
	 sceneImage.getFileName().length() - index2 - 5);
	 qDebug() << scenename;
	 QString slmPath = "/home/linh/Desktop/landmarks/" + scenename + ".TPS";
	 double rs = lmdetection.centroid(image, sceneImage, lpath, slmPath, point);
	 qDebug() << "Point: " << point.x << ", " << point.y << ", centroid:  "
	 << rs;*/

	// centroid point with folder
	/*QString folder = "/home/linh/Desktop/mandibule";
	 QString lmfolder ="/home/linh/Desktop/landmarks";
	 lmdetection.centroidCCorelations(image,lpath,folder,lmfolder);*/
	return landmarks;
}
Mat Scenario::landmarksMatching(Image refImage, Image sceneImage,
		QString reflmPath, int templSize, int scnSize) {
	LandmarkDetection lmdetection;
	double angle;
	vector<Point> mcResult;
	return lmdetection.matchingTemplate(refImage, sceneImage, reflmPath,
			templSize, scnSize, angle,mcResult);
}
void Scenario::landmarksMatchingDirectory(Image refImage, QString folderImages,
		QString lmPath, QString savePath, int templSize, int sceneSize){
	LandmarkDetection lmdetection;
	// estimated the landmarks
	//double angle;
	//return lmdetection.matchingDirectory(refImage,folderImages,lmPath,savePath,templSize,sceneSize,angle);

	// compute the centroid
	QString lmFolder = "/home/linh/Desktop/landmarks";
	lmdetection.centroidMatchingDirectory(refImage,lmPath,folderImages,lmFolder);

}
void Scenario::cCorelationDirectory(Image refImage, QString path,
		QString savePath, QString lmPath) {
	LandmarkDetection lmdetection;
	lmdetection.cCorelationByDirectory(refImage, path, savePath, lmPath);
}
} /* namespace impls_2015 */
