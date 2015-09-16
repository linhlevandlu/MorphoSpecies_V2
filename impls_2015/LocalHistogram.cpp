/*
 * PWHistogram.cpp
 *
 *  Created on: Aug 27, 2015
 *      Author: linh
 */

#include "LocalHistogram.h"

namespace impls_2015 {

LocalHistogram::LocalHistogram() {
	// TODO Auto-generated constructor stub

}

LocalHistogram::~LocalHistogram() {
	// TODO Auto-generated destructor stub
}
void LocalHistogram::setLine(Line refLine) {
	this->refLine = refLine;
}
Line LocalHistogram::getLine() {
	return this->refLine;
}
vector<GFeatures> LocalHistogram::getPWHistgoram() {
	return this->pwHistogram;
}
void LocalHistogram::setpwHistogram(vector<GFeatures> gfeatures) {
	this->pwHistogram = gfeatures;
}
void LocalHistogram::addGFeatures(GFeatures features) {
	pwHistogram.push_back(features);
}
vector<vector<int> > LocalHistogram::presentation() {
	int t_width = 0;
	int height = 180;
	// get the maximal distance
	for (size_t i = 0; i < this->pwHistogram.size(); i++) {
		GFeatures feature = pwHistogram[i];
		if (feature.getDmax() > t_width)
			t_width = round(feature.getDmax());
	}
	vector<vector<int> > matrixP;
	matrixP.resize(height + 1);
	for (int i = 0; i <= height; i++) {
		matrixP[i].resize(t_width + 11, 0);
	}

	for (size_t t = 0; t < pwHistogram.size(); t++) {
		GFeatures feature = pwHistogram[t];
		int x1 = round(feature.getDmin());
		int x2 = round(feature.getDmax());
		int y = accuracyToTimeDegree(feature.getAngle(),0);
		for (int k = x1; k <= x2; k++) {
			if (y >= 0)
				matrixP[y][k] += 1;
		}
	}
	return matrixP;
}
double LocalHistogram::matching(vector<vector<int> > sceneHist) {
	vector<vector<int> > pointer_ref = presentation();
	vector<vector<int> > pointer_scene = sceneHist;
	int max1 = pointer_ref[0].size();
	int max2 = pointer_scene[0].size();
	int distance_size = (max1 > max2) ? max1 : max2;
	if (max2 > max1) {
		for (int i = 0; i < 181; i++) {
			pointer_ref[i].resize(pointer_scene[0].size(), 0);
		}
	} else {
		for (int i = 0; i < 181; i++) {
			pointer_scene[i].resize(pointer_ref[0].size(), 0);
		}
	}

	double distance = 0;
	for (int i = 0; i < 181; i++) {
		for (int j = 0; j < distance_size; j++) {
			int value1 = pointer_ref[i][j];
			int value2 = pointer_scene[i][j];
			distance += sqrt(value1 * value2);
		}
	}

	return distance;
}

int LocalHistogram::accuracyToTimeDegree(double angle,int angleAcc){
	int m = 60;
	switch(angleAcc){
		case 1:
			m = 60;
			break;
		case 2:
			m = 30;
			break;
		case 4:
			m = 15;
			break;
		case 6:
			m = 10;
			break;
		case 12:
			m = 5;
			break;
		case 60:
			m = 1;
			break;
		default:
			m = 60;
			break;
	}
	int minute = convertAngleToMinute(angle);
	double bins = minute / m;
	double bin;
	double mod = modf(bins,&bin);
	if(mod >0)
		bin+=1;
	return bin;
}
int LocalHistogram::convertAngleToMinute(double angle){
	double degree;
	modf(angle,&degree);
	int minute = (angle - degree)* 60;
	int second = (angle - degree - minute/60) * 3600;
	if(second >= 30)
		minute += 1;
	return (degree * 60) + minute;

}
} /* namespace impls_2015 */
