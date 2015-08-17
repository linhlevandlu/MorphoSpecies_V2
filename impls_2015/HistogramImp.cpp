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


} /* namespace impls_2015 */
