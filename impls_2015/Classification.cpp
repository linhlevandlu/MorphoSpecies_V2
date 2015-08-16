/*
 * Classification.cpp
 *
 *  Created on: Aug 13, 2015
 *      Author: linh
 */

#include "Classification.h"
#include "HistogramImp.h"
#include "EdgeSegmentation.h"

#include <QtCore/QDebug>
#include <QtCore/qqueue.h>

namespace impls_2015 {

Classification::Classification() {
	// TODO Auto-generated constructor stub

}

/**
 * Implementation the article about identification of the landmarks
 * @paramter: inputImage - the input image
 * @return: the image contains the landmarks
 */
cv::Mat Classification::classification(cv::Mat inputImage){

	cv::Mat rsImage,grayImage,temp1, temp2;
	cv::cvtColor(inputImage,grayImage,CV_BGR2GRAY);

	// DoG and feature extraction
	int noise = 91;
	cv::GaussianBlur(grayImage,temp1,cv::Size(noise,noise),0,0,BORDER_DEFAULT);
	cv::GaussianBlur(grayImage,temp2,cv::Size((noise * 2) + 1,(noise * 2) + 1),0,0,BORDER_DEFAULT);
	cv::subtract(temp1,temp2,rsImage);

	// edge detection
	cv::Mat cannyImage;
	cv::Canny(rsImage,cannyImage,1.5 * noise,3 * noise,5);


	return cannyImage;
}

/**
 * Get the step edges of image and representation by the set of lines
 * @parameter: inputImage - the input image.
 * @return: queue of points, which represented the step edges
 */
QQueue<cv::Point> Classification::featuresExtraction(cv::Mat inputImage){
	cv::Mat grayImg,bkImage;
	cv::cvtColor(inputImage,grayImg,CV_BGR2GRAY);
	grayImg.copyTo(bkImage);
	cv::Mat histImg = impls_2015::HistogramImp::calcHistogram(grayImg);

	// calculate the median
	double median = impls_2015::HistogramImp::medianHistogram(histImg);
	float mean = impls_2015::HistogramImp::meanHistogram(histImg);

	// find the first maximal point
	int limit = (mean > median ? median: mean);
	limit = (limit >=120 ? (limit - 25) : (limit - 5));
	int imax = -1, max = -1;
	for(int i =0; i < limit;i++){
		if (histImg.at<float>(i) > max) {
				max = histImg.at<float>(i);
				imax = i;
		}
	}

	int limit2 = (mean > median ? mean: median);
	int imin = -1, min = max;
	for(int k = imax; k < limit2;k++){
		if(histImg.at<float>(k) < min){
			min = histImg.at<float>(k);
			imin = k;
		}
	}

	int max2 = -1, imax2 = -1;
	for(int j = limit2; j < 256;j++){
		if(histImg.at<float>(j) > max2){
			max2 = histImg.at<float>(j);
			imax2 = j;
		}
	}

	int mid1 = (imin+imax)/2;
	int mid2 = (imin + imax2)/2;
	int mid = (mid1 + mid2)/2;

	qDebug() << mid1 <<" - " << mid2 <<" - "<<mid;
	cv::threshold(grayImg,grayImg,mid,255,CV_THRESH_BINARY);

	Mat canny_output;
	cv::Canny(grayImg,canny_output,mid,3*mid,5);

	// get contours and drawing it
	vector<vector<cv::Point> > contours = EdgeSegmentation::stepEdgesByFindContours(canny_output);
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	//RNG rng(12345);

	EdgeSegmentation::queuePoints.clear();
	for (size_t i = 0; i < contours.size(); i++) {
		/*Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));*/
		if(contours[i].size() > 2500){
			int nParts = 4; // number of parts, which we want to
			int splitSize =contours[i].size()/nParts;

			int indexSplit = 0;
			while(indexSplit < nParts){

				int begin = indexSplit * splitSize;
				int end = ((indexSplit + 1) * splitSize) - 1;
				if(indexSplit == nParts - 1 ){
					end = contours[i].size() -1;
				}
				cv::Point p0 = contours[i][begin];
				cv::Point pend = contours[i][end];
				//cv::line(drawing, p0, pend, cv::Scalar(0, 255, 255), 1, 8);

				//equation of line
				double m = (double) (pend.y - p0.y) / (double) (pend.x - p0.x);
				double a = m;
				double b = -1;
				double c = p0.y - (m * p0.x);
				double distance = 0;
				distance = (a * contours[i][begin + 1].x + b * contours[i][begin + 1].y + c)
						/ (sqrt(pow(a, 2) + pow(b, 2)));

				bool negative = false;
				if (distance > 0)
					negative = false;
				else
					negative = true;

				QQueue<int> cutPoints;

				for (int j = begin + 1; j < end; j++) {
					//cv::line(drawing, contours[i][j - 1], contours[i][j], cv::Scalar(0,255,0),
					//		1, 8);
					distance = (a * contours[i][j].x + b * contours[i][j].y + c)
							/ (sqrt(pow(a, 2) + pow(b, 2)));
					if ((distance > 0 && negative)
							|| (distance < 0 && !negative)) {
						//qDebug() << "cut point: " << j - 1 << ", x- y: "
						//		<< contours[i][j - 1].x << ", "
						//		<< contours[i][j - 1].y;
						cutPoints.enqueue(j - 1);
						if (distance < 0)
							negative = true;
						else
							negative = false;
					}
				}

				int k0 = begin;
				int k1 = 0;
				while (!cutPoints.isEmpty()) {
					k1 = cutPoints.dequeue();
					vector<cv::Point> edge(contours[i].begin() + k0,
							contours[i].begin() + k1);
					EdgeSegmentation::edgeSegmentation(drawing, edge);
					k0 = k1;
				}
				vector<cv::Point> edge(contours[i].begin() + k0,
						contours[i].begin() + end);
				EdgeSegmentation::edgeSegmentation(drawing, edge);
				indexSplit++;
			}
		}
	}
	return EdgeSegmentation::queuePoints;
}

/**
 * Drawing the step edges
 * @parameter 1: inputImage - the input image
 * @parameter 2: queuePoints - queue of points to draw
 * @return: an image represented the points in queue points.
 */
cv::Mat Classification::drawingEdges(cv::Mat inputImage, QQueue<cv::Point> queuePoints){
	Mat output = Mat::zeros(inputImage.size(), CV_8UC3);
	qDebug() << "Extract from queue...";
	cv::Point p = queuePoints.dequeue();
	while (!queuePoints.isEmpty()) {
		cv::Point q = queuePoints.dequeue();
		cv::line(output, p, q, cv::Scalar(0, 255, 0), 1, 8);
		qDebug() << "(" << q.x << "," << q.y << ") ";
		p = q;
	}
	return output;
}
} /* namespace impls_2015 */
