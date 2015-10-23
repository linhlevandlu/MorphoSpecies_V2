/*
 * Landmark.cpp
 *
 *  Created on: Aug 21, 2015
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

#include "Landmark.h"

namespace impls_2015 {

/**
 * The constructor of landmark
 */
Landmark::Landmark() {

}

/**
 * The constructor of landmark with a point
 */
Landmark::Landmark(cv::Point location) {
	this->location = location;

}

Landmark::~Landmark() {
	// TODO Auto-generated destructor stub
}

/**
 * Get location of landmark
 * @return: position of landmark
 */
cv::Point Landmark::getLocation() {
	return this->location;
}

/**
 * Set the location of landmark
 * @parameter: location - the coordinate of landmark
 */
void Landmark::setLocation(cv::Point location) {
	this->location = location;
}

/**
 * Draw the landmark
 * @parameter: outputImage - the image contains the result
 * @return: an image contains the landmark
 */
void Landmark::drawing(cv::Mat &outputImage) {
	cv::circle(outputImage, this->location, 1, cv::Scalar(0, 0, 255), 1, 8);
}

} /* namespace impls_2015 */
