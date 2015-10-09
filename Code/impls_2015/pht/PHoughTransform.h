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

using namespace std;
using namespace cv;

namespace impls_2015 {

class PHoughTransform: public HoughMethod {
public:
	PHoughTransform();
	virtual ~PHoughTransform();

	static void test(vector<Line> refLines, vector<Line> sceneLines, int width,
			int height);

	bool closetLine(Line line1, Line line2);
	vector<PHTEntry> constructTable(vector<Line> lines, Point refPoint);
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

	vector<vector<int> > createAccumulator(Point refPoint,
			vector<Line> sceneLines, int width, int height, int &maxValue);
	vector<HoughSpace> peaksOfAccumulator(vector<vector<int> > accumulator,
			int maxValue);

	vector<TableEntry> referenceTable(vector<Line> refLines, Point refPoint);
	bool similarLine(Line line1, Line line2, double &diff);
	TableEntry matchingEntry(vector<TableEntry> entryTable, Line sceneLine);
	vector<TableEntry> matchingInScene(vector<TableEntry> entryTable,
			vector<Line> sceneLines, int width, int height);
};

} /* namespace impls_2015 */
#endif /* PHOUGHTRANSFORM_H_ */
