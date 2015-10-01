/*
 * Scenario.h
 *
 *  Created on: Aug 21, 2015
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

#ifndef SCENARIO_H_
#define SCENARIO_H_

#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include <QtCore/qlist.h>
#include <QtGui/QFileDialog>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <time.h>

#include "Line.h"
#include "Image.h"
#include "Edge.h"
#include "segmentation/EdgeSegmentation.h"
#include "histograms/GeometricHistogram.h"
#include "IExtraction.h"
#include "ShapeHistogram.h"
#include "pht/PHoughTransform.h"

using namespace cv;
using namespace std;

namespace impls_2015 {

class Scenario {

private:
	//IExtraction* extraction;
	//vector<Line> segment(Image image);
	//ShapeHistogram pwHistogram(Image image);
public:

	Scenario();
	virtual ~Scenario();
	static vector<Line> edgeSegmentation(Image image,cv::Mat &result);
	static void edgeSegmentationDirectory(QString inputFolder,QString saveFolder);
	static vector<LocalHistogram> pairwiseHistogram(Image image,LocalHistogram::AccuracyPGH angleAcc, int columns, cv::Mat &result);
	static vector<Landmark> landmarksAutoDetect(Image image);
	static double pghMatching(Image refImage, Image sceneImage, GeometricHistogram::MatchingMethod matching, LocalHistogram::AccuracyPGH angleAcc);
	static void matchingDirectory(Image refImage, QString directoryPath, GeometricHistogram::MatchingMethod matching, LocalHistogram::AccuracyPGH angleAcc);
	static void pairwiseHistogramDirectory(QString folderPath,LocalHistogram::AccuracyPGH angleAcc, int colums);
	static void probabilisticHoughTransform(vector<Line> refLines, vector<Line> sceneLines,int width, int height);
};

} /* namespace impls_2015 */
#endif /* SCENARIO_H_ */
