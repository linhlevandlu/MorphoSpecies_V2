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
	Point refPoint;
	vector<PHTEntry> accTable;
public:
	PHoughTransform();
	virtual ~PHoughTransform();
	static cv::Mat pHoughTransform(cv::Mat inputImage);

	PHTEntry findHoughSpace(vector<PHTEntry> entryTable,
			Line line1, Line line2);
	void addPHTEntry(PHTEntry entry);
	vector<PHTEntry> constructTable(vector<Line> lines);
	vector<vector<int> > accumulator(Image refimage, Image sceneImage);
	int maxOfAccumulator(vector<vector<int> > accumulator);
	/*vector<Line> drawAccumulator(vector<vector<int> > accumulator, int threshold);*/
};

} /* namespace impls_2015 */
#endif /* PHOUGHTRANSFORM_H_ */
