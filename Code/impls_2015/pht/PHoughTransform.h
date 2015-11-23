/*
 * PHoughTransform.h
 *
 *  Created on: Sep 23, 2015
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

#ifndef PHOUGHTRANSFORM_H_
#define PHOUGHTRANSFORM_H_


#include <QtGui/QSlider>
#include <QtCore/QDebug>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <time.h>

#include "../Line.h"
#include "../Image.h"
#include "HoughMethod.h"
#include "PHTEntry.h"
#include "../ReadResouces.h"

using namespace std;
using namespace cv;

namespace impls_2015 {

class PHoughTransform: public HoughMethod {
public:
	map<string,int> resources;
	PHoughTransform();
	virtual ~PHoughTransform();

	bool closetLine(Line line1, Line line2);
	vector<PHTEntry> constructTable(vector<Line> lines, Point refPoint);
	bool lineSimilar(Line line1, Line line2);
	bool similarPairLines(Line ref1, Line ref2, Line scene1, Line scene2);
	PHTEntry findHoughSpace(vector<PHTEntry> entryTable, Line line1,
			Line line2);
	PHTEntry matchingInScene(vector<PHTEntry> entryTable,
			vector<Line> sceneLines, int width, int height,
			vector<Line> &maxVector);
	Point refPointInScene(PHTEntry entry, vector<Line> matchLines,
			double &angleDiff, vector<Point> refLandmarks, int width,
			int height);
	vector<Point> findLandmarks(Point refPoint, Point esPoint,
			vector<Point> refLandmarks, int width, int height, int &positive);
	vector<Point> estimateLandmarks(Image mImage, Image sImage, string mlmPath,
			double &angleDiff, Point &ePoint, Image::SegmentMethod sgmethod);
	Mat phtPresentation(Image refImage, Image sceneImage, string reflmPath,
			vector<Point> &esLandmarks, Image::SegmentMethod sgmethod);
	void phtDirectory(Image refImage, string reflmPath, string sceneDir,
			string scenelmDir, string saveDir, Image::SegmentMethod sgmethod,
			int save);
	double angleDifference(Line refLine, Line sceneLine);
	Point newLocation(Point point, double angleDiff, Point refPoint);
	void saveEstLandmarks(vector<Point> esLandmarks, string savePath);
};

} /* namespace impls_2015 */
#endif /* PHOUGHTRANSFORM_H_ */
