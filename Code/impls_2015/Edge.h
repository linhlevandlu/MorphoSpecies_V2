/*
 * Edge.h
 *
 *  Created on: Aug 20, 2015
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

#ifndef EDGE_H_
#define EDGE_H_

#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include <QtCore/qlist.h>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <QtCore/QDebug>
#include <iostream>
#include <istream>
#include <fstream>

#include "Line.h"
#include "IDrawOperation.h"

using namespace cv;
using namespace std;

namespace impls_2015 {

class Edge: public IDrawOperation {
private:
	vector<Line> listOfLines;
	vector<cv::Point> listOfPoints;
	static vector<cv::Point> breakPoints;

	bool checkPointInList(cv::Point point);
	void breakEdge();
	vector<Point> breakEdge(vector<Point> contour);

public:
	Edge();
	Edge(vector<Line> lines);
	Edge(vector<cv::Point> points);
	virtual ~Edge();

	vector<Line> getLines();
	vector<Line> getLines(vector<cv::Point> listPoints);
	void setLines(vector<Line> lines);
	vector<cv::Point> getPoints();
	void setPoints(vector<cv::Point> points);
	void addLine(Line line);
	vector<cv::Point> segment();
	cv::Mat drawing(cv::Mat outputImage);

	static vector<Line> readFile(QString filePath);

};

} /* namespace impls_2015 */
#endif /* EDGE_H_ */
