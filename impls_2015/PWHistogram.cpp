/*
 * PWHistogram.cpp
 *
 *  Created on: Aug 27, 2015
 *      Author: linh
 */

#include "PWHistogram.h"

namespace impls_2015 {

PWHistogram::PWHistogram() {
	// TODO Auto-generated constructor stub

}

PWHistogram::~PWHistogram() {
	// TODO Auto-generated destructor stub
}
void PWHistogram::setLine(Line refLine) {
	this->refLine = refLine;
}
Line PWHistogram::getLine() {
	return this->refLine;
}
vector<GFeatures> PWHistogram::getPWHistgoram() {
	return this->pwHistogram;
}
void PWHistogram::setpwHistogram(vector<GFeatures> gfeatures) {
	this->pwHistogram = gfeatures;
}
void PWHistogram::addGFeatures(GFeatures features) {
	pwHistogram.push_back(features);
}
} /* namespace impls_2015 */
