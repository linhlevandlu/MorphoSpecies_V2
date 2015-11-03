/*
 * PWHistogram.cpp
 *
 *  Created on: Aug 27, 2015
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

#include "LocalHistogram.h"

namespace impls_2015 {

LocalHistogram::LocalHistogram() {
	maxDistance = 0;

}

LocalHistogram::~LocalHistogram() {
	// TODO Auto-generated destructor stub
}

/*
 * Get the list of feature information(angle, dmin, dmax)
 * @return: list of features information
 */
vector<GFeatures> LocalHistogram::getPWHistgoram() {
	return this->pwHistogram;
}

/*
 * Set the list of feature information
 * @parameter: gfeatures - list of feature information
 */
void LocalHistogram::setpwHistogram(vector<GFeatures> gfeatures) {
	this->pwHistogram = gfeatures;
}

/*
 * Get the maximum distance in local PGH
 * @return: the maximum distance
 */
double LocalHistogram::getMaxDistance() {
	return this->maxDistance;
}

/*
 * Add a feature information into the list
 * @parameter: features - the feature information
 */
void LocalHistogram::addGFeatures(GFeatures features) {
	pwHistogram.push_back(features);
	if (features.getDmax() > maxDistance)
		maxDistance = features.getDmax();
}

/*
 * Indicate the row in matrix of an angle with an angle accuracy
 * @parameter 1: angle - the angle
 * @parameter 2: angleAcc - the angle accuracy
 * @return: the order of row, where the angle located in PGH
 */
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
	int minutes = convertAngleToMinute(angle);
	double bins = minutes / m;
	double bin = 0;
	double mod = modf(bins, &bin);
	if (mod > 0)
		bin += 1;
	return bin;
}

/*
 * Get the height of angle axis followed the angle accuracy
 * @parameter: angleAcc - the angle accuracy
 * @return: the height of matrix, which presented PGH
 */
int LocalHistogram::heightOfAngleAxis(AccuracyPGH angleAcc) {
	if (angleAcc == HaftDegree)
		return 90;
	return angleAcc * 180;
}

/*
 * Indicate the column of a distance followed distance accuracy
 * @parameter 1: maxDistance - the maximum distance
 * @parameter 2: distance - the distance
 * @parameter 3: cols - the distance accuracy
 * @return: the position of distance in matrix (which presented PGH)
 */
int LocalHistogram::distanceOffset(double maxDistance,double distance, int cols) {
	double binSize = maxDistance / cols;
	double bin = round(distance / binSize);
	return (bin > 0) ? (bin - 1) : bin;
}

/*
 * Convert the angle into minute
 * @parameter: angle - the angle
 * @return: the minute of angle after converting
 */
int LocalHistogram::convertAngleToMinute(double angle) {
	double degree;
	modf(angle, &degree);
	int minute = (angle - degree) * 60;
	int second = (angle - degree - minute / 60) * 3600;
	if (second >= 30)
		minute += 1;
	return (degree * 60) + minute;

}

/*
 * Construct the matrix of a local PGH
 * @parameter 1: angleAcc - the angle accuracy
 * @parameter 2: distanceAxis - the distance accuracy
 * @parameter 3: totalEntries - the total entries in matrix
 * @return: the matrix presented for PGH
 */
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

/*
 * Compute the measure distance between two local PGHs by Bhattacharrya metric
 * @parameter 1: sceneHistogram - the scene local PGH
 * @parameter 2: distanceAxis - the distance accuracy
 * @parameter 3: angleAcc - the angle accuracy
 * @return: the measure distance between two local PGH
 */
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
	for (int i = 0; i < h_Matrix; i++) {
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
