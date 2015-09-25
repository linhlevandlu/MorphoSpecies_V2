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

#include "GFeatures.h"
#include "Line.h"

using namespace cv;
using namespace std;

namespace impls_2015 {

class PWHistogram {
private:
	Line refLine;
	vector<GFeatures> pwHistogram;
public:
	PWHistogram();
	virtual ~PWHistogram();
	void setLine(Line refLine);
	Line getLine();
	vector<GFeatures> getPWHistgoram();
	void setpwHistogram(vector<GFeatures> gfeatures);
	void addGFeatures(GFeatures features);
};

} /* namespace impls_2015 */
#endif /* PWHISTOGRAM_H_ */