/*
 * EdgeSegmentation.h
 *
 *  Created on: Aug 13, 2015
 *      Author: linh
 */

#ifndef EDGESEGMENTATION_H_
#define EDGESEGMENTATION_H_

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

#include "Helper.h"


using namespace cv;
using namespace std;

namespace impls_2015 {

class EdgeSegmentation {
public:
	static QQueue<cv::Point> queuePoints ;
	EdgeSegmentation();
	static vector<vector<cv::Point> > stepEdgesByFindContours(
			cv::Mat cannyImage);
	static vector<Vec4i> stepEdgesByHoughLines(cv::Mat cannyImage);
	static void edgeSegmentation(cv::Mat inputImage,vector<cv::Point> contours);
};

} /* namespace impls_2015 */
#endif /* EDGESEGMENTATION_H_ */
