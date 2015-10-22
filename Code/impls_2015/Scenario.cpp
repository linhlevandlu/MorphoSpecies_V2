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
	landmarks = lmdetection.crossCorrelation(image, sceneImage, lpath, 200);
	return landmarks;
}

void Scenario::cCorelationDirectory(Image refImage, QString path,
		QString savePath, QString lmPath) {
	LandmarkDetection lmdetection;
	lmdetection.cCorelationByDirectory(refImage, path, savePath, lmPath);
}

double Scenario::mDistanceByCrossCorrelation(Image image, QString lmpath,
		Image sceneImage, int size, Point &ebary) {
	LandmarkDetection lmdetection;
	double rs = lmdetection.centroidCCorelation(image, sceneImage, lmpath, size,
			ebary);
	return rs;
}

void Scenario::mDistanceByCrossCorrelationDir(Image refImage, QString lmpath,
		QString imgFolder) {
	// centroid point with folder
	//QString lmfolder = "/home/linh/Desktop/landmarks";
	LandmarkDetection lmdetection;
	lmdetection.centroidCCorelations(refImage, lmpath, imgFolder);
	//lmdetection.centroidCCorelations(refImage, lmpath, imgFolder,lmfolder);
}
Mat Scenario::landmarksMatching(Image refImage, Image sceneImage,
		QString reflmPath, int templSize, int scnSize) {
	LandmarkDetection lmdetection;
	double angle;
	vector<Point> mcResult;
	return lmdetection.matchingTemplate(refImage, sceneImage, reflmPath,
			templSize, scnSize, angle, mcResult);
}
void Scenario::landmarksMatchingDirectory(Image refImage, QString folderImages,
		QString lmPath, QString savePath, int templSize, int sceneSize) {

	LandmarkDetection lmdetection;
	double angle;
	lmdetection.matchingDirectory(refImage, folderImages, lmPath, savePath,
			templSize, sceneSize, angle);

}

double Scenario::mDistanceByTemplateMatching(Image refImage, Image sceneImage,
		QString lmPath, int templSize, int sceneSize, Point &ebary) {
	LandmarkDetection lmdetection;
	return lmdetection.centroidMatching(refImage, sceneImage, lmPath, templSize,
			sceneSize, ebary);
}

void Scenario::mDistanceByTemplateMatchingDirectory(Image refImage,
		QString lmPath, QString folderImages, int templSize, int sceneSize) {
	// compute the centroid
	QString lmFolder = "/home/linh/Desktop/landmarks";
	LandmarkDetection lmdetection;
	lmdetection.centroidMatchingDirectory(refImage, lmPath, folderImages,templSize,sceneSize);
	//lmdetection.centroidMatchingDirectory(refImage, lmPath, folderImages,lmFolder,templSize,sceneSize);
}
} /* namespace impls_2015 */
