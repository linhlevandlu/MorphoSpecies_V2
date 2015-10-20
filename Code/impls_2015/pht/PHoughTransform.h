/*
 * PHoughTransform.h
 *
 *  Created on: Sep 23, 2015
 *      Author: linh
 */

#ifndef PHOUGHTRANSFORM_H_
#define PHOUGHTRANSFORM_H_

#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include <QtGui/QDialog>
#include <QtGui/QWidget>
#include <QtGui/QSlider>
#include <QtCore/qqueue.h>
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
#include "TableEntry.h"
#include "../landmarks/LandmarkDetection.h"

using namespace std;
using namespace cv;

namespace impls_2015 {

class PHoughTransform: public HoughMethod {
public:
	PHoughTransform();
	virtual ~PHoughTransform();

	static void test(vector<Line> refLines, vector<Line> sceneLines,
			QString refLandmark, int width, int height);

	bool closetLine(Line line1, Line line2);
	vector<PHTEntry> constructTable(vector<Line> lines, Point refPoint);
	bool lineSimilar(Line line1, Line line2);
	bool similarPairLines(Line ref1, Line ref2, Line scene1, Line scene2);
	PHTEntry findHoughSpace(vector<PHTEntry> entryTable, Line line1,
			Line line2);
	PHTEntry matchingInScene(vector<PHTEntry> entryTable,
			vector<Line> sceneLines, int width, int height,
			vector<Line> &maxVector);
	Point refPointInScene(Image modelImage, Image sceneImage);
	Point refPointInScene(PHTEntry entry, vector<Line> matchLines,
			double &angleDiff, int width, int height);
	vector<Point> readLandmarksFile(QString filePath);

	vector<Point> findLandmarks(Point refPoint, Point esPoint,
			double angleDiff, vector<Point> refLandmarks, int width,
			int height);
	vector<Point> estimateLandmarks(Image mImage, Image sImage,
			string mlmPath, double &angleDiff,Point &ePoint);
	Mat phtPresentation(Image refImage, Image sceneImage, string reflmPath);
	void phtDirectory(Image refImage, QString reflmPath, QString sceneDir,
			QString scenelmDir, QString saveDir);
	double angleDifference(Line refLine, Line sceneLine);
	Point newLocation(Point point, double angleDiff,
			Point refPoint);
};

} /* namespace impls_2015 */
#endif /* PHOUGHTRANSFORM_H_ */
