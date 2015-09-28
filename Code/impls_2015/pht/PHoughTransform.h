/*
 * PHoughTransform.h
 *
 *  Created on: Sep 23, 2015
 *      Author: linh
 */

#ifndef PHOUGHTRANSFORM_H_
#define PHOUGHTRANSFORM_H_

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

class PHoughTransform {
private:
	int image_width;
	int image_height;
	vector<vector<int> > accumulator;
public:
	PHoughTransform();
	virtual ~PHoughTransform();
	static cv::Mat pHoughTransform(cv::Mat inputImage);



	vector<vector<int> > createAccumulator(vector<Line> lines, int width,int height);
	int maxOfAccumulator(vector<vector<int> > accumulator);
	vector<Line> drawAccumulator(vector<vector<int> > accumulator, int threshold);
};

} /* namespace impls_2015 */
#endif /* PHOUGHTRANSFORM_H_ */
