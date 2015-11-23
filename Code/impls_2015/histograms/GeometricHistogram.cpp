/*
 * GeometricHistogram.cpp
 *
 *  Created on: Sep 17, 2015
 *      Author: linh
 *  Image processing for morphometrics (IPM) Version 0.2
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

#include "GeometricHistogram.h"

namespace impls_2015 {

GeometricHistogram::GeometricHistogram() {
	// TODO Auto-generated constructor stub

}

GeometricHistogram::~GeometricHistogram() {
	// TODO Auto-generated destructor stub
}

/*
 * Construct the PGH of an image followed an accuracy
 * @parameter 1: image - the input image
 * @parameter 2: angleAcc - the angle accuracy
 * @parameter 3: columns - the distance accuracy
 * @parameter 4 - output: result - the matrix presented the PGH
 * @return: list of local histogram
 */
vector<LocalHistogram> GeometricHistogram::shapeHistogram(Image image,
		LocalHistogram::AccuracyPGH angleAcc, int distanceAcc,
		Image::SegmentMethod sgmethod, cv::Mat &result,
		vector<vector<int> > &matrix) {
	ShapeHistogram pghHistogram = image.getShapeHistogram();

	int threshValue = 0;
	vector<Line> lines = image.lineSegment(sgmethod, threshValue);
	vector<LocalHistogram> listLocalPGH;

	listLocalPGH = pghHistogram.constructPGH(lines);
	matrix = pghHistogram.constructMatPGH(listLocalPGH,angleAcc, distanceAcc);
	result = pghHistogram.presentation(listLocalPGH, angleAcc, distanceAcc);

	return listLocalPGH;
}

/*
 * Compute the measure distance between two images
 * @parameter 1: refImage - the first (reference) image
 * @parameter 2: sceneImage - the second (scene) image
 * @parameter 3: matchingMethod - the matching method
 * @parameter 4: angleAcc - the angle accuracy
 * @parameter 5: distanceAcc - the distance accuracy
 * @return: the measure distance between two images
 */
double GeometricHistogram::pghHistogramMatching(Image refImage,
		Image sceneImage, MatchingMethod matchingMethod,
		LocalHistogram::AccuracyPGH angleAcc, int distanceAcc,
		Image::SegmentMethod sgmethod) {

	int t1 = 0, t2 = 0;
	ShapeHistogram refHist = refImage.getShapeHistogram();
	vector<LocalHistogram> refLocalHists = refHist.constructPGH(refImage.lineSegment(sgmethod, t1));
	refHist.constructMatPGH(refLocalHists,angleAcc, distanceAcc);

	ShapeHistogram sceneHist = sceneImage.getShapeHistogram();
	vector<LocalHistogram> sceneLocalHists =sceneHist.constructPGH(sceneImage.lineSegment(sgmethod, t2));
	sceneHist.constructMatPGH(sceneLocalHists, angleAcc, distanceAcc);

	/*vector<Line> set2 = readFile("test/segmentation_translate200/Md028.PGH");
	 sceneHist.constructPGH(set2);
	 sceneHist.constructMatPGH(angleAcc, 250);*/

	double distance = getDistanceMetric(refHist, sceneHist, matchingMethod);

	return distance;
}

/*
 * Compute the measure distance between two PGHs
 * @parameter 1: refHist - reference PGH
 * @parameter 2: sceneHist - scene PGH
 * @parameter 3: matching - matching method
 * @return: the measure distance between two PGHs followed the matching method
 */
double GeometricHistogram::getDistanceMetric(ShapeHistogram refHist,
		ShapeHistogram sceneHist, MatchingMethod matching) {
	double distance = 0;
	switch (matching) {
	case GeometricHistogram::Bhattacharyya:
		distance = refHist.bhattacharyaMetric(sceneHist);
		break;
	case GeometricHistogram::Chisquared:
		distance = refHist.chiSquaredMetric(sceneHist);
		break;
	case GeometricHistogram::Intersection:
		distance = refHist.intersectionMetric(sceneHist);
		break;
	default:
		distance = refHist.bhattacharyaMetric(sceneHist);
		break;
	}
	return distance;
}

/*
 * Compute the measure distance between pairs of images in a folder
 * @parameter 1: folderPath - the path of images folder
 * @parameter 2: method - the matching method
 * @parameter 3: angleAcc - the angle accuracy
 * @parameter 4: distanceAcc - the distance accuracy
 */
void GeometricHistogram::phgHistogramDirMatching(QString folderPath,
		MatchingMethod method, LocalHistogram::AccuracyPGH angleAcc,
		int distanceAcc) {
	QFileInfoList files = Image::readImagesFolder(folderPath.toStdString());

	QString refName;
	QString sceneName;
	ofstream of("matching_result.txt");

	for (int i = 0; i < files.size(); i++) {
		QFileInfo reffile = files.at(i);
		QString _name = reffile.absoluteFilePath();
		int index2 = _name.lastIndexOf("/");
		refName = _name.mid(index2 + 1, _name.length() - index2).replace(" ",
				"");
		Image refImage(_name.toStdString());
		ShapeHistogram refHist = refImage.getShapeHistogram();
		vector<LocalHistogram>refLocalHist = refHist.constructPGH(refImage.lineSegment());
		refHist.constructMatPGH(refLocalHist, angleAcc, distanceAcc);
		of << refName.toStdString() << "\t";
		for (int j = 0; j < files.size(); j++) {
			if (j >= i) {
				QFileInfo scenefile = files.at(j);
				QString _name2 = scenefile.absoluteFilePath();
				int index2 = _name2.lastIndexOf("/");
				sceneName =
						_name2.mid(index2 + 1, _name2.length() - index2).replace(
								" ", "");
				Image sceneImage(_name2.toStdString());
				ShapeHistogram sceneHist = sceneImage.getShapeHistogram();
				vector<LocalHistogram> sceneLocalHists = sceneHist.constructPGH(sceneImage.lineSegment());
				sceneHist.constructMatPGH(sceneLocalHists, angleAcc, distanceAcc);
				double distance = getDistanceMetric(refHist, sceneHist, method);

				of << distance << "\t";
				qDebug() << refName << "\t" << sceneName;
				qDebug() << "Metric: " << QString::number(distance, 'f', 20);
			} else {
				of << "\t";
			}

		}
		of << "\n";
	}
	of.close();

}

/*
 * Compute the measure distance between an image and the images in a folder
 * @parameter 1: refImage - reference image
 * @parameter 2: folderPath - path of scene images folder
 * @parameter 3: matchingMethod - the matching method
 * @parameter 4: angleAcc - the angle accuracy
 * @parameter 5: distanceAcc - the distance accuracy
 */
void GeometricHistogram::pghHistogramDirectoryMatching(Image refImage,
		QString folderPath, MatchingMethod matchingMethod,
		LocalHistogram::AccuracyPGH angleAcc, int distanceAcc, QString fileSave,
		Image::SegmentMethod sgmethod) {
	QFileInfoList files = Image::readImagesFolder(folderPath.toStdString());

	ofstream of(fileSave.toStdString().c_str());

	qDebug() << "Matching method: " << matchingMethod << ", angle accuracy: "
			<< angleAcc << " * 180.";

	int rtValue, stValue;

	QString refname = refImage.getName();
	ShapeHistogram refHist = refImage.getShapeHistogram();
	vector<LocalHistogram>refLocalHists = refHist.constructPGH(refImage.lineSegment(sgmethod, rtValue));
	refHist.constructMatPGH(refLocalHists, angleAcc, distanceAcc);

	of<<"Reference image "<< refname.toStdString() <<".JPG" <<"\n";
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name.toStdString());
		qDebug() << sceneImage.getName();

		ShapeHistogram sceneHist = sceneImage.getShapeHistogram();
		vector<LocalHistogram>sceneLocalHists = sceneHist.constructPGH(sceneImage.lineSegment(sgmethod, stValue));
		sceneHist.constructMatPGH(sceneLocalHists, angleAcc, distanceAcc);

		of<< sceneImage.getName().toStdString()<<".JPG" << "\t";

		double distance = getDistanceMetric(refHist, sceneHist, matchingMethod);
		of << distance << "\n";
		qDebug() << "Metric: " << QString::number(distance, 'f', 20);

	}
	of.close();
}

/*
 * Construct the PGH for the images in a folder
 * @parameter 1: folderPath - the images folder
 * @parameter 2: angleAcc -the angle accuracy
 * @parameter 3: columns - the distance accuracy
 */
void GeometricHistogram::pairwiseHistogramDirectory(QString folderPath,
		LocalHistogram::AccuracyPGH angleAcc, int distanceAcc) {
	QDir qdir;
	qdir.setPath(folderPath);
	qdir.setFilter(QDir::Files);
	QStringList filterNames;
	filterNames << "*.JPG";
	qdir.setNameFilters(QStringList("*.JPG"));
	QFileInfoList files = qdir.entryInfoList();
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		int index2 = _name.lastIndexOf("/");
		QString savePath = "test/pghmatrix/"
				+ _name.mid(index2 + 1, _name.length() - index2 - 5).replace(
						" ", "") + "_L"
				+ QString::number(LocalHistogram::heightOfAngleAxis(angleAcc))
				+ "_C" + QString::number(distanceAcc) + ".PGH";
		qDebug() << savePath;
		Image sceneImage(_name.toStdString());
		vector<Line> lines = sceneImage.lineSegment();
		vector<LocalHistogram> listLocalPGH;
		ShapeHistogram sceneHist = sceneImage.getShapeHistogram();
		listLocalPGH = sceneHist.constructPGH(lines);
		vector<vector<int> > matrix = sceneHist.constructMatPGH(listLocalPGH,angleAcc,
				distanceAcc);
		sceneHist.writeMatrix(matrix, savePath);
	}
}

} /* namespace impls_2015 */
