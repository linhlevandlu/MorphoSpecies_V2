/*
 * ShapeHistogram.h
 *
 *  Created on: Sep 1, 2015
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

#ifndef SHAPEHISTOGRAM_H_
#define SHAPEHISTOGRAM_H_

#include <QtCore/QDebug>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <time.h>

#include "IDrawOperation.h"
#include "GFeatures.h"
#include "LocalHistogram.h"
#include "Edge.h"

using namespace cv;
using namespace std;

namespace impls_2015 {

class ShapeHistogram {

private:
	double totalEntries;
	double max_distance;
	vector<vector<int> > matrix;
	vector<LocalHistogram> listLocalHistogram;
	int heightAngleAxis(LocalHistogram::AccuracyPGH angleAcc);
	int angleOffset(double angle, LocalHistogram::AccuracyPGH angleAcc);
public:
	ShapeHistogram();
	//ShapeHistogram(vector<Line> lines);
	virtual ~ShapeHistogram();
	double getTotalEntries();
	void setTotalEntries(double entries);
	double getMaxDistance();
	void setMaxDistance(double distance);
	void setMatrix(vector<vector<int> > matrix);
	vector<vector<int> > getMatrix();
	vector<LocalHistogram> getListLocalHistogram();

	cv::Mat presentation(vector<LocalHistogram> pghHistograms,
			LocalHistogram::AccuracyPGH angleAcc, int cols);
	double bhattacharyaMetric(ShapeHistogram scenHist);
	double chiSquaredMetric(ShapeHistogram sceneHist);
	double intersectionMetric(ShapeHistogram sceneHist);

	vector<LocalHistogram> constructPGH(vector<Line> prLines);
	int distanceOffset(double distance, int cols);
	vector<vector<int> > constructMatPGH(vector<LocalHistogram> localHistograms,
			LocalHistogram::AccuracyPGH angleAcc, int cols);
	void writeMatrix(vector<vector<int> > matrixPGH, QString fileName);

};

} /* namespace impls_2015 */
#endif /* PGH_H_ */
