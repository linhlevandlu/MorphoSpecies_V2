/*
 * GeometricHistogram.h
 *
 *  Created on: Sep 17, 2015
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

#ifndef GEOMETRICHISTOGRAM_H_
#define GEOMETRICHISTOGRAM_H_
#include <QtCore/qlist.h>
#include <QtGui/QFileDialog>
#include <QtCore/qdir.h>
#include <QtCore/qstringlist.h>
#include <iostream>
#include <time.h>

#include "../Image.h"
#include "../Line.h"
#include "HistogramMethod.h"

using namespace std;
using namespace cv;

namespace impls_2015 {

class GeometricHistogram: public HistogramMethod {
public:
	enum MatchingMethod {
		Bhattacharyya, Chisquared, Intersection
	};
private:
	double getDistanceMetric(ShapeHistogram refHist, ShapeHistogram sceneHist,
			MatchingMethod matching);
public:
	GeometricHistogram();
	virtual ~GeometricHistogram();
	vector<LocalHistogram> shapeHistogram(Image image,
			LocalHistogram::AccuracyPGH angleAcc, int distanceAcc,
			Image::SegmentMethod sgmethod, cv::Mat &result, vector<vector<int> > &matrix);
	double pghHistogramMatching(Image refImage, Image sceneImage,
			MatchingMethod matchingMethod, LocalHistogram::AccuracyPGH angleAcc,
			int distanceAcc,Image::SegmentMethod sgmethod);
	void pghHistogramDirectoryMatching(Image refImage, string folderPath,
			MatchingMethod matchingMethod, LocalHistogram::AccuracyPGH angleAcc,
			int distanceAcc, string fileSave, Image::SegmentMethod sgmethod);
	void phgHistogramDirMatching(string folderPath, MatchingMethod method,
			LocalHistogram::AccuracyPGH angleAcc, int distanceAcc);
	void pairwiseHistogramDirectory(string folderPath,
			LocalHistogram::AccuracyPGH angleAcc, int distanceAcc);

};

} /* namespace impls_2015 */
#endif /* GEOMETRICHISTOGRAM_H_ */
