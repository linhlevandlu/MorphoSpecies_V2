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
	double m, a, b, c;

	if (p1.x == p2.x) { // line x = n
		m = 1;
		a = m;
		b = 0;
		c = p2.x;
	} else {
		if (p1.y == p2.y) { // line y = m
			m = 0;
			a = m;
			b = 1;
			c = p2.y;
		} else {
			if (p1.x != p2.x && p1.y != p2.y) { // y = ax + b
				m = (double) (this->p2.y - this->p1.y)
						/ (double) (this->p2.x - this->p1.x);
				a = m;
				b = -1;
				c = this->p1.y - (m * this->p1.x);
			}
		}
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
	if (b != 0 && a != 0) {
		distance = ((a * point.x) + (b * point.y) + c)
				/ (sqrt(pow(a, 2) + pow(b, 2)));
	}
	if (b == 0 && a == 1)
		distance = point.x - c;
	if (a == 0 && b == 1)
		distance = point.y - c;
	return distance;
}

/**
 * Find the intersection between two lines
 */
cv::Point Line::intersection(vector<double> equation1,
		vector<double> equation2) {
	double a1 = equation1.at(0);
	double b1 = equation1.at(1);
	double c1 = equation1.at(2);

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
	return cv::Point(round(x), round(y));

}
cv::Point Line::intersection(Line objectLine) {
	vector<double> equation1 = this->equationOfLine();

	vector<double> equation2 = objectLine.equationOfLine();
	return intersection(equation1, equation2);

}
/**
 * Calculate the anlge between two lines (using vector)
 * @parameter: objectLine - the second line
 * @return: the angle between objectLine and this
 */
double Line::angleBetweenLines(Line objectLine) {

	cv::Point inter = intersection(objectLine);
	if (inter.x == -1 && inter.y == -1) // parallel line
		return 0;
	cv::Point ref2, obj2;

	if ((Line(inter, p1)).length() < (Line(inter, p2)).length())
		ref2 = p2;
	else
		ref2 = p1;
	if ((Line(inter, objectLine.getP1())).length()
			< (Line(inter, objectLine.getP2())).length())
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
bool Line::operator==(Line &line) {
	cv::Point lp1 = line.p1;
	cv::Point lp2 = line.p2;
	if ((this->p1.x == lp1.x && this->p1.y == lp1.y && this->p2.x == lp2.x
			&& this->p2.y == lp2.y)
			|| (this->p1.x == lp2.x && this->p1.y == lp2.y
					&& this->p2.x == lp1.x && this->p2.y == lp1.y))
		return true;
	return false;
}
vector<vector<double> > Line::parallelLine(double distance) {
	int x0 = (this->p1.x + this->p2.x) / 2;
	vector<double> eq = this->equationOfLine();
	double a, b, c, c1;
	if (eq[0] == 0) { //a = 0
		b = 1;
		a = 0;
		c = eq[2] + distance;
	} else {
		if (eq[1] == 0) { // b = 0
			b = 0;
			a = 1;
			c = eq[2] + distance;
		} else {
			a = eq[0];
			b = eq[1];
			int y0 = a * x0 + eq[2];
			double k = a * x0 + b * y0;
			double m = distance * (sqrt(a * a + b * b));
			double delta = 4 * m * m;
			if (delta > 0) {
				c = m - k;
				c1 = -m - k;
				//qDebug() << "Nghiem cua phuong trinh: " << c << ", " << c1;
			}
			//double c2 = (distance * (sqrt(a * a + b * b))) - (a * x0 + b * y0);
			//qDebug() << c2;
		}
	}
	vector<vector<double> > equations;
	vector<double> equation1;
	equation1.push_back(a);
	equation1.push_back(b);
	equation1.push_back(c);

	vector<double> equation2;
	equation2.push_back(a);
	equation2.push_back(b);
	equation2.push_back(c1);

	equations.push_back(equation1);
	equations.push_back(equation2);
	return equations;
}

Point Line::interParallel(Line line1, Line line2, double distance1,
		double distance2, int width, int height) {
	vector<double> equation1 = line1.equationOfLine();
	vector<double> equation2 = line2.equationOfLine();
	double a = equation1[0];
	double b = equation1[2];
	double c = equation2[0];
	double d = equation2[2];
	double e = distance1 * (sqrt((a * a) + 1));
	double f = distance2 * (sqrt((c * c) + 1));

	qDebug() << distance1 << ", distance 2: " << distance2;

	Point refPoint(width / 2, height / 2);
	double minDistance = width;

	double x0 = 0, y0 = 0, x = 0, y = 0;
	x0 = (f + b - d - e) / (c - a);
	if (x0 >= 0 && x0 < width) {
		y0 = a * x0 + b - e;
		if (y0 >= 0 && y0 < height) {
			qDebug() << "1 nghiem " << x0 << "," << y0;
			Line l1(refPoint, Point(x0, y0));
			if (l1.length() < minDistance) {
				minDistance = l1.length();
				x = x0;
				y = y0;

			}
		}
	}
	x0 = (b - e - f - d) / (c - a);
	if (x0 >= 0 && x0 < width) {
		y0 = a * x0 + b - e;
		if (y0 >= 0 && y0 < height) {
			qDebug() << "2 nghiem " << x0 << "," << y0;
			Line l2(refPoint, Point(x0, y0));
			if (l2.length() < minDistance) {
				minDistance = l2.length();
				x = x0;
				y = y0;
			}
		}
	}
	x0 = (f + b - d + e) / (c - a);
	if (x0 >= 0 && x0 < width) {
		y0 = a * x0 + b + e;
		if (y0 >= 0 && y0 < height) {
			qDebug() << "3 nghiem " << x0 << "," << y0;
			Line l3(refPoint, Point(x0, y0));
			if (l3.length() < minDistance) {
				minDistance = l3.length();
				x = x0;
				y = y0;
			}
		}
	}
	x0 = (b + e - f - d) / (c - a);
	if (x0 >= 0 && x0 < width) {
		y0 = a * x0 + b + e;
		if (y0 >= 0 && y0 < height) {
			qDebug() << "4 nghiem " << x0 << "," << y0;
			Line l4(refPoint, Point(x0, y0));
			if (l4.length() < minDistance) {
				minDistance = l4.length();
				x = x0;
				y = y0;
			}
		}
	}
	return Point(round(x), round(y));
}

void Line::toString() {
	qDebug() << "((" << p1.x << ", " << p1.y << "),(" << p2.x << ", " << p2.y
			<< "))";
}

bool Line::operator<(const Line& l2) const {
	if (p1.x < l2.p1.x)
		return true;
	return false;
}
bool Line::isNull() {
	if (p1.x == p2.x && p1.y == p2.y)
		return true;
	return false;
}
} /* namespace impls_2015 */
