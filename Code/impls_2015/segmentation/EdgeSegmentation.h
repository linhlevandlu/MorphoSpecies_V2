/*
 * EdgeSegmentation.h
 *
 *  Created on: Aug 13, 2015
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

#ifndef EDGESEGMENTATION_H_
#define EDGESEGMENTATION_H_

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


#include "../Line.h"
#include "../Image.h"
#include "../Edge.h"
#include "../GFeatures.h"
#include "../LocalHistogram.h"
#include "SegmentMethod.h"

using namespace cv;
using namespace std;

namespace impls_2015 {

class EdgeSegmentation: public SegmentMethod {

public:

	EdgeSegmentation();
	//vector<Edge> getEdges(Image inputImage);
	vector<Line> lineSegment(Image image, Image::SegmentMethod sgmethod);
	vector<Line> postLineSegment(Image image, Image::SegmentMethod sgmethod);
	cv::Mat rePresentation(cv::Mat resultImage, vector<Line> lines);
	void segmentDirectory(QString inputFolder,QString saveFolder,Image::SegmentMethod method, int save);
	void savePGHFile(vector<Line> lines, QString savePath);
};

} /* namespace impls_2015 */
#endif /* EDGESEGMENTATION_H_ */
