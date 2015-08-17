/*
 * Classification.h
 *
 *  Created on: Aug 13, 2015
 *      Author: linh
 */

#ifndef CLASSIFICATION_H_
#define CLASSIFICATION_H_

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


using namespace cv;
using namespace std;

namespace impls_2015 {

class Classification {
public:
	Classification();
	static cv::Mat classification(cv::Mat inputImage);
	static QQueue<cv::Point> featuresExtraction(cv::Mat inputImage);
};

} /* namespace impls_2015 */
#endif /* CLASSIFICATION_H_ */
