/*
 * GeometricHistogram.cpp
 *
 *  Created on: Sep 17, 2015
 *      Author: linh
 */

#include "GeometricHistogram.h"

namespace impls_2015 {

GeometricHistogram::GeometricHistogram() {
	// TODO Auto-generated constructor stub

}

GeometricHistogram::~GeometricHistogram() {
	// TODO Auto-generated destructor stub
}
vector<LocalHistogram> GeometricHistogram::shapeHistogram(Image image,
		LocalHistogram::AccuracyPGH angleAcc, int columns, cv::Mat &result) {
	ShapeHistogram pghHistogram = image.getShapeHistogram();

	pghHistogram.createShape();

	vector<Line> lines = image.lineSegment();
	vector<LocalHistogram> listLocalPGH;
	//listLocalPGH= pghHistogram.constructPGH(lines, angleAcc, 0);

	listLocalPGH = pghHistogram.constructPGH(lines);
	pghHistogram.constructMatPGH(angleAcc, columns);

	result = pghHistogram.presentation(listLocalPGH, angleAcc, columns);

	return listLocalPGH;
}

double GeometricHistogram::pghHistogramMatching(Image refImage,
		Image sceneImage, MatchingMethod matchingMethod,
		LocalHistogram::AccuracyPGH angleAcc, int distanceAcc) {
	clock_t t1, t2;
	t1 = clock();

	ShapeHistogram refHist = refImage.getShapeHistogram();

	//refHist.constructPGH(refImage.lineSegment(), angleAcc, 0);

	refHist.constructPGH(refImage.lineSegment());
	refHist.constructMatPGH(angleAcc, distanceAcc);

	/*vector<Line> set1 = readFile("test/segmentation/Md028.PGH");
	refHist.constructPGH(set1);
	refHist.constructMatPGH(angleAcc, 250);*/

	ShapeHistogram sceneHist = sceneImage.getShapeHistogram();

	//sceneHist.constructPGH(sceneImage.lineSegment(), angleAcc,
	//		refHist.getMaxDistance());

	sceneHist.constructPGH(sceneImage.lineSegment());
	sceneHist.constructMatPGH(angleAcc, distanceAcc);

	/*vector<Line> set2 = readFile("test/segmentation_translate200/Md028.PGH");
	sceneHist.constructPGH(set2);
	sceneHist.constructMatPGH(angleAcc, 250);*/

	double distance = getDistanceMetric(refHist, sceneHist, matchingMethod);
	t2 = clock();
	qDebug() << "time save PGH: " << ((float) t2 - (float) t1) / CLOCKS_PER_SEC
			<< " seconds";
	return distance;
}
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

void GeometricHistogram::pghHistogramDirectoryMatching(Image refImage,
		QString folderPath, MatchingMethod matchingMethod,
		LocalHistogram::AccuracyPGH angleAcc) {
	QDir qdir;
	qdir.setPath(folderPath);
	qdir.setFilter(QDir::Files);
	qdir.setNameFilters(QStringList("*.JPG"));
	QFileInfoList files = qdir.entryInfoList();

	ofstream of("matching_result.txt");

	qDebug() << "Matching method: " << matchingMethod << ", angle accuracy: "
			<< angleAcc << " * 180.";
	//IExtraction *extraction = new EdgeSegmentation();
	//Scenario scenario(extraction);
	QString imgpath = refImage.getFileName();
	int index = imgpath.lastIndexOf("/");
	QString refname = imgpath.mid(index + 1, imgpath.length() - index).replace(
			" ", "");
	qDebug() << refname;
	ShapeHistogram refHist = refImage.getShapeHistogram();
	refHist.constructPGH(refImage.lineSegment(), angleAcc, 0);

	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		int index2 = _name.lastIndexOf("/");
		QString scenename =
				_name.mid(index2 + 1, _name.length() - index2).replace(" ", "");
		of << refname.toStdString() << "\t" << scenename.toStdString() << "\t";
		qDebug() << scenename;

		Image sceneImage(_name);
		ShapeHistogram sceneHist = sceneImage.getShapeHistogram();
		sceneHist.constructPGH(sceneImage.lineSegment(), angleAcc,
				refHist.getMaxDistance());

		double distance = getDistanceMetric(refHist, sceneHist, matchingMethod);
		of << distance << "\n";
		qDebug() << "Metric: " << QString::number(distance, 'f', 20);
		/*if(i == 9)
		 break;*/

	}
	of.close();
}

void GeometricHistogram::pairwiseHistogramDirectory(QString folderPath,
		LocalHistogram::AccuracyPGH angleAcc, int columns) {
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
		QString scenename = "test/pghmatrix/"
				+ _name.mid(index2 + 1, _name.length() - index2 - 5).replace(
						" ", "") + "_L"
				+ QString::number(LocalHistogram::heightOfAngleAxis(angleAcc))
				+ "_C" + QString::number(columns) + ".PGH";
		qDebug() << scenename;
		Image sceneImage(_name);
		vector<Line> lines = sceneImage.lineSegment();
		vector<LocalHistogram> listLocalPGH;
		ShapeHistogram sceneHist = sceneImage.getShapeHistogram();
		listLocalPGH = sceneHist.constructPGH(lines);
		sceneHist.constructMatPGH(angleAcc, columns);
		sceneHist.writeMatrix(scenename);
	}
}
void GeometricHistogram::copyFile(QString folderPath, QString savePath) {
	QDir qdir;
	qdir.setPath(folderPath);
	qdir.setFilter(QDir::Files);
	qdir.setNameFilters(QStringList("*.PGH"));
	QFileInfoList files = qdir.entryInfoList();
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		int index2 = _name.lastIndexOf("/");
		if (_name.contains("L1080_C1000")) {

			QString scenename = savePath
					+ _name.mid(index2 + 1, _name.length() - index2).replace(
							" ", "");
			QFile::copy(_name, scenename);
		}
	}
}

} /* namespace impls_2015 */
