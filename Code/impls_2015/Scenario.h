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
#include "ShapeHistogram.h"
#include "pht/PHoughTransform.h"
#include "landmarks/LandmarkDetection.h"

using namespace cv;
using namespace std;

namespace impls_2015 {

class Scenario {

public:

	Scenario();
	virtual ~Scenario();
	static vector<Line> edgeSegmentation(Image image, cv::Mat &result,
			Image::SegmentMethod sgmethod);
	static void edgeSegmentationDirectory(QString inputFolder,
			QString saveFolder, Image::SegmentMethod segmentMethod, int save);
	static vector<LocalHistogram> pairwiseHistogram(Image image,
			LocalHistogram::AccuracyPGH angleAcc, int columns,
			Image::SegmentMethod sgmethod, cv::Mat &result,
			vector<vector<int> > &matrix);
	static vector<Point> landmarksByCrossCorelation(Image image, QString lpath,
			Image sceneImage, int tsize);
	static double mDistanceByCrossCorrelation(Image image, QString lmpath,
			Image sceneImage, int size, Point &ebary);
	static void mDistanceByCrossCorrelationDir(Image refImage, QString lmpath,
			QString imgFolder);
	static Mat landmarksMatching(Image refImage, Image sceneImage,
			QString reflmPath, int templSize, int scnSize,
			Image::SegmentMethod sgmethod, vector<Point> &mcResult,
			double &angle, Point &ePoint);
	static void landmarksMatchingDirectory(Image refImage, QString folderImages,
			QString lmPath, QString savePath, int templSize, int sceneSize,
			Image::SegmentMethod sgmethod, int save);
	static void cCorelationDirectory(Image refImage, QString path,
			QString savePath, QString lmPath);
	static double pghMatching(Image refImage, Image sceneImage,
			GeometricHistogram::MatchingMethod matching,
			LocalHistogram::AccuracyPGH angleAcc, int distanceAcc,
			Image::SegmentMethod sgmethod);
	static void matchingDirectory(Image refImage, QString directoryPath,
			GeometricHistogram::MatchingMethod matching,
			LocalHistogram::AccuracyPGH angleAcc, int distanceAcc,
			QString fileSave, Image::SegmentMethod sgmethod);
	static void matchingDirectory(QString directoryPath,
			GeometricHistogram::MatchingMethod matching,
			LocalHistogram::AccuracyPGH angleAcc, int distanceAcc);
	static void pairwiseHistogramDirectory(QString folderPath,
			LocalHistogram::AccuracyPGH angleAcc, int columns);
	static Mat probabilisticHoughTransform(Image refImage, Image sceneImage,
			QString reflmPath, Image::SegmentMethod sgmethod,
			vector<Point> &esLandmarks);
	static void phtDirectory(Image refImage, QString reflmPath,
			QString sceneDir, QString scenelmDir, QString saveDir,
			impls_2015::Image::SegmentMethod sgmethod, int save);

	static double mDistanceByTemplateMatching(Image refImage, Image sceneImage,
			QString lmPath, int templSize, int sceneSize,
			Image::SegmentMethod sgmethod, Point &ebary);
	static void mDistanceByTemplateMatchingDirectory(Image refImage,
			QString lmPath, QString folderImages, QString lmFolder,
			int templSize, int sceneSize, QString savePath,
			Image::SegmentMethod sgmethod);
	static Mat loadOriginalLandmarks(Image image, QString lmPath,
			vector<Point> &orgLandmarks);
};

} /* namespace impls_2015 */
#endif /* SCENARIO_H_ */
