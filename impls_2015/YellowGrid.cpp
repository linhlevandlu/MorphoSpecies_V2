/*
 * YellowGrid.cpp
 *
 *  Created on: Jul 9, 2015
 *      Author: linh
 */

#include "YellowGrid.h"
#include <QtCore/QDebug>
#include <QtCore/qqueue.h>
#include "../ImageConvert.h"
#include "../ImageViewer.h"
#include "Helper.h"
#include "HistogramImp.h"


namespace impls_2015 {

YellowGrid::YellowGrid() {
}

/**
 * Get the part name of image by file name
 * @parameter: path - the path of input file
 * @return: which part in insect (elytre, mandibule droite,
 * 			mandibule gauche, pronotum or tete)
 */
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

/**
 * Remove the yellow grid in the image
 * @paramter 1: matImage - the input image was presented by Mat
 * @parameter 2: minBrightness - the minimum brightness indicate
 * 				the point need to replace
 * @return: the image after remove the yellow grid (presented by Mat)
 */
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
		if (bgr_planes[0].at<uchar>(5, j) > 100 ||
				(bgr_planes[0].at<uchar>(5, j) > 70
						&& bgr_planes[0].at<uchar>(5, j) < 100
						&& bgr_planes[1].at<uchar>(5, j) < 10
						&& bgr_planes[2].at<uchar>(5, j) > 175)) {
			limit_point.x = j;
			limit_point.y = 0;
			yellow_count = 0;
			for (int i = 1; i < bgr_planes[0].rows * 2 / 3; i++) {
				if (bgr_planes[0].at<uchar>(i, j) <= 38) {
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
	//bg.x = bgr_planes[0].rows - limit_point.y - 5;
	//bg.y = bgr_planes[0].cols - 30;

	cv::Mat gray_img, hist_img;
	if (matImage.channels() == 3)
		cv::cvtColor(matImage, gray_img, CV_BGR2GRAY);
	else
		gray_img = matImage;

	hist_img = HistogramImp::calcHistogram(gray_img);
	float avg = HistogramImp::meanHistogram(hist_img);

	// find the replate point
	for (int i = 0; i < gray_img.rows; i++) {
		for (int j = 0; j < gray_img.cols; j++) {
			if (gray_img.at<uchar>(i, j) >= avg
					&& bgr_planes[0].at<uchar>(i, j) > 90
					&& bgr_planes[0].at<uchar>(i, j) < 130
					&& bgr_planes[1].at<uchar>(i, j) > 51
					&& bgr_planes[2].at<uchar>(i, j) > 215) { // background
				bg.x = i;
				bg.y = j;
				break;
			}
		}
		if (bg.x != 0)
			break;
	}

	YellowGrid::SpeciesType _type = YellowGrid::getType(pathImage);
	qDebug() << "Species type: " << _type;


	// replace the points
	for (int i = 0; i < bgr_planes[0].rows; i++) {
		for (int j = 0; j < limit_point.x; j++) {
			if(_type == ELYTRE){
				if (j > bgr_planes[0].cols / 4
						&& ((bgr_planes[0].at<uchar>(i, j + 50) >= 90
								&& bgr_planes[0].at<uchar>(i, j + 50) <= 130
								&& bgr_planes[2].at<uchar>(i, j + 50) >= 10
								&& bgr_planes[2].at<uchar>(i, j + 50) <= 120)
								|| (bgr_planes[0].at<uchar>(i, j + 50) >= 90
										&& bgr_planes[0].at<uchar>(i, j + 50)
												<= 130
										&& bgr_planes[1].at<uchar>(i, j + 50)
												>= 60
										&& bgr_planes[2].at<uchar>(i, j + 50)
												>= 10
										&& bgr_planes[2].at<uchar>(i, j + 50)
												<= 120)))
					break;
			}

			if(_type == MDROITE || _type == MGAUCHE){
				if ((bgr_planes[0].at<uchar>(i, j + 50) >= 39
						&& bgr_planes[0].at<uchar>(i, j + 50) <= 130
						&& bgr_planes[1].at<uchar>(i, j + 50) >= 39))
					break;
			}
			if(_type == PRONOTUM){
				if (j > bgr_planes[0].cols / 5
						&& ((bgr_planes[0].at<uchar>(i, j + 50) >= 40
								&& bgr_planes[0].at<uchar>(i, j + 50) <= 170
								&& bgr_planes[1].at<uchar>(i, j + 50) >= 30
								&& bgr_planes[1].at<uchar>(i, j + 50) <= 95
								&& bgr_planes[2].at<uchar>(i, j + 50) >= 35
								&& bgr_planes[2].at<uchar>(i, j + 50) <= 255
								)))
					break;
			}
			if(_type == TETE){
				if (j > bgr_planes[0].cols / 4
						&& ((bgr_planes[0].at<uchar>(i, j + 50) >= 39
								&& bgr_planes[0].at<uchar>(i, j + 50) <= 140
								&& bgr_planes[2].at<uchar>(i, j + 50) >= 10
								&& bgr_planes[2].at<uchar>(i, j + 50) <= 145)
						|| (bgr_planes[0].at<uchar>(i, j + 50) > 130
								&& bgr_planes[1].at<uchar>(i, j + 50) < 70
								&& bgr_planes[2].at<uchar>(i, j + 50) > 120)
						|| (bgr_planes[0].at<uchar>(i, j + 50) >=  3
								&& bgr_planes[0].at<uchar>(i, j + 50) <= 15
								&& bgr_planes[2].at<uchar>(i, j + 50) >= 120
								&& bgr_planes[2].at<uchar>(i, j + 50) <= 153)))
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
	dest.copyTo(enddest);

	// Making a mask
	/*cv::Mat mask(matImage.size(), CV_8UC1);
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

	// Morphological image
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

	// Convert the left-background (black) to right-background
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
	cv::merge(bgr_planes2, enddest);*/

	return enddest;
}

} /* namespace impls_2015 */





























