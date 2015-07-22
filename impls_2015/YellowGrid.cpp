/*
 * YellowGrid.cpp
 *
 *  Created on: Jul 9, 2015
 *      Author: linh
 */

#include "YellowGrid.h"
#include <QtCore/QDebug>
#include "../ImageConvert.h"
#include "../ImageViewer.h"

namespace impls_2015 {

YellowGrid::YellowGrid() {

}
YellowGrid::SpeciesType YellowGrid::getType(QString path){
	YellowGrid::SpeciesType sType;
	QString temp = path.toLower();
	if(temp.contains("ely"))
		sType = ELYTRE;
	else{
		if(temp.contains("md"))
			sType = MDROITE;
		else{
			if (temp.contains("mg"))
				sType = MGAUCHE;
			else {
				if (temp.contains("prono"))
					sType = PRONOTUM;
				else {
					if (temp.contains("tete"))
						sType = TETE;
				}
			}
		}
	}
	return sType;
}
cv::Mat YellowGrid::removeYellowLines(cv::Mat matImage, int minBrightness,
		QString pathImage) {
	qDebug() << "Remove yellow lines action...";
	cv::Mat hsvImage;
	cv::Mat dest;
	vector<cv::Mat> bgr_planes;

	cv::cvtColor(matImage, hsvImage, cv::COLOR_BGR2HSV); // convert image to HSV
	cv::split(hsvImage, bgr_planes); // split the hsv image to process
	cv::Point bg = cv::Point(0, 0); // point to store the value to replace

	// find the point out of yellow grid (limited on the left hand side of image)

	cv::Point limit_point = cv::Point(0, 0);
	int yellow_count = 0;
	for (int j = 10; j < bgr_planes[0].cols; j++) {
		if (bgr_planes[0].at<uchar>(0, j) > 100 ||
				(bgr_planes[0].at<uchar>(0, j) > 70
						&& bgr_planes[1].at<uchar>(0, j) < 10
						&& bgr_planes[2].at<uchar>(0, j) > 175)) {
			limit_point.x = j;
			limit_point.y = 0;
			yellow_count = 0;
			for (int i = 1; i < bgr_planes[0].rows * 2 / 3; i++) {
				if (bgr_planes[0].at<uchar>(i, j) <= 40) {
					yellow_count++;
					if (yellow_count >= 8) {
						limit_point.x = 0;
						limit_point.y = 0;
						break;
					}
				}
			}
			if (limit_point.x != 0)
				break;
		}
	}

	if (limit_point.x == 0) {
		limit_point.x = bgr_planes[0].cols / 3 + 200;
		limit_point.y = 0;
	}

	// assign value to replace
	bg.x = bgr_planes[0].rows - limit_point.y - 5;
	bg.y = bgr_planes[0].cols - 30;

	YellowGrid::SpeciesType _type = YellowGrid::getType(pathImage);
	qDebug() << "Species type: " << _type;


	// replace the points
	for (int i = 0; i < bgr_planes[0].rows; i++) {
		for (int j = 0; j < limit_point.x; j++) {
			if(_type == ELYTRE){
				if (j > bgr_planes[0].cols / 4
						&& (bgr_planes[0].at<uchar>(i, j + 50) >= 90
								&& bgr_planes[0].at<uchar>(i, j + 50) <= 130
								&& bgr_planes[2].at<uchar>(i, j + 50) >= 10
								&& bgr_planes[2].at<uchar>(i, j + 50) <= 105))
					break;
			}
			if(_type == MDROITE || _type == MGAUCHE){
				if ((bgr_planes[0].at<uchar>(i, j + 50) >= 120
						&& bgr_planes[0].at<uchar>(i, j + 50) <= 160
						&& bgr_planes[2].at<uchar>(i, j + 50) <= 150)
						|| (bgr_planes[0].at<uchar>(i, j + 50) > 160
								&& bgr_planes[2].at<uchar>(i, j + 50) <= 160))
					break;
			}
			if(_type == PRONOTUM){
				if (j > bgr_planes[0].cols / 5
						&& ((bgr_planes[0].at<uchar>(i, j + 50) >= 40
								&& bgr_planes[0].at<uchar>(i, j + 50) <= 130
								&& bgr_planes[2].at<uchar>(i, j + 50) <= 160)
								|| (bgr_planes[0].at<uchar>(i, j + 50) > 130
										&& bgr_planes[2].at<uchar>(i, j + 50)
												>= 60)))
					break;
			}
			if(_type == TETE){
				if (j > bgr_planes[0].cols / 4
						&& ((bgr_planes[0].at<uchar>(i, j + 50) >= 90
								&& bgr_planes[0].at<uchar>(i, j + 50) <= 130
								&& bgr_planes[2].at<uchar>(i, j + 50) >= 10
								&& bgr_planes[2].at<uchar>(i, j + 50) <= 105)
								|| (bgr_planes[0].at<uchar>(i, j + 50) > 130)
								|| (bgr_planes[0].at<uchar>(i, j + 50) <= 15
										&& bgr_planes[2].at<uchar>(i, j + 50)
												>= 100
										&& bgr_planes[2].at<uchar>(i, j + 50)
												<= 160)))
					break;
			}
			// replace the points
			if (bgr_planes[0].at<uchar>(i, j) <= 38
					&& (bgr_planes[2].at<uchar>(i, j) >= 10
							&& bgr_planes[2].at<uchar>(i, j) <= 100)) {
				bgr_planes[0].at<uchar>(i, j) = bgr_planes[0].at<uchar>(bg.x,
						bg.y);
				bgr_planes[1].at<uchar>(i, j) = bgr_planes[1].at<uchar>(bg.x,
						bg.y);
				bgr_planes[2].at<uchar>(i, j) = bgr_planes[2].at<uchar>(bg.x,
						bg.y);
			} else { // there are not yellow points
					 // replace the point what do not enough the bright (not in yellow range)
				if (bgr_planes[2].at<uchar>(i, j) > minBrightness) {
					bgr_planes[0].at<uchar>(i, j) = bgr_planes[0].at<uchar>(
							bg.x, bg.y);
					bgr_planes[1].at<uchar>(i, j) = bgr_planes[1].at<uchar>(
							bg.x, bg.y);
					bgr_planes[2].at<uchar>(i, j) = bgr_planes[2].at<uchar>(
							bg.x, bg.y);
				}
			}
		}
	}

	// merge bgr_planes to dest image
	cv::merge(bgr_planes, dest);
	cv::cvtColor(dest, dest, cv::COLOR_HSV2BGR);
	cv::Mat enddest;

	// Masking V2
	cv::Mat mask(matImage.size(), CV_8UC1);
	for (int i = 0; i < matImage.rows; i++) {
		for (int j = 0; j < matImage.cols; j++) {
			if (matImage.at<cv::Vec3b>(i, j)[0] != dest.at<cv::Vec3b>(i, j)[0]
					&& matImage.at<cv::Vec3b>(i, j)[1]
							!= dest.at<cv::Vec3b>(i, j)[1]
					&& matImage.at<cv::Vec3b>(i, j)[2]
							!= dest.at<cv::Vec3b>(i, j)[2]) {
				mask.at<uchar>(i, j) = 0;
			} else {
				mask.at<uchar>(i, j) = 255;
			}
		}
	}

	int erosion_size = 6;
	cv::Mat element = getStructuringElement(cv::MORPH_CROSS,
			cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
			cv::Point(-1, -1));
	cv::erode(mask, mask, element);

	// mask with the source image
	vector<cv::Mat> splits(3), rs(3);
	cv::split(matImage, splits);
	cv::bitwise_and(splits[0], mask, rs[0]);
	cv::bitwise_and(splits[1], mask, rs[1]);
	cv::bitwise_and(splits[2], mask, rs[2]);

	cv::merge(rs, enddest);

	cv::vector<cv::Mat> bgr_planes2;
	cv::split(enddest, bgr_planes2);
	// chuyen phan den thanh xanh
	for (int i = 0; i < bgr_planes2[0].rows; i++) {
		for (int j = 0; j < bgr_planes2[0].cols; j++) {
			if (bgr_planes2[0].at<uchar>(i, j) == 0
					&& bgr_planes2[1].at<uchar>(i, j) == 0
					&& bgr_planes2[2].at<uchar>(i, j) == 0) {
				bgr_planes2[0].at<uchar>(i, j) = bgr_planes2[0].at<uchar>(
						20, bgr_planes2[0].cols - 20);//bgr_planes2[0].rows - 20
				bgr_planes2[1].at<uchar>(i, j) = bgr_planes2[1].at<uchar>(
						20, bgr_planes2[0].cols - 20);//bgr_planes2[1].rows - 20
				bgr_planes2[2].at<uchar>(i, j) = bgr_planes2[2].at<uchar>(
						20, bgr_planes2[0].cols - 20);//bgr_planes2[2].rows - 20
			}
		}
	}
	cv::merge(bgr_planes2, enddest);

	// draw some lines

	 /*cv::line(enddest, cv::Point(enddest.cols / 2, 0),
	 cv::Point(enddest.cols / 2, enddest.rows), cv::Scalar(0, 255, 255), 5,
	 8);//2/4
	 cv::line(enddest, cv::Point(0, enddest.rows/2),
	 cv::Point(enddest.cols, enddest.rows/2), cv::Scalar(0, 255, 255), 5,
	 8);

	cv::line(enddest, cv::Point(limit_point.x, 0),
			cv::Point(limit_point.x, enddest.rows), cv::Scalar(255, 255, 255),
			5, 8);
	cv::line(enddest, cv::Point(limit_point.x, 0),
				cv::Point(limit_point.x, enddest.rows), cv::Scalar(0, 0, 255),
				1, 8);*/

	return enddest;
}
/*cv::Mat YellowGrid::usingHistogram(cv::Mat input) {
	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };


	cv::Mat gray_img, hist_img;
	if(input.channels() == 3)
		cv::cvtColor(input, gray_img, CV_BGR2GRAY);
	else
		gray_img = input;

	cv::calcHist(&gray_img, 1, 0, cv::Mat(), hist_img, 1, &histSize, &histRange,
			true, false);

	float nb_pixel = 0, nbs_temp = 0, avg = 0;
	// calculate the average value of histogram
	for (int i = 0; i < histSize; i++) {
		nbs_temp += (hist_img.at<uchar>(i) * i);
		nb_pixel += hist_img.at<uchar>(i);
	}
	avg = nbs_temp/nb_pixel;
	//avg = otSu(hist_img);
	qDebug() << "average value" << avg;

	// replace the background by white color
	for (int i = 0; i < gray_img.rows; i++) {
		for (int j = 0; j < gray_img.cols; j++) {
			if (gray_img.at<uchar>(i, j) >= avg) { // background
				gray_img.at<uchar>(i, j) = 255;
			}
		}
	}

	return gray_img;
}

float YellowGrid::otSu(cv::Mat histogram) {
	int sum = 0;
	for (int i = 1; i < 256; ++i) {
		sum += i * histogram.at<uchar>(i);
	}
	int sumB, wB, wF, mB, mF;
	float max = 0, between = 0, thr1 = 0, thr2 = 0;
	for (int i = 0; i < 256; i++) {
		wB += histogram.at<uchar>(i);
		if (wB == 0)
			continue;
		wF = (histogram.cols * histogram.rows) - wB;
		if (wF == 0)
			break;
		sumB += i * histogram.at<uchar>(i);
		mB = sumB / wB;
		mF = (sum - sumB) / wF;
		between = wB * wF * (mB - mF) * (mB - mF);
		if (between >= max) {
			thr1 = i;
			if (between > max) {
				thr2 = i;
			}
			max = between;
		}
	}
	return (thr1 + thr2) / 2;
}*/
cv::Mat YellowGrid::landmarkIndentify(cv::Mat inputImage){

	cv::Mat rsImage,grayImage,temp1, temp2;
	cv::cvtColor(inputImage,grayImage,CV_BGR2GRAY);

	// DoG and feature extraction
	cv::GaussianBlur(grayImage,temp1,cv::Size(1,1),0,0,BORDER_DEFAULT);
	cv::GaussianBlur(grayImage,temp2,cv::Size(51,51),0,0,BORDER_DEFAULT);
	cv::subtract(temp2,temp1,rsImage);
	cv::bitwise_not(rsImage,rsImage);
	cv::threshold(rsImage,rsImage,230,255,THRESH_BINARY); // 235 -


	// edge detection
	cv::Mat cannyImage;
	int lowThreshold = 10;
	//cv::GaussianBlur(rsImage,rsImage,cv::Size(3,3),0,0,BORDER_DEFAULT);
	cv::Canny(rsImage,cannyImage,lowThreshold,2*lowThreshold,3);

	vector<vector<Point> > contours;
	  vector<Vec4i> hierarchy;
	cv::findContours(cannyImage, contours, hierarchy, CV_RETR_TREE,
			CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Draw contours
	Mat drawing = Mat::zeros(cannyImage.size(), CV_8UC3);
	for(size_t i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(255,255,255);
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}

	return drawing;
}
} /* namespace impls_2015 */





























