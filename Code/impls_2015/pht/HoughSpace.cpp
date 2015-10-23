/*
 * HoughSpace.cpp
 *
 *  Created on: Sep 27, 2015
 *      Author: linh
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

#include "HoughSpace.h"

namespace impls_2015 {

HoughSpace::HoughSpace() {
	// TODO Auto-generated constructor stub

}

HoughSpace::~HoughSpace() {
	// TODO Auto-generated destructor stub
}

/**
 * Tim giao diem cua objectLine va duong thang di qua diem origin, vuong goc voi objectLine
 */
cv::Point HoughSpace::closestPoint(Line objectLine, Point origin) {
	vector<double> equation = objectLine.equationOfLine();
	double a = equation.at(0);
	double b = equation.at(1);
	double c = equation.at(2);
	double x, y;
	if (a == 0 && b == 1) {
		x = origin.x;
		y = c;
	} else {
		if (a == 1 && b == 0) {
			x = c;
			y = origin.y;
		} else {
			x = (b * (b * origin.x - a * origin.y) - (a * c)) / (a * a + b * b);
			y = (a * (-b * origin.x + a * origin.y) - (b * c))
					/ (a * a + b * b);
		}
	}
	return Point(x, y);
}
void HoughSpace::setDistance(double distance) {
	this->distance = distance;
}
void HoughSpace::setAngle(double angle) {
	this->angle = angle;
}
double HoughSpace::getDistance() {
	return this->distance;
}
double HoughSpace::getAngle() {
	return this->angle;
}

double HoughSpace::computeDistance(Line objectLine, Point refPoint) {
	return abs(objectLine.perpendicularDistance(refPoint));
}

/**
 * Tinh goc tu refPoint toi objectLine (Ox la di qua refPoint)
 */
double HoughSpace::computeAngle(Line objectLine, Point refPoint) {
	Line oX(refPoint, cv::Point(refPoint.x + 100, refPoint.y));
	cv::Point pCloset = closestPoint(objectLine, refPoint);
	Line distanceLine(refPoint, pCloset);
	double theta = oX.angleBetweenLines(distanceLine);
	if (pCloset.y > refPoint.y)
		return (360 - theta);
	return theta;
}
} /* namespace impls_2015 */
