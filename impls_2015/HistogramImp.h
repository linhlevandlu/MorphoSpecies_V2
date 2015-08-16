/*
 * Histogram.h
 *
 *  Created on: Aug 13, 2015
 *      Author: linh
 */

#ifndef HISTOGRAMIMP_H_
#define HISTOGRAMIMP_H_
#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include <QtCore/qqueue.h>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <QtCore/QDebug>
#include <QtCore/qqueue.h>

using namespace cv;
using namespace std;

namespace impls_2015 {

class HistogramImp {
public:
	HistogramImp();
	static cv::Mat calcHistogram(cv::Mat inputImage);
	static double medianHistogram(cv::Mat hImage);
	static float meanHistogram(cv::Mat hImage);
	static void drawHistogram(cv::Mat hImage);
};

} /* namespace impls_2015 */
#endif /* HISTOGRAM_H_ */
