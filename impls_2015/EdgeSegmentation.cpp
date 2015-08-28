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
EdgeSegmentation::EdgeSegmentation() {
}

/**
 * Get the image
 * @return: the image
 */
/*Image EdgeSegmentation::getImage() {
 return this->image;
 }*/

/**
 * Set the image
 * @parameter: image - the input image
 */
/*void EdgeSegmentation::setImage(Image image) {
 this->image = image;
 }*/

/**
 * Get the edges of an image
 * @return: list of edge which construct the image
 */
vector<Edge> EdgeSegmentation::getEdges(Image inputImage) {
	cv::Mat grayImg;
	cv::cvtColor(inputImage.getMatImage(), grayImg, CV_BGR2GRAY);

	cv::Mat histImg = inputImage.histogram();
	// calculate the median
	double median = inputImage.medianHistogram(); //  impls_2015::HistogramImp::medianHistogram(histImg);
	float mean = inputImage.meanHistogram(); // impls_2015::HistogramImp::meanHistogram(histImg);
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
	findContours(cannyImage, contours, hierarchy, RETR_LIST,
			CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<Edge> edges;

	for (size_t i = 0; i < contours.size(); i++) {
		Edge edge(contours[i]);
		if (edge.getPoints().size() > 50) {
			vector<Edge> splEdge = edge.splitEdge(4);
			edges.insert(edges.end(), splEdge.begin(), splEdge.end());
		} else
			edges.push_back(edge);
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

vector<PWHistogram> EdgeSegmentation::constructPGHs(vector<Line> prLines) {

	vector<PWHistogram> pwh;
	for (size_t t = 0; t < prLines.size(); t++) {
		Line refLine = prLines.at(t);
		PWHistogram pwHistogram;
		pwHistogram.setLine(refLine);
		vector<GFeatures> pgh;
		for (size_t i = 0; i < prLines.size(); i++) {
			Line objLine = prLines.at(i);
			if (t != i) {
				GFeatures prH = refLine.pairwiseHistogram(objLine);
				pgh.push_back(prH);
			}
		}
		pwHistogram.setpwHistogram(pgh);
		pwh.push_back(pwHistogram);
	}
	return pwh;
}

void EdgeSegmentation::pgh() {
	cv::Point p1(1400, 800);
	cv::Point p2(2000, 600);
	cv::Point p3(2000, 200);
	cv::Point p4(1400, 0);
	cv::Point p5(0, 200);
	cv::Point p6(0, 600);

	Line l1(p1, p2);
	Line l2(p2, p3);
	Line l3(p3, p4);
	Line l4(p4, p5);
	Line l5(p5, p6);
	Line l6(p6, p1);
	Line ox(cv::Point(0, 0), p4);

	vector<Line> vtl;
	vtl.push_back(l1);
	vtl.push_back(l2);
	vtl.push_back(l3);
	vtl.push_back(l4);
	vtl.push_back(l5);
	vtl.push_back(l6);
	//cv::Point its = l2.intersection(l3);
	//qDebug() << "Intersection point: " <<its.x <<"," << its.y;
	vector<PWHistogram> pghs = this->constructPGHs(vtl);
	for (size_t t = 0; t < pghs.size(); t++) {
		PWHistogram pwh = pghs[t];
		qDebug() << "ref line: " << pwh.getLine() << ", pwh of line: ";

		int totalEntries = 0;
		for (size_t i = 0; i < pwh.getPWHistgoram().size(); i++) {
			GFeatures gfeature = pwh.getPWHistgoram().at(i);
			totalEntries += (gfeature.getDmax() - gfeature.getDmin());
		}
		totalEntries = totalEntries * pwh.getLine().length();
		qDebug() <<totalEntries;

		cv::Mat pwImage(cv::Size(totalEntries/1000, 648000/1000), CV_8UC3,
				cv::Scalar(0, 0, 0));

		for (size_t i = 0; i < pwh.getPWHistgoram().size(); i++) {
			GFeatures gfeature = pwh.getPWHistgoram().at(i);
			qDebug() << gfeature.getAngle() << " - " << gfeature.getDmin()
					<< " - " << gfeature.getDmax();

			int ycoor = (int) round(gfeature.getAngle() * 3600);
			cv::line(pwImage, cv::Point(gfeature.getDmin()/1000, ycoor/1000),
					cv::Point(gfeature.getDmax()/1000, ycoor/1000),
					cv::Scalar(0, 255, 255), 2, 8);
		}
		cv::namedWindow("ab" + t, CV_WINDOW_AUTOSIZE);
		cv::imshow("ab" + t, pwImage);
		pwImage.release();
	}
}
} /* namespace impls_2015 */
