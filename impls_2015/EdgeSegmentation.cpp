/*
 * EdgeSegmentation.cpp
 *
 *  Created on: Aug 13, 2015
 *      Author: linh
 */

#include "EdgeSegmentation.h"
#include <QtCore/QDebug>
#include <QtCore/qqueue.h>



namespace impls_2015 {
QQueue<cv::Point> EdgeSegmentation::queuePoints;
EdgeSegmentation::EdgeSegmentation() {
}

/**
 * Get the step edges using the findContours function
 * @parameter: cannyImage - the image after apply the Canny algorithm
 * @return: list of the step edges, which was presented by the list of Point
 */
vector<vector<cv::Point> > EdgeSegmentation::stepEdgesByFindContours(cv::Mat cannyImage){

	// using findContours to obtain the step edges
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);
	findContours(cannyImage, contours, hierarchy, RETR_TREE,
			CHAIN_APPROX_SIMPLE, Point(0, 0));

	// drawing the contours
	Mat contourImg = Mat::zeros(cannyImage.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));
		if (contours[i].size() > 10) {
			for (int j = 0; j < contours[i].size(); j++) {
				//qDebug() << contours[i][j].x << "x" << contours[i][j].y << " ";
				cv::drawContours(contourImg, contours, i, color, 1, 8, hierarchy, 0,
						Point());
			}
		}
	}
	//cv::namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	//cv::imshow("Contours", contourImg);
	return contours;
}

/**
 * Get the step edges using the probabilistic Hough transform algorithm for line detection
 * @parameter: cannyImage - the image after apply the Canny algorithm
 * @return: list of lines, which was presented by a vector for begin and end point of line
 */
vector<Vec4i> EdgeSegmentation::stepEdgesByHoughLines(cv::Mat cannyImage){

	// using HoughLinesP to obtain the step edges
	vector<Vec4i> lines;
	cv::HoughLinesP(cannyImage, lines, 1, CV_PI / 180, 5, 5, 10);

	//drawing the lines
	Mat hLinesImg = Mat::zeros(cannyImage.size(), CV_8UC3);
	for (size_t i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];
		cv::line(hLinesImg, Point(l[0], l[1]), Point(l[2], l[3]),
				Scalar(0, 255, 0), 1, 8);
	}
	cv::namedWindow("HoughLinesP", CV_WINDOW_AUTOSIZE);
	cv::imshow("HoughLinesP", hLinesImg);
	return lines;
}

/**
 * Representation the edge by the connected lines
 * @parameter contours: set of points on the edge
 * @return:
 */
void EdgeSegmentation::edgeSegmentation(cv::Mat inputImage,vector<cv::Point> contours){

	cv::Point p0 = contours[0];
	cv::Point pend = contours[contours.size() - 1];
	double length = sqrt(pow(pend.x - p0.x,2) + pow(pend.y - p0.y,2));


	//cv::line(result,p0,pend,cv::Scalar(0,0,255),1,8);
	double m  = (double)(pend.y - p0.y)/(double)(pend.x-p0.x);
	double a = m;
	double b = -1;
	double c = p0.y - (m * p0.x);

	double distance  = 0;
	double maxDistance  = 0;
	double imax =-1;
	for(int i = 0;i < contours.size();i++){
		cv::Point pointi = contours[i];
		distance = abs(a * pointi.x + b * pointi.y + c)/(sqrt(pow(a,2) +  pow(b,2)));
		if(distance > maxDistance){
			maxDistance = distance;
			imax = i;
		}
	}

	double lamda = maxDistance/length;
	double l1 = sqrt(pow(contours[imax].x - p0.x,2) + pow(contours[imax].y - p0.y,2));
	double ratio = (maxDistance/l1);
	if(ratio > lamda){
		vector<cv::Point> part1(contours.begin(),contours.begin()+imax + 1);
		vector<cv::Point> part2(contours.begin() + imax, contours.end());
		edgeSegmentation(inputImage,part1);
		edgeSegmentation(inputImage,part2);
	}
	RNG rng(12345);
	if(imax != -1){
		//cv::circle(inputImage,contours[imax],1,cv::Scalar(0,0,255),1,8);
		if(!Helper::checkPointInQueue(queuePoints,p0))
			queuePoints.enqueue(p0);
		if(!Helper::checkPointInQueue(queuePoints,contours[imax]))
			queuePoints.enqueue(contours[imax]);
		if(!Helper::checkPointInQueue(queuePoints,pend))
			queuePoints.enqueue(pend);
		//qDebug() << imax <<" - ("<<p0.x<<","<<p0.y<<") - ("<<pend.x<<","<<pend.y <<")" ;
	}
	// the output is the queue of points
}
} /* namespace impls_2015 */
