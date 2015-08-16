/*
 * Histogram.cpp
 *
 *  Created on: Aug 13, 2015
 *      Author: linh
 */

#include "HistogramImp.h"
#include <QtCore/QDebug>
#include <QtCore/qqueue.h>


namespace impls_2015 {

HistogramImp::HistogramImp() {
}

/**
 * Calculate the histogram of an input image.
 * @parameter: inputImage - the input image was presented by Mat
 * @return: an image contains the histogram of the input image,
 * 			it also presented by Mat
 */
cv::Mat HistogramImp::calcHistogram(cv::Mat inputImage){

	cv::Mat grayImage;
	if(inputImage.channels() == 3)
		cv::cvtColor(inputImage,grayImage,CV_BGR2GRAY);
	else
		inputImage.copyTo(grayImage);

	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true;
	bool accumulate = false;

	cv::Mat histImage;

	/// Compute the histograms:
	cv::calcHist(&grayImage, 1, 0, cv::Mat(), histImage, 1, &histSize,
			&histRange, uniform, accumulate);
	return histImage;
}

/**
 * Calculate the median of histogram of an image
 * @parameter: histogram of an image presented by a matrix
 * @return: the median value of the input histogram
 */
double HistogramImp::medianHistogram(cv::Mat hImage){
	double median = 0;
	float total = 0;
	for (int i = 0; i < 256; i++) {
		total += hImage.at<float>(i);
	}
	float avm = total / 2;
	total = 0;
	for (int i = 0; i < 256; i++) {
		total += hImage.at<float>(i);
		if (total >= avm) {
			median = i;
			break;
		}
	}
	return median;
}

/**
 * Calculate the mean of histogram of an image
 * @parameter: histogram of an image presented by a matrix
 * @return: the mean value of the input histogram
 */
float HistogramImp::meanHistogram(cv::Mat hImage){
	int total = 0;
	float pi = 0;
	for (int i = 0; i < 256; i++) {
		total += hImage.at<float>(i);
		pi += (i * hImage.at<float>(i));
	}

	return (pi / total);
}

/**
 * Drawing the histogram of an image
 * @parameter: histogram of an image presented by a matrix
 */
void HistogramImp::drawHistogram(cv::Mat hImage){
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
} /* namespace impls_2015 */
