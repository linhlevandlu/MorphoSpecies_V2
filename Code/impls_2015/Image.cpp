/*
 * Image.cpp
 *
 *  Created on: Aug 21, 2015
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

#include "Image.h"

namespace impls_2015 {

/**
 * Constructor of image
 */
Image::Image() {

}

/**
 * Constructor of image with a path
 */
Image::Image(QString filePath) {
	this->fileName = filePath;
	this->histogramSize = 256;
	this->matrixImage = getMatImage();
	getEdges();
}

Image::~Image() {
	// TODO Auto-generated destructor stub
}

/*
 * Get the file name of image
 * @return: the file path of image
 */
QString Image::getFileName() {
	return this->fileName;
}

/*
 * Set the file path of image
 * @filePath: the image path
 */
void Image::setFileName(QString filePath) {
	this->fileName = filePath;
}

/*
 * Get the edges of image
 * @return: the list of edges of image
 */
vector<Edge> Image::getEdges() {
	cv::Mat grayImg, orgImg = this->matrixImage;

	cv::cvtColor(orgImg, grayImg, CV_BGR2GRAY);
	//cv::cvtColor(this->matrixImage, grayImg, CV_BGR2GRAY);

	cv::Mat histImg = histogram();
	// calculate the median
	double median = medianHistogram();
	float mean = meanHistogram();
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

	//qDebug() << "middle value: " << mid;
	Mat cannyImage;
	cv::Canny(grayImg, cannyImage, mid, 3 * mid, 5);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);
	findContours(cannyImage, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE,
			Point(0, 0));
	vector<Edge> edges;

	for (size_t i = 0; i < contours.size(); i++) {
		Edge edge(contours[i]);
		edges.push_back(edge);
	}
	this->listOfEdges = edges;
	return edges;
}

/**
 * Set the edges of image
 * @parameter: edges - edges of image
 */
void Image::setEdges(vector<Edge> edges) {
	this->listOfEdges = edges;
}

/**
 * Get the landmarks of image
 * @return: list of landmarks of image
 */
/*QList<Landmark> Image::getLandmarks() {
 return this->listOfLandmarks;
 }*/

/**
 * Set the landmarks of image
 * @parameter: landmarks - the landmarks of image
 */
/*void Image::setLandmarks(QList<Landmark> landmarks) {
 this->listOfLandmarks = landmarks;
 }*/

/**
 * Get the bins of histogram
 * @return: the number of bins
 */
int Image::getHistSize() {
	return this->histogramSize;
}

/*
 * Set the bin to calculate the histogram
 * @parameter: histSize - the size of bins
 */
void Image::setHistSize(int histSize) {
	this->histogramSize = histSize;
}
cv::Mat Image::getMatrixImage() {
	return this->matrixImage;
}
/**
 * Get the image in Matrix
 * @return: the matrix represented the image
 */
cv::Mat Image::getMatImage() {
	return cv::imread(fileName.toStdString());
}

/**
 * Add an edge into the list of edges
 * @parameter: edge - edge need to add into the list of edges
 */
void Image::addEdge(Edge edge) {
	this->listOfEdges.push_back(edge);
}

/**
 * Add a landmarks into the list of landmarks
 * @parameter: landmark - the landmark need to add into the list of landmarks
 */
/*void Image::addLandmark(Landmark landmark) {
 this->listOfLandmarks.append(landmark);
 }*/

/**
 * Compute the histogram of image
 * @return: the histogram of image in matrix
 */
cv::Mat Image::histogram() {
	cv::Mat inputImage, grayImage;
	inputImage = this->matrixImage;

	if (inputImage.channels() == 3)
		cv::cvtColor(inputImage, grayImage, CV_BGR2GRAY);
	else
		inputImage.copyTo(grayImage);

	//int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true;
	bool accumulate = false;

	cv::Mat histImage;

	/// Compute the histograms:
	cv::calcHist(&grayImage, 1, 0, cv::Mat(), histImage, 1,
			&this->histogramSize, &histRange, uniform, accumulate);
	return histImage;
}

/**
 * Compute the median of histogram
 * @return: the value of median
 */
double Image::medianHistogram() {
	cv::Mat hImage = histogram();
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
 * Compute the mean of histogram
 * @return: mean value of histogram
 */
float Image::meanHistogram() {
	cv::Mat hImage = histogram();
	int total = 0;
	float pi = 0;
	for (int i = 0; i < 256; i++) {
		total += hImage.at<float>(i);
		pi += (i * hImage.at<float>(i));
	}

	return (pi / total);
}

/**
 * Drawing the histogram of image
 * @return: presentation histogram of image in matrix
 */
cv::Mat Image::drawingHistogram() {
	cv::Mat hImage = histogram();
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
	return histImage;
}

/**
 * Drawing the image
 * @parameter: outputImage - the output image
 * @return: the image in matrix
 */
cv::Mat Image::drawing(cv::Mat outputImage) {
	//Mat output = Mat::zeros(inputImage.size(), CV_8UC3);
	QString title;
	if (listOfEdges.size() > 0) {
		qDebug() << "Draw the image.";
		for (size_t i = 0; i < listOfEdges.size(); i++) {
			Edge edgei = listOfEdges.at(i);
			outputImage = edgei.drawing(outputImage);
		}
		title = "The image";
	}

	return outputImage;
}

/**
 * Find the limit point of yellow grid in image
 * @return: the limit point of grid
 */
cv::Point Image::findLimitPoint() {
	vector<cv::Mat> hsv_planes = splitChannels();

	cv::Point limit_point = cv::Point(0, 0);
	int yellow_count = 0;
	for (int j = 10; j < hsv_planes[0].cols; j++) {
		if (hsv_planes[0].at<uchar>(5, j) > 100
				|| (hsv_planes[0].at<uchar>(5, j) > 70
						&& hsv_planes[0].at<uchar>(5, j) < 100
						&& hsv_planes[1].at<uchar>(5, j) < 10
						&& hsv_planes[2].at<uchar>(5, j) > 175)) {
			limit_point.x = j;
			limit_point.y = 0;
			yellow_count = 0;
			for (int i = 1; i < hsv_planes[0].rows * 2 / 3; i++) {
				if (hsv_planes[0].at<uchar>(i, j) <= 38) {
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
		limit_point.x = hsv_planes[0].cols / 3 + 200;
		limit_point.y = 0;
	}
	return limit_point;
}

/**
 * Find the replace point for yellow point in image
 * @return: the coordinate of point where we use the value to replace the yellow point
 */
cv::Point Image::findReplacePoint() {
	cv::Mat inputImage = this->matrixImage;
	cv::Mat gray_img, hist_img;
	if (inputImage.channels() == 3)
		cv::cvtColor(inputImage, gray_img, CV_BGR2GRAY);
	else
		gray_img = inputImage;
	hist_img = this->histogram();
	float avg = meanHistogram();

	vector<cv::Mat> hsv_planes = splitChannels();
	// find the replate point
	cv::Point repPoint = cv::Point(0, 0);
	for (int i = 0; i < gray_img.rows; i++) {
		for (int j = 0; j < gray_img.cols; j++) {
			if (gray_img.at<float>(i, j) >= avg
					&& hsv_planes[0].at<uchar>(i, j) > 90
					&& hsv_planes[0].at<uchar>(i, j) < 130
					&& hsv_planes[1].at<uchar>(i, j) > 51
					&& hsv_planes[2].at<uchar>(i, j) > 215) { // background
				repPoint.x = i;
				repPoint.y = j;
				break;
			}
		}
		if (repPoint.x != 0)
			break;
	}
	return repPoint;
}

/**
 * Get which part of insect
 * @return: the insect part
 */
Image::InsectPart Image::getPart() {
	Image::InsectPart sType;
	QString temp = this->fileName.toLower();
	if (temp.contains("ely"))
		sType = ELYTRE;
	else {
		if (temp.contains("md"))
			sType = MDROITE;
		else {
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
 * Remove the grid with a minimum brightness
 * @parameter: minBrightness - the minimum brightness
 * @return: the image after removing the grid in matrix
 */
cv::Mat Image::removingGrid(int minBrightness) {
	Image::InsectPart insectPart = getPart();
	cv::Point limit_point = findLimitPoint();
	cv::Point bg = findReplacePoint();

	cv::Mat inputImage = this->matrixImage;
	vector<cv::Mat> hsv_planes;
	hsv_planes = splitChannels();
	// replace the points
	for (int i = 0; i < hsv_planes[0].rows; i++) {
		for (int j = 0; j < limit_point.x; j++) {
			if (insectPart == ELYTRE) {
				if (j > hsv_planes[0].cols / 4
						&& ((hsv_planes[0].at<uchar>(i, j + 50) >= 90
								&& hsv_planes[0].at<uchar>(i, j + 50) <= 130
								&& hsv_planes[2].at<uchar>(i, j + 50) >= 10
								&& hsv_planes[2].at<uchar>(i, j + 50) <= 120)
								|| (hsv_planes[0].at<uchar>(i, j + 50) >= 90
										&& hsv_planes[0].at<uchar>(i, j + 50)
												<= 130
										&& hsv_planes[1].at<uchar>(i, j + 50)
												>= 60
										&& hsv_planes[2].at<uchar>(i, j + 50)
												>= 10
										&& hsv_planes[2].at<uchar>(i, j + 50)
												<= 120)))
					break;
			}

			if (insectPart == MDROITE || insectPart == MGAUCHE) {
				if ((hsv_planes[0].at<uchar>(i, j + 50) >= 39
						&& hsv_planes[0].at<uchar>(i, j + 50) <= 130
						&& hsv_planes[1].at<uchar>(i, j + 50) >= 39))
					break;
			}
			if (insectPart == PRONOTUM) {
				if (j > hsv_planes[0].cols / 5
						&& ((hsv_planes[0].at<uchar>(i, j + 50) >= 40
								&& hsv_planes[0].at<uchar>(i, j + 50) <= 170
								&& hsv_planes[1].at<uchar>(i, j + 50) >= 30
								&& hsv_planes[1].at<uchar>(i, j + 50) <= 95
								&& hsv_planes[2].at<uchar>(i, j + 50) >= 35
								&& hsv_planes[2].at<uchar>(i, j + 50) <= 255)))
					break;
			}
			if (insectPart == TETE) {
				if (j > hsv_planes[0].cols / 4
						&& ((hsv_planes[0].at<uchar>(i, j + 50) >= 39
								&& hsv_planes[0].at<uchar>(i, j + 50) <= 140
								&& hsv_planes[2].at<uchar>(i, j + 50) >= 10
								&& hsv_planes[2].at<uchar>(i, j + 50) <= 145)
								|| (hsv_planes[0].at<uchar>(i, j + 50) > 130
										&& hsv_planes[1].at<uchar>(i, j + 50)
												< 70
										&& hsv_planes[2].at<uchar>(i, j + 50)
												> 120)
								|| (hsv_planes[0].at<uchar>(i, j + 50) >= 3
										&& hsv_planes[0].at<uchar>(i, j + 50)
												<= 15
										&& hsv_planes[2].at<uchar>(i, j + 50)
												>= 120
										&& hsv_planes[2].at<uchar>(i, j + 50)
												<= 153)))
					break;
			}
			// replace the points
			if (hsv_planes[0].at<uchar>(i, j) <= 38
					&& (hsv_planes[2].at<uchar>(i, j) >= 10
							&& hsv_planes[2].at<uchar>(i, j) <= 100)) {
				hsv_planes[0].at<uchar>(i, j) = hsv_planes[0].at<uchar>(bg.x,
						bg.y);
				hsv_planes[1].at<uchar>(i, j) = hsv_planes[1].at<uchar>(bg.x,
						bg.y);
				hsv_planes[2].at<uchar>(i, j) = hsv_planes[2].at<uchar>(bg.x,
						bg.y);
			} else { // there are not yellow points
					 // replace the point what do not enough the bright (not in yellow range)
				if (hsv_planes[2].at<uchar>(i, j) > minBrightness) {
					hsv_planes[0].at<uchar>(i, j) = hsv_planes[0].at<uchar>(
							bg.x, bg.y);
					hsv_planes[1].at<uchar>(i, j) = hsv_planes[1].at<uchar>(
							bg.x, bg.y);
					hsv_planes[2].at<uchar>(i, j) = hsv_planes[2].at<uchar>(
							bg.x, bg.y);
				}
			}
		}
	}

	cv::Mat dest;
	// merge bgr_planes to dest image
	cv::merge(hsv_planes, dest);
	cv::cvtColor(dest, dest, cv::COLOR_HSV2BGR);
	return dest;
}

/**
 * Convert and split image into several channels in HSV mode
 * @return: a vector contains the channels of image in HSV mode
 */
vector<cv::Mat> Image::splitChannels() {
	cv::Mat hsvImage;
	vector<cv::Mat> hsv_planes;
	cv::cvtColor(this->matrixImage, hsvImage, cv::COLOR_BGR2HSV);
	cv::split(hsvImage, hsv_planes);
	return hsv_planes;
}
vector<Line> Image::lineSegment() {
	vector<Line> totalLines;
	for (size_t i = 0; i < this->listOfEdges.size(); i++) {
		Edge ed = this->listOfEdges.at(i);
		vector<cv::Point> breakPoints = ed.segment();
		vector<Line> lines = ed.getLines(breakPoints);
		totalLines.insert(totalLines.end(), lines.begin(), lines.end());
	}
	qDebug() << "Total lines: " << totalLines.size();
	return totalLines;
}
void Image::setShapeHistogram(ShapeHistogram shapeHistogram) {
	this->pghHistogram = shapeHistogram;
}
ShapeHistogram Image::getShapeHistogram() {
	return this->pghHistogram;
}
vector<Point> Image::readLandmarksFile(QString filePath) {
	ifstream openFile(filePath.toStdString().c_str());
	string lineText;
	vector<Point> landmarks;
	if (openFile.is_open()) {
		getline(openFile, lineText);
		QString firsLine = (lineText.c_str());
		int numOfLandmarks = firsLine.split("=").at(1).toInt();
		int i = 0;
		while (i < numOfLandmarks) {
			getline(openFile, lineText);
			QString line = lineText.c_str();
			QStringList listString = line.split(" ");
			cv::Point point1(listString.at(0).toDouble(),
					listString.at(1).toDouble());
			landmarks.push_back(point1);
			i++;
		}
		openFile.close();
	}
	return landmarks;
}
QFileInfoList Image::readFolder(QString folderPath) {
	QDir qdir;
	qdir.setPath(folderPath);
	qdir.setFilter(QDir::Files);
	QStringList filterNames;
	filterNames << "*.JPG";
	qdir.setNameFilters(QStringList("*.JPG"));
	QFileInfoList files = qdir.entryInfoList();
	return files;
}
} /* namespace impls_2015 */
