/*
 * Lines.cpp
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

#include "Line.h"
#include <QtCore/qdebug.h>
#include <QtCore/QDebug>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>
#include <QtGui/qslider.h>
#include <QtCore/qset.h>
#include <QtCore/qlist.h>
#include <math.h>

namespace impls_2015 {

/**
 * Constructor of a line
 */
Line::Line() {

}

/**
 * Constructor of a line with 2 points
 * @parameter: p1 - the first point
 * @parameter: p2 - the second point
 */
Line::Line(cv::Point p1, cv::Point p2) {
	this->p1 = p1;
	this->p2 = p2;
}

/**
 * Constructor of a line with 2 points
 * @parameter: p1 - the first point
 * @parameter: p2 - the second point
 */
Line::Line(QPoint p1, QPoint p2) {
	this->p1 = cv::Point(p1.x(), p1.y());
	this->p2 = cv::Point(p2.x(), p2.y());
}

/**
 * Get the first point of line
 * @return: the first point of line
 */
cv::Point Line::getP1() {
	return this->p1;
}

/**
 * Set the first point
 * @parameter: p - the first point
 */
void Line::setP1(cv::Point p) {
	this->p1 = p;
}

/**
 * Get the second point of line
 * @return: the second point of line
 */
cv::Point Line::getP2() {
	return this->p2;

}

/**
 * Set the second point of line
 * @parameter: p - the second point of line
 */
void Line::setP2(cv::Point p) {
	this->p2 = p;
}

/**
 * Compute the length of line
 * @return: the length of line
 */
double Line::length() {
	double distance = sqrt(
			pow(this->p2.x - this->p1.x, 2) + pow(this->p2.y - this->p1.y, 2));
	return distance;
}

/**
 * Compute the perpendicular distance of a point to line
 * @parameter: point - the point need to compute the distance to line
 * @return: the perpendicular distance from point to line
 */
double Line::perpendicularDistance(cv::Point point) {
	double m = (double) (this->p2.y - this->p1.y)
			/ (double) (this->p2.x - this->p1.x);
	double a = m;
	double b = -1;
	double c = this->p1.y - (m * this->p1.x);
	double distance = abs(a * point.x + b * point.y + c)
			/ (sqrt(pow(a, 2) + pow(b, 2)));
	return distance;
}
/**
 * Calculate the anlge between two lines
 * @parameter: objectLine - the second line
 * @return: the angle between objectLine and this
 */
double Line::angleBetweenLines(Line objectLine) {
	double slope1 = (double) (this->p2.y - this->p1.y)
			/ (double) (this->p2.x - this->p1.x);
	double slope2 = (double) (objectLine.p2.y - objectLine.p1.y)
			/ (double) (objectLine.p2.x - objectLine.p1.x);
	double angle = atan((slope1 - slope2) / (1 + (slope1 * slope2))) * 180/M_PI;
	return angle;
}

/**
 * Check a point belong to the line or not
 * @parameter: point - the point need to check
 * @return: true, if the point on line; otherwise, false
 */
bool Line::checkBelongPoint(cv::Point point) {
	double distance = perpendicularDistance(point);
	if (distance == 0)
		return true;
	return false;
}

/**
 * Constructor the pairwise histogram of two lines
 * @parameter: objectLine - the object line
 * @return: the pairwise histogram between object line on this
 */
void Line::pairwiseHistogram(Line objectLine) {

}

/**
 * Draw the line
 * @parameter: outputImage - the output image
 * @return: the result image in matrix
 */
cv::Mat Line::drawing(cv::Mat outputImage) {
	qDebug() << "Draw a line";
	//Mat output = Mat::zeros(outputImage.size(), CV_8UC3);
	cv::line(outputImage, this->p1, this->p2, cv::Scalar(255, 0, 0), 1, 8);
	return outputImage;
}

} /* namespace impls_2015 */
