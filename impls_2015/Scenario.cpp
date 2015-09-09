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
	ShapeHistogram shapeHist;

	shapeHist.createSquare();
	shapeHist.createTriangle();
	shapeHist.createTrapezoid();
	shapeHist.createShape();
	//vector<LocalHistogram> pghHistograms = shapeHist.shapePGH(appLines);
	int t_width = shapeHist.getMaxDistance();
	int height = 180;
	cv::Mat result(cv::Size(t_width + 10, height + 1), CV_8UC3,
			cv::Scalar(0, 0, 0));
	/*for (size_t t = 0; t < pghHistograms.size(); t++) {
		LocalHistogram pwh = pghHistograms[t];
		for (size_t i = 0; i < pwh.getPWHistgoram().size(); i++) {
			GFeatures gfeature = pwh.getPWHistgoram().at(i);
			int x1 = round(gfeature.getDmin());
			int x2 = round(gfeature.getDmax());
			int y = LocalHistogram::convertAngle(gfeature.getAngle());
			cv::line(result, cv::Point(x1, y), cv::Point(x2, y),
					cv::Scalar(255, 255, 255), 1, 8);
		}
	}*/
	return result;
}
double Scenario::histogramMatching(Image refImage, Image sceneImage) {

	vector<Line> refLines = this->segment(refImage);
	vector<Line> sceneLines = this->segment(sceneImage);
	ShapeHistogram refHist(refLines);
	ShapeHistogram sceneHist(sceneLines);

	return refHist.bhattacharyaMetric(sceneHist);
}
double Scenario::histogramMatching(ShapeHistogram refImage, Image sceneImage) {
	vector<Line> refLines = this->segment(sceneImage);
	ShapeHistogram shapeHist2(refLines);
	return refImage.bhattacharyaMetric(shapeHist2);
}

void Scenario::matchingDirectory(Image refImage, QString directoryPath) {
	QDir qdir;
	qdir.setPath(directoryPath);
	qdir.setFilter(QDir::Files);
	QStringList filterNames;
	filterNames << "*.JPG";
	qdir.setNameFilters(QStringList("*.JPG"));
	QFileInfoList files = qdir.entryInfoList();

	ofstream of("matching_result.txt");

	//IExtraction *extraction = new EdgeSegmentation();
	//Scenario scenario(extraction);
	QString imgpath = refImage.getFileName();
	int index = imgpath.lastIndexOf("/");
	QString refname = imgpath.mid(index + 1, imgpath.length() - index).replace(
			" ", "");
	qDebug() << refname;
	vector<Line> lines = segment(refImage);
	ShapeHistogram refShape(lines);
	refShape.savePGH(refShape.shapePGH(lines));

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
		double matching = refShape.bhattacharyaMetric(sceneHist);
		of << matching << "\n";
		qDebug() << "Metric: " << QString::number(matching, 'f', 20);
		if(i == 99)
			break;

	}
	of.close();
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
