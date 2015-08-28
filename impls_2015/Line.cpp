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

vector<double> Line::equationOfLine() {
	double m = (double) (this->p2.y - this->p1.y)
			/ (double) (this->p2.x - this->p1.x);

	double a = m;
	double b = -1;
	double c = this->p1.y - (m * this->p1.x);

	if (p1.x == p2.x) {
		m = 1;
		a = m;
		b = 0;
		c = p2.x;
	}
	vector<double> rs;
	rs.push_back(a);
	rs.push_back(b);
	rs.push_back(c);
	return rs;
}
/**
 * Compute the perpendicular distance of a point to line
 * @parameter: point - the point need to compute the distance to line
 * @return: the perpendicular distance from point to line
 */
double Line::perpendicularDistance(cv::Point point) {

	vector<double> equation = this->equationOfLine();
	double a = equation.at(0);
	double b = equation.at(1);
	double c = equation.at(2);
	double distance;
	if (b != 0) {
		distance = ((a * point.x) + (b * point.y) + c)
				/ (sqrt(pow(a, 2) + pow(b, 2)));
	} else {
		distance = point.x - c;
	}
	return distance;
}

/**
 * Find the intersection between two lines
 */
cv::Point Line::intersection(Line objectLine) {
	vector<double> equation1 = this->equationOfLine();
	double a1 = equation1.at(0);
	double b1 = equation1.at(1);
	double c1 = equation1.at(2);

	vector<double> equation2 = objectLine.equationOfLine();
	double a2 = equation2.at(0);
	double b2 = equation2.at(1);
	double c2 = equation2.at(2);

	if ((b1 == 0 && b2 == 0) || (a1 == a2)) // parallel lines
		return cv::Point(-1, -1);

	double a; // = a1 - a2;
	double c; // = c2 - c1;
	double x; // = c / a;
	double y; // = (a1 * x) + c1;

	if (b1 == 0 && b2 != 0) {
		x = c1;
		y = (a2 * x) + c2;
	} else {
		if (b1 != 0 && b2 == 0) {
			x = c2;
			y = (a1 * x) + c1;
		}
	}
	if (b1 != 0 && b2 != 0) {
		a = a1 - a2;
		c = c2 - c1;
		x = c / a;
		y = (a1 * x) + c1;
	}
	//qDebug() << x << " dddd " << y;
	return cv::Point(round(x), round(y));

}
/**
 * Calculate the anlge between two lines (using vector)
 * @parameter: objectLine - the second line
 * @return: the angle between objectLine and this
 */
double Line::angleBetweenLines(Line objectLine) {
	/*	double slope1 = (double) (this->p2.y - this->p1.y)
	 / (double) (this->p2.x - this->p1.x);
	 double slope2 = (double) (objectLine.p2.y - objectLine.p1.y)
	 / (double) (objectLine.p2.x - objectLine.p1.x);
	 double angle = atan((slope1 - slope2) / (1 + (slope1 * slope2))) * 180/M_PI;*/

	cv::Point inter = intersection(objectLine);
	if (inter.x == -1 && inter.y == -1)
		return 0;
	cv::Point ref2, obj2;

	if ((Line(inter, p1)).length() < (Line(inter, p2)).length()) // this->p1.x == inter.x && this->p1.y == inter.y)
		ref2 = p2;
	else
		ref2 = p1;
	if ((Line(inter, objectLine.getP1())).length()
			< (Line(inter, objectLine.getP2())).length()) //(objectLine.getP1().x == inter.x && objectLine.getP1().y == inter.y)
		obj2 = objectLine.getP2();
	else
		obj2 = objectLine.getP1();
	cv::Point u(ref2.x - inter.x, ref2.y - inter.y);
	cv::Point v(obj2.x - inter.x, obj2.y - inter.y);
	double ulength = sqrt((u.x * u.x) + (u.y * u.y));
	double vlength = sqrt((v.x * v.x) + (v.y * v.y));
	double uv = (u.x * v.x) + (u.y * v.y);
	double angle = acos(uv / (ulength * vlength)) * 180 / M_PI;
	return angle;
}

/**
 * Check a point belong to the line or not
 * @parameter: point - the point need to check
 * @return: true, if the point on line; otherwise, false
 */
bool Line::checkBelongPoint(cv::Point point) {
	double distance = abs(perpendicularDistance(point));
	if (distance == 0)
		return true;
	return false;
}

/**
 * Constructor the pairwise histogram of two lines
 * @parameter: objectLine - the object line
 * @return: the pairwise histogram between object line on this
 */
GFeatures Line::pairwiseHistogram(Line objectLine) {
	GFeatures pgh = GFeatures();
	pgh.setAngle(this->angleBetweenLines(objectLine));

	double distance1 = abs(this->perpendicularDistance(objectLine.p1));
	double distance2 = abs(this->perpendicularDistance(objectLine.p2));
	pgh.setDmin((distance1 < distance2 ? distance1 : distance2));
	pgh.setDmax((distance1 < distance2 ? distance2 : distance1));
	return pgh;
}

/**
 * Draw the line
 * @parameter: outputImage - the output image
 * @return: the result image in matrix
 */
cv::Mat Line::drawing(cv::Mat outputImage) {
	//qDebug() << "Draw a line";
	//Mat output = Mat::zeros(outputImage.size(), CV_8UC3);
	cv::line(outputImage, this->p1, this->p2, cv::Scalar(255, 255, 0), 1, 8);
	return outputImage;
}

} /* namespace impls_2015 */
