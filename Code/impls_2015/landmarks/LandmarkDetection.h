/*
 * LandmarkDetection.h
 *
 *  Created on: Aug 21, 2015
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

#ifndef LANDMARKDETECTION_H_
#define LANDMARKDETECTION_H_

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <QtCore/QDebug>
#include <QtCore/qlist.h>
#include <fstream>
#include <QtCore/qdir.h>
#include <ostream>

#include "LandmarkMethod.h"
#include "../Image.h"
#include "../pht/PHoughTransform.h"

using namespace cv;
using namespace std;

namespace impls_2015 {

class LandmarkDetection: public LandmarkMethod {
public:
	LandmarkDetection();
	virtual ~LandmarkDetection();
	Mat createTemplate(Mat matImage, Point landmark, int tsize, Point &location,
			Point &distance);
	Point matCrossCorrelation(Mat templ, Mat scene);
	vector<Point> crossCorrelation(Image refImage, Image sceneImage,
			QString lmPath, int templSize);
	void cCorrelationByDirectory(Image refImage, QString folderpath,
			QString savepath, QString lmPath);
	double centroidCCorrelation(Image refImage, Image sceneImage,
			QString lmPath, int size, Point &ebary);
	void centroidCCorrelations(Image refImage, QString lmPath,
			QString folderImages);
	void centroidCCorrelations(Image refImage, QString lmPath,
			QString folderImages, QString folderlandmarks);
	//Mat rotateImage(Mat source, double angle, Point center);
	Mat matchingTemplate(Image refImage, Image sceneImage, QString lmPath,
			int templSize, int sceneSize, double &angleDiff,
			Image::SegmentMethod sgmethod, vector<Point> &mcResult, Point &ePoint);
	void matchingDirectory(Image refImage, QString folderImages, QString lmPath,
			QString savePath, int templSize, int sceneSize, double angleDiff,
			Image::SegmentMethod sgmethod, int save);
	double measureDistance(vector<Point> landmarks, Point &bary);
	double measureDistance(vector<Point> refLandmarks,
			vector<Point> esLandmarks, double &mCentroid, double &eCentroid,
			Point &mBary, Point &eBary);
	void centroidMatchingDirectory(Image refImage, QString lmPath,
			QString folderImages, QString folderlandmarks, int templSize,
			int sceneSize, QString savePath, Image::SegmentMethod sgmethod);
	double centroidMatching(Image refImage, Image sceneImage, QString lmPath,
			int templSize, int sceneSize,Image::SegmentMethod sgmethod, Point &ebary);
	void centroidMatchingDirectory(Image refImage, QString lmPath,
			QString folderImages, int templSize, int sceneSize,
			QString savePath, Image::SegmentMethod sgmethod);
	void save_Estimated_Landmarks_To_File(vector<Point> esLandmarks,
			QString path);
	void save_Original_And_Estimated_Landmarks_To_File(
			vector<Point> orgLandmarks, vector<Point> esLandmarks,
			QString path);
	Mat loadOriginalLandmarks(Image image, QString lmPath,vector<Point> &orgLandmarks);
};

} /* namespace impls_2015 */
#endif /* LANDMARKDETECTION_H_ */
