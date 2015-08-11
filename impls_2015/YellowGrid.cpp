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

namespace impls_2015 {
	QQueue<cv::Point> YellowGrid::queuePoints;
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
	int histSize = 256;
		float range[] = { 0, 256 };
		const float* histRange = { range };


		cv::Mat gray_img, hist_img;
		if(matImage.channels() == 3)
			cv::cvtColor(matImage, gray_img, CV_BGR2GRAY);
		else
			gray_img = matImage;
		cv::calcHist(&gray_img, 1, 0, cv::Mat(), hist_img, 1, &histSize, &histRange,true, false);

		float nb_pixel = 0, nbs_temp = 0, avg = 0;
		// calculate the average value of histogram
		for (int i = 0; i < histSize; i++) {
			nbs_temp += (hist_img.at<uchar>(i) * i);
			nb_pixel += hist_img.at<uchar>(i);
		}
		avg = nbs_temp/nb_pixel;
		qDebug() << "average value" << avg;

		// find the replate point
		for (int i = 0; i < gray_img.rows; i++) {
			for (int j = 0; j < gray_img.cols; j++) {
				if (gray_img.at<uchar>(i, j) >= avg
						&& bgr_planes[0].at<uchar>(i, j) > 90
					&& bgr_planes[0].at<uchar>(i, j) < 130
					&& bgr_planes[1].at<uchar>(i, j) > 51
					&& bgr_planes[2].at<uchar>(i, j) > 215) { // background
					bg.x = i;
					bg.y =j;
					break;
				}
			}
			if(bg.x != 0)
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

	cv::line(enddest,cv::Point(limit_point.x,0),cv::Point(limit_point.x,enddest.rows),cv::Scalar(0,255,255),1,8);
	return enddest;
}
cv::Mat YellowGrid::act2(cv::Mat inputImage){

	cv::Mat hsvImg, grayImg,resultImg;
	vector<cv::Mat> hsv_images;
	cv::cvtColor(inputImage,hsvImg,CV_BGR2HSV);
	cv::split(hsvImg,hsv_images);
	/*cv::Mat histImage = calcHistogram(inputImage);
	double median = 0;
	int total = 0;
	for(int i=0;i<256;i++){
		total += histImage.at<float>(i);
	}
	float avm = total/2;
	total =0;
	for (int i = 0; i < 256; i++) {
		total += histImage.at<float>(i);
		if(total >= avm){
			median = i;
			break;
		}
	}
	qDebug() << median;*/

	int max = 0;
	int imax = -1, jmax = -1;
	int yCount = 0;
	for(int i = 0 ; i < hsv_images[0].rows/2;i++){
		yCount = 0;
		for(int j = 10; j < 600; j++){
			if(hsv_images[0].at<uchar>(i,j) <= 38)
				yCount++;
		}
		if(yCount > max){
			max = yCount;
			imax = i;
		}
	}
	int bCount = 0;
	bool cont = false;
	for(int j = 600; j < hsv_images[0].cols/2;j++ ){
		if(hsv_images[0].at<uchar>(imax,j) >= 90
				&& hsv_images[0].at<uchar>(imax,j) <=130){
			if(cont)
				bCount++;
			else{
				cont = true;
				bCount = 1;
			}
		}else
			cont = false;

		if(bCount == 15){
			jmax = j;
			break;
		}
	}
	cv::Scalar cv1(39,0,0);
	cv::Scalar cv2(179,255,255);
	cv::inRange(hsvImg,cv1,cv2,resultImg);
	cv::GaussianBlur(resultImg, resultImg, Size(11,11), 0);
	Mat kernel = (Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,0);
	cv::erode(resultImg, resultImg, kernel);
	//cv::line(hsv_images[2],cv::Point(0,hsv_images[2].rows - 100),cv::Point(hsv_images[2].cols,hsv_images[2].rows - 100),cv::Scalar(0),1,8);
	//cv::line(hsv_images[2],cv::Point(hsv_images[2].cols/3,0),cv::Point(hsv_images[2].cols/3,hsv_images[2].rows),cv::Scalar(0),1,8);
	/*cv::merge(hsv_images,resultImg);
	cv::cvtColor(resultImg,resultImg,CV_HSV2BGR);
	cv::line(resultImg,cv::Point(0,imax),cv::Point(resultImg.cols,imax),cv::Scalar(0,0,255),3,8);
	cv::line(resultImg,cv::Point(0,resultImg.rows/2),cv::Point(resultImg.cols,resultImg.rows/2),cv::Scalar(0,255,255),3,8);
	cv::line(resultImg,cv::Point(jmax,0),cv::Point(jmax,resultImg.rows),cv::Scalar(0),3,8);*/
	return resultImg;
}

cv::Mat YellowGrid::calcHistogram(cv::Mat inputImage){

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

cv::Mat YellowGrid::histogram(cv::Mat inputImage){
	cv::Mat grayImg,bkImage;
	cv::cvtColor(inputImage,grayImg,CV_BGR2GRAY);
	grayImg.copyTo(bkImage);
	cv::Mat histImg = calcHistogram(grayImg);

	// calculate the median
	double median = 0;
	int total = 0;
	float pi = 0;
	for (int i = 0; i < 256; i++) {
		total += histImg.at<float>(i);
		pi += (i * histImg.at<float>(i));
	}
	float avm = total / 2;
	float mean = pi/total;
	total = 0;
	for (int i = 0; i < 256; i++) {
		total += histImg.at<float>(i);
		if (total >= avm) {
			median = i;
			break;
		}
	}

	qDebug() << median <<"- " << (int)mean;
	// Draw the histogram
	int hist_w = 512;
	int hist_h = 300;
	int bin_w = cvRound((double) hist_w / 256);
	cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::normalize(histImg, histImg, 0, histImage.rows, cv::NORM_MINMAX, -1,
			cv::Mat());
	for (int i = 1; i < 256; i++) {
		cv::line(histImage,
				cv::Point(bin_w * (i - 1),
						hist_h - cvRound(histImg.at<float>(i - 1))),
				cv::Point(bin_w * (i), hist_h - cvRound(histImg.at<float>(i))),
				cv::Scalar(255, 0, 0), 1, 8, 0);
	}
	/*cv::line(histImage,
					cv::Point(bin_w * 100,0),
					cv::Point(bin_w * 100, histImage.rows),
					cv::Scalar(0, 0, 255), 1, 8, 0);
	cv::line(histImage,
						cv::Point(bin_w * median,0),
						cv::Point(bin_w * median, histImage.rows),
						cv::Scalar(0, 255, 255), 1, 8, 0);
	cv::line(histImage,
							cv::Point(bin_w * mean,0),
							cv::Point(bin_w * mean, histImage.rows),
							cv::Scalar(255, 255, 255), 1, 8, 0);*/

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

	cv::circle(histImage,
						cv::Point(bin_w * imax,hist_h - cvRound(histImg.at<float>(imax))),3,
						cv::Scalar(0, 0, 255), 1, 8, 0);
	cv::circle(histImage,
						cv::Point(bin_w * imin,hist_h - cvRound(histImg.at<float>(imin))),
						3,
						cv::Scalar(0, 255, 255), 1, 8, 0);
	cv::circle(histImage,
							cv::Point(bin_w * (imin+ imax)/2,hist_h - cvRound(histImg.at<float>((imin+ imax)/2))),
							3,
							cv::Scalar(0, 255, 0), 1, 8, 0);
	cv::circle(histImage,
			cv::Point(bin_w * imax2,
					hist_h - cvRound(histImg.at<float>(imax2))), 3,
			cv::Scalar(0, 255, 0), 1, 8, 0);
	int mid1 = (imin+imax)/2;
	int mid2 = (imin + imax2)/2;
	int mid = (mid1 + mid2)/2;

	qDebug() << mid1 <<" - " << mid2 <<" - "<<mid;
	cv::threshold(grayImg,grayImg,mid,255,CV_THRESH_BINARY);




	Mat canny_output;
	cv::Canny(grayImg,canny_output,mid,3*mid,5);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);
	findContours(canny_output, contours, hierarchy, RETR_TREE,
			CHAIN_APPROX_SIMPLE, Point(0, 0));
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);

	float totalX = 0;
	float totalY =0;
	float totalPoint =0;

	for (size_t i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));
		qDebug() << "==============================================";
		totalPoint += contours[i].size();
		/**/
		if(contours[i].size() > 2500){
			drawContours(drawing, contours, (int) i, color, 1, 8, hierarchy, 0,
					Point());

			/*for (int j = 0; j < contours[i].size(); j++) {
				int ccx = contours[i][j].x;
				int ccy = contours[i][j].y;
				qDebug() << ccx << "x - y" << ccy << " ";
			}*/
			//getStepEdges(contours[i]);


			sort(contours[i].begin(),contours[i].end(),sortByXaxis);
			//sort(contours[i].begin(),contours[i].end(),sortByYaxis);
			cv::line(drawing,contours[i][0],contours[i][contours[i].size()-1],cv::Scalar(0,255,0),1,8);
			/*cv::Point cv = breakArc(drawing,contours[i]);
			cv::circle(drawing,cv::Point(cv.x,cv.y),10,cv::Scalar(255,0,0),3,8);*/
		}
	}

	/*float x = totalX/totalPoint;
	float y = totalY/totalPoint;
	qDebug()<<x <<" x - y " << y;
	cv::circle(drawing,cv::Point(x,y),10,cv::Scalar(255,255,255),3,8);*/
	//cv::namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
	//cv::imshow("Histogram", histImage);

	/*for(int k = 5; k >1;k=k-2){
			cv::Mat structure = getStructuringElement(MORPH_RECT,cv::Size(k,k),cv::Point(-1,-1));
			cv::dilate(grayImg,grayImg,structure);
	}

	for(int k = 31; k >1;k=k-2){
		cv::Mat structure = getStructuringElement(MORPH_RECT,cv::Size(k,k),cv::Point(-1,-1));
		cv::erode(grayImg,grayImg,structure);
	}
	cv::Mat mask;
	cv::bitwise_not(grayImg,mask);
	cv::bitwise_and(bkImage,mask,grayImg);*/

	return drawing;
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
cv::Mat YellowGrid::landmarkIndentify(cv::Mat inputImage){

	cv::Mat rsImage,grayImage,temp1, temp2;
	cv::cvtColor(inputImage,grayImage,CV_BGR2GRAY);

	// DoG and feature extraction
	int noise = 91;
	cv::GaussianBlur(grayImage,temp1,cv::Size(noise,noise),0,0,BORDER_DEFAULT);
	cv::GaussianBlur(grayImage,temp2,cv::Size((noise * 2) + 1,(noise * 2) + 1),0,0,BORDER_DEFAULT);
	cv::subtract(temp1,temp2,rsImage);

	// edge detection
	/*cv::Mat cannyImage;
	cv::Canny(rsImage,cannyImage,1.5 * noise,3 * noise,5);*/
	/*vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	cv::findContours(cannyImage, contours, hierarchy, CV_RETR_TREE,
			CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	/// Draw contours
	cv::RNG rng(12345);
	Mat drawing = Mat::zeros(cannyImage.size(), CV_8UC3);
	for(size_t i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
		qDebug() << "=================================";
		if(contours[i].size() > 10){
			for(int j = 0; j < contours[i].size(); j++){
				qDebug() << contours[i][j].x << "x" << contours[i][j].y << " ";
				drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
			}
		}
	}*/


	return rsImage;
}
cv::Point YellowGrid::breakArc(cv::Mat &image,vector<cv::Point> lsPoints){
	cv::Point p0 = lsPoints[0],pend = lsPoints[lsPoints.size()-1];

	// line between the endpoints
	double m = (pend.y - p0.y)/(pend.x - p0.x);
	double a = 1;
	double b = -m;
	double c = (m * p0.x) - p0.y;
	double realLength = cv::arcLength(lsPoints,false);


	double lengthSLine = sqrt(pow(pend.x - p0.x,2) + pow((pend.y - p0.y),2));
	qDebug() << lengthSLine;
	int maxDistance = 0;
	int imax = -1;

	for(int i =0 ;i < lsPoints.size();i++){
		cv::Point p = lsPoints[i];
		float d = abs(a * p.x + b*p.y + c)/sqrt(pow(a,2) + pow(b,2));
		if(d > maxDistance){
			maxDistance = d;
			imax = i;
		}
	}
	double lengthImax = sqrt(pow(lsPoints[imax].x - p0.x,2) + pow((lsPoints[imax].y - p0.y),2));
	double ratio = maxDistance/lengthImax;
	double lamda = maxDistance/lengthSLine;

	//if(ratio > lamda){
		/*vector<cv::Point> p1(lsPoints.begin(),lsPoints.begin() + imax);
		vector<cv::Point> p2(lsPoints.begin() + imax,lsPoints.end());
		breakArc(p1);
		breakArc(p2);*/
	//}
		return lsPoints[imax];
}

bool YellowGrid::sortByXaxis(const Point &a, const Point &b){
	return a.x < b.x;
}

bool YellowGrid::sortByYaxis(const Point &a, const Point &b){
	return a.y < b.y;
}

cv::Mat YellowGrid::edgeSegmentation(cv::Mat inputImage,vector<cv::Point> contours){
	//Mat result = Mat::zeros(inputImage.size(), CV_8UC3);

	for(int i =1; i < contours.size();i++){
		cv::line(inputImage,
				contours[i-1],
				contours[i],
				cv::Scalar(255, 0, 0), 1, 8, 0);
	}
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
		distance = (a * pointi.x + b * pointi.y + c)/(sqrt(pow(a,2) +  pow(b,2)));
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
		/*for (int i = 0; i < part1.size(); i++) {
			qDebug() << "(" << part1[i].x << "," << part1[i].y << ")";
		}
		qDebug() << "part 2";
		for (int i = 0; i < part2.size(); i++) {
			qDebug() << "(" << part2[i].x << "," << part2[i].y << ")";
		}*/
		edgeSegmentation(inputImage,part1);
		edgeSegmentation(inputImage,part2);
	}
	RNG rng(12345);
	if(imax != -1){
		cv::circle(inputImage,contours[imax],1,cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255)),1,8);
		queuePoints.enqueue(contours[imax]);
		qDebug() << imax <<" - ("<<p0.x<<","<<p0.y<<") - ("<<pend.x<<","<<pend.y <<")" ;
	}
	return inputImage;
}
void YellowGrid::getStepEdges(vector<cv::Point> listPoints){
	// sort the list of points by X axis
	sort(listPoints.begin(),listPoints.end(),sortByXaxis);

	// two points have least and greatest X
	cv::Point p0 = listPoints[0];
	cv::Point pend = listPoints[listPoints.size()-1];

	// the equation of line through 2 points

	double m  = (double)(pend.y - p0.y)/(double)(pend.x-p0.x);
	double a = m;
	double b = -1;
	double c = p0.y - (m * p0.x);

	QQueue<cv::Point> breakPoints; // store break point between curve and line
	double distance = -1;
	for(int i=0;i<listPoints.size();i++){
		cv::Point pti = listPoints[i];
		distance = (a * pti.x + b * pti.y + c)/(sqrt(pow(a,2) +  pow(b,2)));
		if(distance == 0){
			breakPoints.enqueue(pti);
		}
	}
	qDebug()<<"Queue size: "<<breakPoints.size();

	cv::Point p1 = breakPoints.dequeue();
	cv::Point p2(-1,-1);
	vector<vector<cv::Point> > lsCur;
	bool up =false;
	while(!breakPoints.empty()){
		p2 = breakPoints.dequeue();
		int first = getFirstIndexPoint(listPoints,p1);
		int last = getFirstIndexPoint(listPoints,p2);
		cv::Point pcheck = listPoints[last - 1];
		if(pcheck.y < p2.y){
			up = true; // arc above the line
		}else{
			up = false;
		}

		if(p1.x != -1 && p2.x != -1){

			for(int i =first; i< listPoints.size();i++){
				cv::Point pt = listPoints[i];
				if(pt.x > p2.x)
					break;
				if(pt.x >= p1.x && pt.x <= p2.x){
????
				}
			}
		}
	}
}
int YellowGrid::getFirstIndexPoint(vector<cv::Point> listPoints, cv::Point point){
	for(int i =0;i<listPoints;i++){
		if(listPoints[i].x == point.x
			&& listPoints[i].y == point.y)
			return i;
	}
	return -1;
}
} /* namespace impls_2015 */





























