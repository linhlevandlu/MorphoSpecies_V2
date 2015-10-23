/*
 * PGH.h
 *
 *  Created on: Sep 1, 2015
 *      Author: linh
 */

#ifndef SHAPEHISTOGRAM_H_
#define SHAPEHISTOGRAM_H_

#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include <QtGui/QDialog>
#include <QtGui/QWidget>
#include <QtGui/QSlider>
#include <QtCore/qqueue.h>
#include <QtCore/QDebug>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <time.h>

#include "IDrawOperation.h"
#include "GFeatures.h"
#include "LocalHistogram.h"
#include "Edge.h"
#include "Landmark.h"


using namespace cv;
using namespace std;

namespace impls_2015 {

class ShapeHistogram {

private:
	double totalEntries;
	double max_distance;
	vector<vector<int> > matrix;
	vector<LocalHistogram> listLocalHistogram;
	int heightAngleAxis(LocalHistogram::AccuracyPGH angleAcc);
	int angleOffset(double angle, LocalHistogram::AccuracyPGH angleAcc);
public:
	ShapeHistogram();
	//ShapeHistogram(vector<Line> lines);
	virtual ~ShapeHistogram();
	double getTotalEntries();
	void setTotalEntries(double entries);
	double getMaxDistance();
	void setMaxDistance(double distance);
	void setMatrix(vector<vector<int> > matrix);
	vector<vector<int> > getMatrix();
	vector<LocalHistogram> getListLocalHistogram();

	cv::Mat presentation(vector<LocalHistogram> pghHistograms, LocalHistogram::AccuracyPGH angleAcc, int cols);
	double bhattacharyaMetric(ShapeHistogram scenHist);
	double chiSquaredMetric(ShapeHistogram sceneHist);
	double intersectionMetric(ShapeHistogram sceneHist);

	vector<LocalHistogram> constructPGH(vector<Line> prLines);
	int distanceOffset(double distance, int cols);
	void constructMatPGH(LocalHistogram::AccuracyPGH angleAcc, int cols);
	void writeMatrix(QString fileName);

};

} /* namespace impls_2015 */
#endif /* PGH_H_ */
