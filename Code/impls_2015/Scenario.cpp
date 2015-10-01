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

void Scenario::edgeSegmentationDirectory(QString inputFolder,QString saveFolder){
	EdgeSegmentation edgeSegment;
	edgeSegment.segmentDirectory(inputFolder,saveFolder);
}
vector<LocalHistogram> Scenario::pairwiseHistogram(Image image,
		LocalHistogram::AccuracyPGH angleAcc, int columns, cv::Mat &result) {
	GeometricHistogram geomHistogram;
	vector<LocalHistogram> pghHist = geomHistogram.shapeHistogram(image,
			angleAcc,columns, result);

	return pghHist;
}
void Scenario::pairwiseHistogramDirectory(QString folderPath,LocalHistogram::AccuracyPGH angleAcc, int colums){
	qDebug()<<"Pairwise for directory: ";
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
	for(size_t t=0;t<angleAccs.size();t++){
		LocalHistogram::AccuracyPGH acc = angleAccs.at(t);
		for(size_t k=0;k < columns.size();k++){
			int cl = columns.at(k);
			geomHistogram.pairwiseHistogramDirectory(folderPath,acc,cl);
		}
	}
}

double Scenario::pghMatching(Image refImage, Image sceneImage,
		GeometricHistogram::MatchingMethod matching,
		LocalHistogram::AccuracyPGH angleAcc) {
	GeometricHistogram geomHistogram;
	return geomHistogram.pghHistogramMatching(refImage, sceneImage, matching,
			angleAcc);
}

void Scenario::matchingDirectory(Image refImage, QString directoryPath,
		GeometricHistogram::MatchingMethod matching,
		LocalHistogram::AccuracyPGH angleAcc) {
	qDebug() << "Matching directory";
	GeometricHistogram geomHistogram;
	geomHistogram.pghHistogramDirectoryMatching(refImage, directoryPath, matching,
			angleAcc);
}

void Scenario::probabilisticHoughTransform(vector<Line> refLines, vector<Line> sceneLines, int width, int height){
	PHoughTransform pht;

	pht.test(refLines,sceneLines,width, height);
}

/**
 * Detect the landmarks on image automatically
 * @return: the image contains the landmarks
 */
vector<Landmark> Scenario::landmarksAutoDetect(Image image) {
	//cv::Mat result(image.getMatImage().size(), CV_8UC3);

	// detect the step edges
	vector<Landmark> landmarks;
	return landmarks;
}
} /* namespace impls_2015 */
