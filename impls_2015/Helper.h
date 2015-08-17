/*
 * Helper.h
 *
 *  Created on: Aug 12, 2015
 *      Author: linh
 */

#ifndef HELPER_H_
#define HELPER_H_
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

class Helper {
public:
	Helper();

	static int getFirstIndexPoint(vector<cv::Point> listPoints,
			cv::Point point);
	static double distanceBetweenPoints(cv::Point point1, cv::Point point2);
	static bool checkPointInQueue(QQueue<cv::Point> queue, cv::Point point);

};

} /* namespace impls_2015 */
#endif /* HELPER_H_ */
