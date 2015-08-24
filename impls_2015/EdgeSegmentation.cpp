/*
 * EdgeSegmentation.cpp
 *
 *  Created on: Aug 13, 2015
 *  Image processing for morphometrics (IPM) Version 2
 *	Copyright (C) 2015 LE Van Linh (linhlevandlu@gmail.com)
 *
 *	This program is free software: you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License as published by
 * 	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include "EdgeSegmentation.h"
#include <QtCore/QDebug>
#include <QtCore/qqueue.h>

namespace impls_2015 {

/**
 * Constructor an edgeSegmentation
 */
EdgeSegmentation::EdgeSegmentation(Image image) {
	this->image = image;
}

/**
 * Get the image
 * @return: the image
 */
Image EdgeSegmentation::getImage() {
	return this->image;
}

/**
 * Set the image
 * @parameter: image - the input image
 */
void EdgeSegmentation::setImage(Image image) {
	this->image = image;
}

/**
 * Get the edges of an image
 * @return: list of edge which construct the image
 */
QList<Edge> EdgeSegmentation::getEdges() {
	cv::Mat grayImg;
	cv::cvtColor(image.getMatImage(), grayImg, CV_BGR2GRAY);

	cv::Mat histImg = image.histogram();
	// calculate the median
	double median = image.medianHistogram(); //  impls_2015::HistogramImp::medianHistogram(histImg);
	float mean = image.meanHistogram(); // impls_2015::HistogramImp::meanHistogram(histImg);
	// find the first maximal point
	int limit = (mean > median ? median : mean);
	limit = (limit >= 120 ? (limit - 25) : (limit - 5));
	int imax = -1, max = -1;
	for (int i = 0; i < limit; i++) {
		if (histImg.at<float>(i) > max) {
			max = histImg.at<float>(i);
			imax = i;
		}
	}

	int limit2 = (mean > median ? mean : median);
	int imin = -1, min = max;
	for (int k = imax; k < limit2; k++) {
		if (histImg.at<float>(k) < min) {
			min = histImg.at<float>(k);
			imin = k;
		}
	}

	int max2 = -1, imax2 = -1;
	for (int j = limit2; j < 256; j++) {
		if (histImg.at<float>(j) > max2) {
			max2 = histImg.at<float>(j);
			imax2 = j;
		}
	}

	int mid1 = (imin + imax) / 2;
	int mid2 = (imin + imax2) / 2;
	int mid = (mid1 + mid2) / 2;
	cv::threshold(grayImg, grayImg, mid, 255, CV_THRESH_BINARY);

	Mat cannyImage;
	cv::Canny(grayImg, cannyImage, mid, 3 * mid, 5);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);
	findContours(cannyImage, contours, hierarchy, RETR_TREE,
			CHAIN_APPROX_SIMPLE, Point(0, 0));
	QList<Edge> edges;

	for (size_t i = 0; i < contours.size(); i++) {
		std::list<Point> list(contours[i].begin(), contours[i].end());
		QList<Point> points = QList<Point>::fromStdList(list);
		Edge edge(points);
		edges.append(edge);
	}
	return edges;
}

/**
 * Get the landmarks on image
 * @return: list of landmark on the image
 */
QList<Landmark> EdgeSegmentation::getLandmarks() {
	QList<Landmark> landmarks;
	return landmarks;
}
} /* namespace impls_2015 */
