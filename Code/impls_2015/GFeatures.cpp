/*
 * PairwiseHistogram.cpp
 *
 *  Created on: Aug 24, 2015
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

#include "GFeatures.h"

namespace impls_2015 {

GFeatures::GFeatures() {
	// TODO Auto-generated constructor stub

}

GFeatures::~GFeatures() {
	// TODO Auto-generated destructor stub
}
double GFeatures::getAngle() {
	return this->angle;
}
double GFeatures::getDmin() {
	return this->dmin;
}
double GFeatures::getDmax() {
	return this->dmax;
}
void GFeatures::setAngle(double angle) {
	this->angle = angle;
}
void GFeatures::setDmin(double dmin) {
	this->dmin = dmin;
}
void GFeatures::setDmax(double dmax) {
	this->dmax = dmax;
}
} /* namespace impls_2015 */
