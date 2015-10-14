/*
 * PWHistogram.h
 *
 *  Created on: Aug 27, 2015
 *      Author: linh
 */

#ifndef PWHISTOGRAM_H_
#define PWHISTOGRAM_H_

#include <QtGui/QDialog>
#include <QtGui/QWidget>
#include <QtGui/QSlider>
#include <QtCore/qqueue.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

#include "GFeatures.h"
#include "Line.h"

using namespace cv;
using namespace std;

namespace impls_2015 {

class LocalHistogram {
public:
	enum AccuracyPGH {
		HaftDegree = 0,
		Degree = 1,
		TwoTimeDegree = 2,
		FourTimeDegree = 4,
		SixTimeDegree = 6,
		TwelveTimeDegree = 12,
		SixtyTimeDegree = 60
	};
private:
	vector<GFeatures> pwHistogram;
	static int convertAngleToMinute(double angle);
	double maxDistance;
public:
	LocalHistogram();
	virtual ~LocalHistogram();
	vector<GFeatures> getPWHistgoram();
	void setpwHistogram(vector<GFeatures> gfeatures);
	double getMaxDistance();
	void addGFeatures(GFeatures features);
	vector<vector<int> > constructorMatrix(AccuracyPGH angleAcc, int distanceAxis,int &totalEntries);
	double bhattacharyyaMetric(LocalHistogram sceneHistogram, int distanceAxis, AccuracyPGH angleAcc);

	static int accuracyToTimeDegree(double angle,
			AccuracyPGH angleAcc);
	static int heightOfAngleAxis(AccuracyPGH angleAcc);
	static int distanceOffset(double maxDistance, double distance, int cols);
};

} /* namespace impls_2015 */
#endif /* PWHISTOGRAM_H_ */
