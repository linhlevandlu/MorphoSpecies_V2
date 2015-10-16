/*
 * Lines.h
 *
 *  Created on: Jun 24, 2015
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

#ifndef LINE_H_
#define LINE_H_

#include <QtGui/QDialog>
#include <QtGui/QWidget>
#include <QtGui/QSlider>
#include <QtCore/qqueue.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

#include "IDrawOperation.h"
#include "GFeatures.h"

using namespace cv;
using namespace std;

namespace impls_2015 {

class Line: public QLine, IDrawOperation {
private:
	cv::Point p1;
	cv::Point p2;

public:
	vector<double> equationOfLine();
	Line();
	Line(cv::Point p1, cv::Point p2);

	cv::Point getP1();
	void setP1(cv::Point p);
	cv::Point getP2();
	void setP2(cv::Point p);
	double length();
	double perpendicularDistance(cv::Point point);
	cv::Point intersection(vector<double> equation1, vector<double> equation2);
	cv::Point intersection(Line objectLine);
	double angleBetweenLines(Line objectLine);
	bool checkBelongPoint(cv::Point point);
	GFeatures pairwiseHistogram(Line objectLine);
	cv::Mat drawing(cv::Mat outputImage);
	bool operator==(Line &line);
	bool operator<(const Line& l2) const;
	void toString();
	vector<vector<double> > parallelLine(double distance);
	Point interParallel(Line line1, Line line2, double distance1,
			double distance2, int width, int height);
	bool isNull();
};

} /* namespace impls_2015 */
#endif /* LINES_H_ */
