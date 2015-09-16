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
Scenario::Scenario(IExtraction* extraction) {
	this->extraction = extraction;

}

Scenario::~Scenario() {

}
vector<Line> Scenario::segment(Image image) {
	EdgeSegmentation* edSegment =
			dynamic_cast<EdgeSegmentation *>(this->extraction);
	vector<Edge> listEdges = edSegment->getEdges(image); // get step edges from image
	vector<Line> appLines = edSegment->lineSegment(listEdges);
	return appLines;
}
cv::Mat Scenario::edgeSegmentation(Image image) {

	cv::Mat result(image.getMatImage().size(), CV_8UC3, cv::Scalar(0, 0, 0));
	// detect the step edges
	EdgeSegmentation* edSegment =
			dynamic_cast<EdgeSegmentation *>(this->extraction);
	vector<Line> appLines = this->segment(image);
	result = edSegment->rePresentation(result, appLines);
	return result;

}

cv::Mat Scenario::pairwiseHistogram(Image image) {
	vector<Line> appLines = this->segment(image);
	ShapeHistogram shapeHist(appLines);

	shapeHist.createTriangle();
	shapeHist.createShape();
	return shapeHist.presentation(ShapeHistogram::TwoTimeDegree);
}
double Scenario::histogramMatching(Image refImage, Image sceneImage,
		MatchingMethod matching, ShapeHistogram::AccuracyPGH angleAcc) {

	vector<Line> refLines = this->segment(refImage);
	vector<Line> sceneLines = this->segment(sceneImage);

	clock_t t1, t2;
	t1 = clock();
	ShapeHistogram refHist(refLines);
	refHist.constructPGH(angleAcc,0);

	ShapeHistogram sceneHist(sceneLines);
	sceneHist.constructPGH(angleAcc,refHist.getMaxDistance());
	double distance = getDistanceMetric(refHist, sceneHist, matching);
	t2 = clock();
	qDebug() << "time save PGH: " << ((float) t2 - (float) t1) / CLOCKS_PER_SEC
			<< " seconds";
	return distance;
}
double Scenario::histogramMatching(ShapeHistogram refHist, Image sceneImage,
		MatchingMethod matching, ShapeHistogram::AccuracyPGH angleAcc) {
	vector<Line> refLines = this->segment(sceneImage);
	ShapeHistogram sceneHist(refLines);
	sceneHist.constructPGH(angleAcc,refHist.getMaxDistance());
	double distance = getDistanceMetric(refHist, sceneHist, matching);
	return distance;
}

void Scenario::matchingDirectory(Image refImage, QString directoryPath,
		MatchingMethod matching, ShapeHistogram::AccuracyPGH angleAcc) {
	QDir qdir;
	qdir.setPath(directoryPath);
	qdir.setFilter(QDir::Files);
	QStringList filterNames;
	filterNames << "*.JPG";
	qdir.setNameFilters(QStringList("*.JPG"));
	QFileInfoList files = qdir.entryInfoList();

	ofstream of("matching_result.txt");

	qDebug() << "Matching method: " << matching << ", angle accuracy: "
			<< angleAcc << " * 180.";
	//IExtraction *extraction = new EdgeSegmentation();
	//Scenario scenario(extraction);
	QString imgpath = refImage.getFileName();
	int index = imgpath.lastIndexOf("/");
	QString refname = imgpath.mid(index + 1, imgpath.length() - index).replace(
			" ", "");
	qDebug() << refname;
	vector<Line> lines = segment(refImage);
	ShapeHistogram refHist(lines);
	refHist.constructPGH(angleAcc,0);

	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		int index2 = _name.lastIndexOf("/");
		QString scenename =
				_name.mid(index2 + 1, _name.length() - index2).replace(" ", "");
		of << refname.toStdString() << "\t" << scenename.toStdString() << "\t";
		qDebug() << scenename;

		Image sceneImage(_name);
		vector<Line> sceneLines = segment(sceneImage);
		ShapeHistogram sceneHist(sceneLines);
		sceneHist.constructPGH(angleAcc,refHist.getMaxDistance());

		double distance = getDistanceMetric(refHist, sceneHist, matching);
		of << distance << "\n";
		qDebug() << "Metric: " << QString::number(distance, 'f', 20);

	}
	of.close();
}
double Scenario::getDistanceMetric(ShapeHistogram refHist,
		ShapeHistogram sceneHist, MatchingMethod matching) {
	double distance = 0;
	switch (matching) {
	case Bhattacharyya:
		distance = refHist.bhattacharyaMetric(sceneHist);
		break;
	case Chisquared:
		distance = refHist.chiSquaredMetric(sceneHist);
		break;
	case Intersection:
		distance = refHist.intersectionMetric(sceneHist);
		break;
	default:
		distance = refHist.bhattacharyaMetric(sceneHist);
		break;
	}
	return distance;
}
/**
 * Detect the landmarks on image automatically
 * @return: the image contains the landmarks
 */
cv::Mat Scenario::landmarksAutoDetect(Image image) {
	cv::Mat result(image.getMatImage().size(), CV_8UC3);

	// detect the step edges

	return result;
}
} /* namespace impls_2015 */
