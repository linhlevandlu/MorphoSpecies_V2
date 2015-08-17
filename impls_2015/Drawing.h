/*
 * Drawing.h
 *
 *  Created on: Aug 17, 2015
 *      Author: linh
 */

#ifndef DRAWING_H_
#define DRAWING_H_
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

using namespace std;
using namespace cv;

namespace impls_2015 {

class Drawing {
public:
	Drawing();
	static void drawHistogram(cv::Mat hImage);
	static cv::Mat drawingEdges(cv::Mat inputImage, QQueue<cv::Point> queuePoints);
	static cv::Mat drawingEdges(cv::Mat inputImage, QList<QLine> listLines);
};

} /* namespace impls_2015 */
#endif /* DRAWING_H_ */
