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

using namespace std;
using namespace cv;

namespace impls_2015 {

class PHoughTransform: public HoughMethod {
private:
	//Point refPoint;
	vector<PHTEntry> accTable;
public:
	PHoughTransform();
	virtual ~PHoughTransform();
	static cv::Mat pHoughTransform(cv::Mat inputImage);
	static void test(vector<Line> refLines, vector<Line> sceneLines,int width, int height);

	bool similarPairLines(Line ref1, Line ref2, Line scene1, Line scene2);
	PHTEntry findHoughSpace(vector<PHTEntry> entryTable, Line line1,
			Line line2);
	void addPHTEntry(PHTEntry entry);
	vector<PHTEntry> constructTable(vector<Line> lines, Point refPoint);
	vector<vector<int> > createAccumulator(Point refPoint, vector<Line> sceneLines,int width, int height, int &maxValue);
	vector<vector<int> > createAccumulator2(Point refPoint,
			vector<Line> sceneLines, int width, int height, int &maxValue);

	Point refPointInScene(vector<PHTEntry> tableEntry, vector<Line> sceneLines,
			int width, int height);
	Point refPointInScene(Image modelImage, Image sceneImage);
	vector<HoughSpace> peaksOfAccumulator(vector<vector<int> > accumulator,int maxValue);
};

} /* namespace impls_2015 */
#endif /* PHOUGHTRANSFORM_H_ */
