/*
 * PairwiseHistogram.cpp
 *
 *  Created on: Aug 24, 2015
 *      Author: linh
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
