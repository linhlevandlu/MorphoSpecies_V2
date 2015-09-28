/*
 * PHTMethod.h
 *
 *  Created on: Sep 28, 2015
 *      Author: linh
 */

#ifndef HOUGHMETHOD_H_
#define HOUGHMETHOD_H_

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

#include "../Image.h"


using namespace std;
using namespace cv;


namespace impls_2015 {

class HoughMethod {
public:
	HoughMethod();
	virtual ~HoughMethod();
	virtual vector<vector<int> > accumulator(Image image) = 0;
};

} /* namespace impls_2015 */
#endif /* PHTMETHOD_H_ */
