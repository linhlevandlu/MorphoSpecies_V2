/*
 * Drawing.cpp
 *
 *  Created on: Aug 17, 2015
 *      Author: linh
 */

#include "Drawing.h"
#include <QtCore/qdebug.h>
#include <QtCore/QDebug>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>
#include <QtGui/qslider.h>
#include <QtCore/qset.h>
#include <QtCore/qlist.h>

namespace impls_2015 {

Drawing::Drawing() {

}
/**
 * Drawing the histogram of an image
 * @parameter: histogram of an image presented by a matrix
 */
void Drawing::drawHistogram(cv::Mat hImage){
	int hist_w = 512;
	int hist_h = 300;
	int bin_w = cvRound((double) hist_w / 256);
	cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::normalize(hImage, hImage, 0, histImage.rows, cv::NORM_MINMAX, -1,
			cv::Mat());
	for (int i = 1; i < 256; i++) {
		cv::line(histImage,
				cv::Point(bin_w * (i - 1),
						hist_h - cvRound(hImage.at<float>(i - 1))),
				cv::Point(bin_w * (i), hist_h - cvRound(hImage.at<float>(i))),
				cv::Scalar(255, 0, 0), 1, 8, 0);
	}
	cv::namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
	cv::imshow("Histogram", histImage);
}

/**
 * Drawing the step edges from a queue of points
 * @parameter 1: inputImage - the input image
 * @parameter 2: queuePoints - queue of points to draw
 * @return: an edges represented the points in queue points.
 */
cv::Mat Drawing::drawingEdges(cv::Mat inputImage, QQueue<cv::Point> queuePoints){
	qDebug() << "Drawing the edges from queue of points.";
	Mat output = Mat::zeros(inputImage.size(), CV_8UC3);
	cv::Point p;
	if(!queuePoints.isEmpty())
		p = queuePoints.dequeue();
	while (!queuePoints.isEmpty()) {
		cv::Point q = queuePoints.dequeue();
		cv::line(output, p, q, cv::Scalar(0, 255, 0), 1, 8);
		qDebug() << "(" << q.x << "," << q.y << ") ";
		p = q;
	}
	return output;
}

/**
 * Drawing the step edges from a list of lines
 * @parameter 1: inputImage - the input image
 * @parameter 2: listLines - list of lines to draw
 * @return: an edge was represented by a list of lines.
 */
cv::Mat Drawing::drawingEdges(cv::Mat inputImage, QList<QLine> listLines){
	qDebug() << "Drawing the edges from a list of lines.";
	Mat output = Mat::zeros(inputImage.size(), CV_8UC3);
	for(int i =0;i< listLines.size();i++){
		QLine linei = listLines[i];
		cv::line(output,cv::Point(linei.p1().x(),linei.p1().y()),cv::Point(linei.p2().x(),linei.p2().y()) , cv::Scalar(0, 255, 0), 1, 8);
	}
	return output;
}
} /* namespace impls_2015 */
