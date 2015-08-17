/*
 * Lines.h
 *
 *  Created on: Jun 24, 2015
 *      Author: linh
 */

#ifndef LINES_H_
#define LINES_H_

#include <QtGui/QDialog>
#include <QtGui/QWidget>
#include <QtGui/QSlider>
#include <QtCore/qqueue.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
namespace impls_2015 {

class Lines {
public:
	Lines();
	static QList<QLine> getLines(QQueue<cv::Point> queuePoints);
};

} /* namespace impls_2015 */
#endif /* LINES_H_ */
