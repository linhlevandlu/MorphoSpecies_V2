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
cv::Mat YellowGrid::removeYellowLines(cv::Mat matImage, int minBrightness,
		int synValue) {
	qDebug() << "Remove yellow lines action...";
	cv::Mat hsvImage;
	cv::Mat dest;
	vector<cv::Mat> bgr_planes;

	cv::cvtColor(matImage, hsvImage, cv::COLOR_BGR2HSV); // convert image to HSV
	cv::split(hsvImage, bgr_planes); // split the hsv image to process
	cv::Point bg = cv::Point(0, 0); // point to store the value to replace

	// find the point out of yellow grid (limited on the left hand side of image (cols/3 + 200))

	cv::Point limit_point = cv::Point(0, 0);
	int yellow_count = 0;
	int blue_count =0;
	for (int j = 10; j < bgr_planes[0].cols / 3 + 200; j++) {
		if (bgr_planes[0].at<uchar>(0, j) > 100) {
			limit_point.x = j;
			limit_point.y = 0;
			blue_count++;
			yellow_count = 0;
			for (int i = 1; i < bgr_planes[0].rows * 2 / 3; i++) {
				if (bgr_planes[0].at<uchar>(i, j) >= 0
						&& bgr_planes[0].at<uchar>(i, j) <= 40) {
					yellow_count++;
					if (yellow_count >= 10) {
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

	qDebug() << "Blue count: " << blue_count;
	qDebug() << "Yellow count: " << yellow_count;
	if (limit_point.x == 0) {
		limit_point.x = bgr_planes[0].cols / 3 + 200;
		limit_point.y = 0;
	}
	qDebug() << "Number of yellow point: " << yellow_count;
	qDebug() << "The limit point: " << limit_point.x << ", " << limit_point.y;

	// assign value to replace
	bg.x = bgr_planes[0].rows - limit_point.y - 5;
	bg.y = bgr_planes[0].cols - 30;
	qDebug() << "The replace point: " << bg.x << ", " << bg.y << ", value: ("
			<< bgr_planes[0].at<uchar>(bg.x, bg.y) << ","
			<< bgr_planes[1].at<uchar>(bg.x, bg.y) << ", "
			<< bgr_planes[2].at<uchar>(bg.x, bg.y) << ")";

	// replace the points
	for (int i = 0; i < bgr_planes[0].rows; i++) {
		for (int j = 0; j < limit_point.x; j++) {
			if (j > bgr_planes[0].cols / 4
					&& (bgr_planes[0].at<uchar>(i, j) >= 90
							&& bgr_planes[0].at<uchar>(i, j) <= 130)
					&& (bgr_planes[2].at<uchar>(i, j) >= 10
							&& bgr_planes[2].at<uchar>(i, j) <= 90)) {
				break;
			} else {
				// replace the yellow points
				if (bgr_planes[0].at<uchar>(i, j) <= 38
						&& (bgr_planes[2].at<uchar>(i, j) >= 10
								&& bgr_planes[2].at<uchar>(i, j) <= 100)) {
					bgr_planes[0].at<uchar>(i, j) = bgr_planes[0].at<uchar>(
							bg.x, bg.y);
					bgr_planes[1].at<uchar>(i, j) = bgr_planes[1].at<uchar>(
							bg.x, bg.y);
					bgr_planes[2].at<uchar>(i, j) = bgr_planes[2].at<uchar>(
							bg.x, bg.y);
				} else { // there are not yellow points
						 // replace the point what do not enough the bright (not in yellow range)
					if (bgr_planes[2].at<uchar>(i, j) > minBrightness
							&& bgr_planes[2].at<uchar>(i, j) <= 255) {
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
						bgr_planes2[0].rows - 20, bgr_planes2[0].cols - 20);
				bgr_planes2[1].at<uchar>(i, j) = bgr_planes2[1].at<uchar>(
						bgr_planes2[0].rows - 20, bgr_planes2[0].cols - 20);
				bgr_planes2[2].at<uchar>(i, j) = bgr_planes2[2].at<uchar>(
						bgr_planes2[0].rows - 20, bgr_planes2[0].cols - 20);
			}
		}
	}
	cv::merge(bgr_planes2, enddest);

	// draw some lines

	/*cv::line(enddest, cv::Point(enddest.cols / 2, 0),
	 cv::Point(enddest.cols / 2, enddest.rows), cv::Scalar(0, 0, 255), 5,
	 8);//1/4
	 cv::line(enddest, cv::Point(enddest.cols / 4, 0),
	 cv::Point(enddest.cols / 4, enddest.rows), cv::Scalar(0, 0, 255), 5,
	 8);//2/4
	 cv::line(enddest, cv::Point(enddest.cols * 3 / 4, 0),
	 cv::Point(enddest.cols * 3 / 4, enddest.rows), cv::Scalar(0, 255,0), 5,
	 8);//3/4*/
	/*cv::line(enddest, cv::Point(enddest.cols / 3, 0),
	 cv::Point(enddest.cols / 3, enddest.rows), cv::Scalar(0, 255, 255), 5,
	 8);//1/3
	 cv::line(enddest, cv::Point(2 * enddest.cols / 3, 0),
	 cv::Point(2 * enddest.cols / 3, enddest.rows), cv::Scalar(0, 255, 255), 5,
	 8);
*/

	cv::line(enddest, cv::Point(limit_point.x, 0),
			cv::Point(limit_point.x, enddest.rows), cv::Scalar(255, 255, 255),
			5, 8);

	return enddest;
}
cv::Mat YellowGrid::usingHistogram(cv::Mat input) {
	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };

	cv::Mat gray_img, hist_img;
	cv::cvtColor(input, gray_img, CV_BGR2GRAY);

	cv::calcHist(&gray_img, 1, 0, cv::Mat(), hist_img, 1, &histSize, &histRange,
			true, false);

	float nb_pixel = 0, nbs_temp = 0, avg = 0;
	// calculate the average value of histogram
	for (int i = 0; i < histSize; i++) {
		nbs_temp += (hist_img.at<uchar>(i) * i);
		nb_pixel += hist_img.at<uchar>(i);
	}
	avg = nbs_temp/nb_pixel;

	qDebug() << "average value" << avg;

	// replace the background by white color
	for (int i = 0; i < gray_img.rows; i++) {
		for (int j = 0; j < gray_img.cols; j++) {
			if (gray_img.at<uchar>(i, j) >= avg) { // background
				gray_img.at<uchar>(i, j) = 255;
			}
		}
	}
	cv::Mat hsv_img;
	vector<cv::Mat> hsv_planes;
	cv::cvtColor(gray_img,hsv_img,COLOR_GRAY2BGR);
	cv::cvtColor(hsv_img,hsv_img,COLOR_BGR2HSV);
	split(hsv_img,hsv_planes);
	for(int i = 0 ;i < hsv_planes[2].rows;i++){
		for(int j =0; j < hsv_planes[2].rows;j++){
			if(hsv_planes[2].at<uchar>(i,j) >= 110){
				hsv_planes[2].at<uchar>(i,j) = 255;
			}
		}
	}
	merge(hsv_planes,hsv_img);
	cv::Mat bgr_img;
	cv::cvtColor(hsv_img,bgr_img,COLOR_HSV2BGR);
	return bgr_img;
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
}
} /* namespace impls_2015 */
