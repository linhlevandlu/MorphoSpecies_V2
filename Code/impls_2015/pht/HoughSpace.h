/*
 * HoughSpace.h
 *
 *  Created on: Sep 27, 2015
 *  Image processing for morphometrics (IPM) Version 0.2
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

#ifndef HOUGHSPACE_H_
#define HOUGHSPACE_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <time.h>

#include "../Line.h"

using namespace std;
using namespace cv;

namespace impls_2015 {

class HoughSpace {
private:
	double distance;
	double angle;
public:
	HoughSpace();
	virtual ~HoughSpace();
	void setDistance(double distance);
	void setAngle(double angle);
	double getDistance();
	double getAngle();
	double computeDistance(Line objectLine, Point refPoint);
	double computeAngle(Line objectLine, Point refPoint);
	cv::Point closestPoint(Line objectLine, Point origin);
};

} /* namespace impls_2015 */
#endif /* HOUGHSPACE_H_ */
