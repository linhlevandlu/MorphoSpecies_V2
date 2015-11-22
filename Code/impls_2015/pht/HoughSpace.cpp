/*
 * HoughSpace.cpp
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

#include "HoughSpace.h"

namespace impls_2015 {

HoughSpace::HoughSpace() {
	// TODO Auto-generated constructor stub

}

HoughSpace::~HoughSpace() {
	// TODO Auto-generated destructor stub
}

/*
 * Find the intersection on line between line and the perpendicular line with line via a point
 * @parameter 1: objectLine - the object line
 * @parameter 2: origin - the point
 * @return: point on object line, which was intersected
 * 			between line and its perpendicular via origin point
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

/*
 * Set the distance information
 * @parameter: distance - the distance
 */
void HoughSpace::setDistance(double distance) {
	this->distance = distance;
}

/*
 * Set the angle information
 * @parameter: angle - the angle
 */
void HoughSpace::setAngle(double angle) {
	this->angle = angle;
}

/*
 * Get the distance information
 * @return: the distance
 */
double HoughSpace::getDistance() {
	return this->distance;
}

/*
 * Get the angle information
 */
double HoughSpace::getAngle() {
	return this->angle;
}

/*
 * Compute the perpendicular distance from a point to a line
 * @parameter 1: objectLine - the reference line
 * @parameter 2: refPoint - the point
 * @return: the distance from the point to reference line
 */
double HoughSpace::computeDistance(Line objectLine, Point refPoint) {
	return abs(objectLine.perpendicularDistance(refPoint));
}

/*
 * Angle between object line and a line via reference point and parallel with Ox
 * @parameter 1: objectLine - the object line
 * @parameter 2: refPoint - the point
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
