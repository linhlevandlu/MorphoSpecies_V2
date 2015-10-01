/*
 * HoughSpace.h
 *
 *  Created on: Sep 27, 2015
 *      Author: linh
 */

#ifndef HOUGHSPACE_H_
#define HOUGHSPACE_H_

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

#include "../Line.h"

using namespace std;
using namespace cv;

namespace impls_2015 {

class HoughSpace {
private:
	double distance;
	double angle;
public:
	HoughSpace();
	virtual ~HoughSpace();
	void setDistance(double distance);
	void setAngle(double angle);
	double getDistance();
	double getAngle();
	double computeDistance(Line objectLine, Point refPoint);
	double computeAngle(Line objectLine, Point refPoint);
	cv::Point closestPoint(Line objectLine, Point origin);
};

} /* namespace impls_2015 */
#endif /* HOUGHSPACE_H_ */
