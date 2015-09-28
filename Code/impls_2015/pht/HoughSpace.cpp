/*
 * HoughSpace.cpp
 *
 *  Created on: Sep 27, 2015
 *      Author: linh
 */

#include "HoughSpace.h"

namespace impls_2015 {

HoughSpace::HoughSpace() {
	// TODO Auto-generated constructor stub

}

HoughSpace::~HoughSpace() {
	// TODO Auto-generated destructor stub
}
void HoughSpace::setDistance(Line objectLine, Point refPoint) {
	this->distance = abs(objectLine.perpendicularDistance(refPoint));
}
void HoughSpace::setAngle(Line objectLine, Point refPoint) {
	Line oX(refPoint, cv::Point(refPoint.x + 10, refPoint.y));
	double temp = oX.angleBetweenLines(objectLine);
	double theta;
	if (temp <= 90) {
		theta = 90 - temp;
	} else {
		theta = temp - 90;
	}
	this->angle = theta;
}
double HoughSpace::getDistance() {
	return this->distance;
}
double HoughSpace::getAngle() {
	return this->angle;
}
} /* namespace impls_2015 */
