/*
 * PGH.h
 *
 *  Created on: Sep 1, 2015
 *      Author: linh
 */

#ifndef PGH_H_
#define PGH_H_

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
#include "Image.h"
#include "IExtraction.h"
#include "EdgeSegmentation.h"

using namespace cv;
using namespace std;

namespace impls_2015 {

class ShapeHistogram: public IExtraction {
private:
	double totalEntries;
	vector<Line> lines;
	//vector<LocalHistogram> constructPGH();
	double max_distance;
	vector<vector<int> > matrix;
	int heightAngleAxis(int n);
	int binLocation(double angle, int n);
public:
	enum AccuracyPGH{
			Degree = 1,
			TwoTimeDegree = 2,
			FourTimeDegree = 4,
			SixTimeDegree = 6,
			TwelveTimeDegree = 12,
			SixtyTimeDegree = 60
		};
	//ShapeHistogram();
	ShapeHistogram(vector<Line> lines);
	virtual ~ShapeHistogram();
	double getTotalEntries();
	void setTotalEntries(double entries);
	double getMaxDistance();
	void setMaxDistance(double distance);
	void setLines(vector<Line> lines);
	void setMatrix(vector<vector<int> > matrix);
	vector<vector<int> > getMatrix();

	vector<LocalHistogram> constructPGH(AccuracyPGH angleAcc,double distanceAxis);
	//vector<LocalHistogram> shapePGH();
	//vector<vector<int> > savePGH(vector<LocalHistogram> pghHistograms,AccuracyPGH angleAcc);
	cv::Mat presentation(AccuracyPGH angleAcc);
	double bhattacharyaMetric(ShapeHistogram scenHist);
	double chiSquaredMetric(ShapeHistogram sceneHist);
	double intersectionMetric(ShapeHistogram sceneHist);


	void createTriangle();
	void createShape();

	vector<Edge> getEdges(Image image);
	QList<Landmark> getLandmarks();
};

} /* namespace impls_2015 */
#endif /* PGH_H_ */
