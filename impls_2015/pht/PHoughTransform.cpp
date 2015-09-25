/*
 * PHoughTransform.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: linh
 */

#include "PHoughTransform.h"

namespace impls_2015 {

PHoughTransform::PHoughTransform() {
	// TODO Auto-generated constructor stub

}

PHoughTransform::~PHoughTransform() {
	// TODO Auto-generated destructor stub
}
cv::Mat PHoughTransform::pHoughTransform(cv::Mat inputImage) {
	cv::Mat dst;
	cv::Canny(inputImage, dst, 50, 255, 3);
	vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI / 180, 60, 20, 10);
	for (size_t i = 0; i < lines.size(); i++) {
		Vec4i line = lines[i];
		cv::line(inputImage, Point(line[0], line[1]), Point(line[2], line[3]),
				Scalar(0, 0, 255), 1, 8);
	}

	return inputImage;
}

vector<vector<int> > PHoughTransform::createAccumulator(vector<Line> lines,
		int width, int height) {
	this->image_height = height;
	this->image_width = width;

	int acc_w = (int) sqrt((width * width) + (height * height));
	int acc_h = 181;

	cv::Mat mat(cv::Size(acc_w, acc_h), CV_8UC3, cv::Scalar(0, 0, 0));

	// Initialization of accumulator array
	vector<vector<int> > accumulator;
	accumulator.resize(acc_h);
	for (int k = 0; k < acc_h; k++) {
		accumulator[k].resize(acc_w, 0);
	}

	for (size_t i = 0; i < lines.size(); i++) {
		Line line = lines.at(i);
		Point p1 = line.getP1();
		Point p2 = line.getP2();

		for (int j = 0; j <= 180; j++) {
			// assign the value of P1 into accumulator array
			double r1 = (double) (p1.x * cos(double(j * M_PI / 180)))
					+ (double) (p1.y * sin(double(j * M_PI / 180)));

			if (r1 > 0 && r1 <= acc_w) {
				accumulator[j][(int) round(r1)] += 1;
				mat.at<Vec3b>(j, round(r1)) = 255;
			}

			// assign the value of P2 into accumulator array
			double r2 = (double) (p2.x * cos(double(j * M_PI / 180)))
					+ (double) (p2.y * sin(double(j * M_PI / 180)));
			if (r2 > 0 && r2 <= acc_w) {
				accumulator[j][(int) round(r2)] += 1;
				mat.at<Vec3b>(j, round(r2)) = 255;
			}
		}
	}
	cv::namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
	cv::imshow("Histogram", mat);
	return accumulator;
}

int PHoughTransform::maxOfAccumulator(vector<vector<int> > accumulator) {
	int max = 0;
	for (size_t i = 0; i < accumulator.size(); i++) {
		for (size_t j = 0; j < accumulator[i].size(); j++) {
			if (accumulator[i][j] > max)
				max = accumulator[i][j];
		}
	}
	return max;
}
vector<Line> PHoughTransform::drawAccumulator(vector<vector<int> > accumulator,
		int threshold) {
	cv::Mat mat(cv::Size(accumulator[0].size(), accumulator.size()), CV_8UC3,
			cv::Scalar(0, 0, 0));
	vector<Line> lines;
	for (size_t i = 0; i < accumulator.size(); i++) { // theta
		for (size_t j = 0; j < accumulator[i].size(); j++) { // r
			if (accumulator[i][j] > threshold) {
				Point p1, p2;
				p1.x = 0;
				p1.y = (j - p1.x * cos(double(i * M_PI / 180)))
						/ sin(double(i * M_PI / 180));
				p2.x = image_height - 0;
				p2.y = (j - p2.x * cos(double(i * M_PI / 180)))
						/ sin(double(i * M_PI / 180));
				lines.push_back(Line(p1,p2));

				qDebug() << "position (theta, r): " << i << ", " << j
						<< ", value: " << accumulator[i][j];
			} else
				accumulator[i][j] = 0;
		}
	}
	return lines;
}
} /* namespace impls_2015 */
