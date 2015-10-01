/*
 * GeometricHistogram.h
 *
 *  Created on: Sep 17, 2015
 *      Author: linh
 */

#ifndef GEOMETRICHISTOGRAM_H_
#define GEOMETRICHISTOGRAM_H_
#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include <QtCore/qlist.h>
#include <QtGui/QFileDialog>
#include <QtCore/qdir.h>
#include <QtCore/qstringlist.h>
#include <iostream>

#include "../Image.h"
#include "../Line.h"
#include "HistogramMethod.h"

using namespace std;
using namespace cv;

namespace impls_2015 {

class GeometricHistogram: public HistogramMethod {
public:
	enum MatchingMethod {
		Bhattacharyya, Chisquared, Intersection
	};
private:
	double getDistanceMetric(ShapeHistogram refHist, ShapeHistogram sceneHist,
			MatchingMethod matching);
public:
	GeometricHistogram();
	virtual ~GeometricHistogram();
	vector<LocalHistogram> shapeHistogram(Image image,
			LocalHistogram::AccuracyPGH angleAcc, int columns, cv::Mat &result);
	//cv::Mat representShapeHistogram(ShapeHistogram shapeHistogram, ShapeHistogram::AccuracyPGH angleAcc);
	double pghHistogramMatching(Image refImage, Image sceneImage,
			MatchingMethod matchingMethod,
			LocalHistogram::AccuracyPGH angleAcc);
	void pghHistogramDirectoryMatching(Image refImage, QString folderPath,
			MatchingMethod matchingMethod,
			LocalHistogram::AccuracyPGH angleAcc);
	void pairwiseHistogramDirectory(QString folderPath,
			LocalHistogram::AccuracyPGH angleAcc, int columns);

	void copyFile(QString folderPath, QString savePath);

};

} /* namespace impls_2015 */
#endif /* GEOMETRICHISTOGRAM_H_ */
