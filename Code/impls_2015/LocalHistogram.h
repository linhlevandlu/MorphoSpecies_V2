/*
 * PWHistogram.h
 *
 *  Created on: Aug 27, 2015
 *  Image processing for morphometrics (IPM) Version 0.2
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

#ifndef PWHISTOGRAM_H_
#define PWHISTOGRAM_H_


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

#include "GFeatures.h"
#include "Line.h"

using namespace cv;
using namespace std;

namespace impls_2015 {

class LocalHistogram {
public:
	enum AccuracyPGH {
		HaftDegree = 0,
		Degree = 1,
		TwoTimeDegree = 2,
		FourTimeDegree = 4,
		SixTimeDegree = 6,
		TwelveTimeDegree = 12,
		SixtyTimeDegree = 60
	};
private:
	vector<GFeatures> pwHistogram;
	static int convertAngleToMinute(double angle);
	double maxDistance;
public:
	LocalHistogram();
	virtual ~LocalHistogram();
	vector<GFeatures> getPWHistgoram();
	void setpwHistogram(vector<GFeatures> gfeatures);
	double getMaxDistance();
	void addGFeatures(GFeatures features);
	vector<vector<int> > constructorMatrix(AccuracyPGH angleAcc, int distanceAxis,int &totalEntries);
	double bhattacharyyaMetric(LocalHistogram sceneHistogram, int distanceAxis, AccuracyPGH angleAcc);

	static int accuracyToTimeDegree(double angle,
			AccuracyPGH angleAcc);
	static int heightOfAngleAxis(AccuracyPGH angleAcc);
	static int distanceOffset(double maxDistance, double distance, int cols);
};

} /* namespace impls_2015 */
#endif /* PWHISTOGRAM_H_ */
