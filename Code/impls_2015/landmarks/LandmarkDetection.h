/*
 * LandmarkDetection.h
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

#ifndef LANDMARKDETECTION_H_
#define LANDMARKDETECTION_H_

#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include <QtCore/qqueue.h>
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
#include <QtGui/QFileDialog>
#include <QtCore/qdir.h>
#include <ostream>

#include "LandmarkMethod.h"
#include "../Image.h"
#include "../Landmark.h"
#include "../pht/PHoughTransform.h"


using namespace cv;
using namespace std;

namespace impls_2015 {

class LandmarkDetection: public LandmarkMethod {
private:
	double noise;
public:
	LandmarkDetection();
	virtual ~LandmarkDetection();
	void setNoise(double noise);
	vector<Landmark> getLandmarks(Image image);
	Mat createTemplate(Mat matImage, Point landmark,int tsize, Point &location,Point &distance);
	Point crossCorrelation(Mat templ, Mat scene) ;
	vector<Point> crossCorrelation(Image refImage,Image sceneImage, QString lmPath,int templSize);
	void landmarksByDirectory(Image refImage, QString path,QString savepath,QString lmPath);
	double centroid(Image refImage, Image sceneImage, QString lmPath, QString slmPath, Point &center);
	void centroidFolder(Image refImage, QString lmPath,
			QString folderImages, QString folderlandmarks);
	Mat rotateImage(Mat source, double angle);
	Mat matchingTemplate(Image refImage, Image sceneImage,
			QString lmPath, int templSize, int sceneSize, double angleDiff);
	void matchingDirectory(Image refImage, QString folderImages,
			QString lmPath, QString savePath, int templSize, int sceneSize, double angleDiff);
};

} /* namespace impls_2015 */
#endif /* LANDMARKDETECTION_H_ */
