/*
 * PWHistogram.cpp
 *
 *  Created on: Aug 27, 2015
 *      Author: linh
 */

#include "LocalHistogram.h"

namespace impls_2015 {

LocalHistogram::LocalHistogram() {
	maxDistance = 0;

}

LocalHistogram::~LocalHistogram() {
	// TODO Auto-generated destructor stub
}
vector<GFeatures> LocalHistogram::getPWHistgoram() {
	return this->pwHistogram;
}
void LocalHistogram::setpwHistogram(vector<GFeatures> gfeatures) {
	this->pwHistogram = gfeatures;
}
double LocalHistogram::getMaxDistance() {
	return this->maxDistance;
}
void LocalHistogram::addGFeatures(GFeatures features) {
	pwHistogram.push_back(features);
	if (features.getDmax() > maxDistance)
		maxDistance = features.getDmax();
}
int LocalHistogram::accuracyToTimeDegree(double angle, AccuracyPGH angleAcc) {
	int m = 60;
	switch (angleAcc) {
	case HaftDegree:
		m = 120;
		break;
	case Degree:
		m = 60;
		break;
	case TwoTimeDegree:
		m = 30;
		break;
	case FourTimeDegree:
		m = 15;
		break;
	case SixTimeDegree:
		m = 10;
		break;
	case TwelveTimeDegree:
		m = 5;
		break;
	case SixtyTimeDegree:
		m = 1;
		break;
	default:
		m = 60;
		break;
	}
	int minute = convertAngleToMinute(angle);
	double bins = minute / m;
	double bin;
	double mod = modf(bins, &bin);
	if (mod > 0)
		bin += 1;
	return bin;
}
int LocalHistogram::heightOfAngleAxis(AccuracyPGH angleAcc) {
	if (angleAcc == HaftDegree)
		return 90;
	return angleAcc * 180;
}
int LocalHistogram::distanceOffset(double maxDistance,double distance, int cols) {
	double binSize = maxDistance / cols;
	double bin = round(distance / binSize);
	return (bin > 0) ? (bin - 1) : bin;
}
int LocalHistogram::convertAngleToMinute(double angle) {
	double degree;
	modf(angle, &degree);
	int minute = (angle - degree) * 60;
	int second = (angle - degree - minute / 60) * 3600;
	if (second >= 30)
		minute += 1;
	return (degree * 60) + minute;

}
vector<vector<int> > LocalHistogram::constructorMatrix(AccuracyPGH angleAcc,
		int distanceAxis, int &totalEntries) {
	int height = heightOfAngleAxis(angleAcc);
	int width = 0;
	if (distanceAxis != 0)
		width = distanceAxis;
	vector<vector<int> > matrix;
	matrix.resize(height + 1);
	for (int i = 0; i <= height; ++i) {
		matrix[i].resize(width + 1, 0);
	}

	for (size_t j = 0; j < pwHistogram.size(); j++) {
		GFeatures features = pwHistogram.at(j);
		int dmin = distanceOffset(this->maxDistance,features.getDmin(),width);
		int dmax = distanceOffset(this->maxDistance,features.getDmax(),width);
		int angle = accuracyToTimeDegree(features.getAngle(), angleAcc);
		if (!isnan(angle)) {
			for (int k = dmin; k <= dmax; k++) {
				if(angle >=0 && k < width){
					matrix[angle][k] += 1;
					totalEntries++;
				}
			}
		}
	}
	return matrix;
}
double LocalHistogram::bhattacharyyaMetric(LocalHistogram sceneHistogram,
		int distanceAxis,AccuracyPGH angleAcc) {
	double result = 0;
	int refTotalEntries = 0;
	int sceneTotalEntries = 0;
	vector<vector<int> > refMatrix = this->constructorMatrix(angleAcc, distanceAxis,
			refTotalEntries);
	int w_Matrix = distanceAxis;
	int h_Matrix = heightOfAngleAxis(angleAcc);
	vector<vector<int> > sceneMatrix = sceneHistogram.constructorMatrix(
			angleAcc, distanceAxis, sceneTotalEntries);
	for (size_t i = 0; i < h_Matrix; i++) {
		for (int j = 0; j < w_Matrix; j++) {
			double value1 = sqrt(
					(double) refMatrix[i][j] / (double) refTotalEntries);
			double value2 = sqrt(
					(double) sceneMatrix[i][j] / (double) sceneTotalEntries);
			result += value1 * value2;
		}
	}
	return result;
}
} /* namespace impls_2015 */
