/*
 * LandmarkDetection.cpp
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

#include "LandmarkDetection.h"

namespace impls_2015 {

/**
 * Constructor the landmarks detection
 */
LandmarkDetection::LandmarkDetection() {
}

LandmarkDetection::~LandmarkDetection() {
	// TODO Auto-generated destructor stub
}
/**
 * Set the noise when find the landmarks
 * @parameter: noise - the noise
 */
void LandmarkDetection::setNoise(double noise) {
	this->noise = noise;
}

/**
 * Get the landmarks of image
 * @return: the list of landmarks in image
 */
vector<Landmark> LandmarkDetection::getLandmarks(Image image) {
	vector<Landmark> landmarks;
	return landmarks;
}


} /* namespace impls_2015 */
