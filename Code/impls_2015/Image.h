/*
 * Image.h
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

#ifndef IMAGE_H_
#define IMAGE_H_

/*
#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include <QtCore/qlist.h>
*/
#include <QtCore/qstring.h>
#include <QtCore/qdir.h>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

#include "Edge.h"
#include "ShapeHistogram.h"
#include "ReadResouces.h"

using namespace std;
using namespace cv;

namespace impls_2015 {

class Image: public IDrawOperation {
private:
	string fileName;
	vector<Edge> listOfEdges;
	vector<Point> listOfLandmarks;
	int histogramSize;
	cv::Mat matrixImage;
	ShapeHistogram pghHistogram;
	cv::Mat getMatImage();
public:
	enum SegmentMethod{
		Otsu = 0,
		Other = 1
	};
	enum InsectPart {
		ELYTRE, MDROITE, MGAUCHE, PRONOTUM, TETE
	};
	Image();
	Image(string filePath);
	virtual ~Image();

	string getFileName();
	void setFileName(string filePath);
	vector<Edge> getEdges(int thresholdValue);
	void setEdges(vector<Edge> edges);
	int getThresholdValue_Otsu();
	int getThresholdValue();
	int thresholdBadFiles();
	int getHistSize();
	void setHistSize(int histSize);
	cv::Mat getMatrixImage();
	void addEdge(Edge edge);
	//void addLandmark(Landmark landmark);

	cv::Mat histogram();
	double medianHistogram();
	float meanHistogram();
	cv::Mat drawingHistogram();
	void drawing(cv::Mat &outputImage);

	// removing yellow grid
	InsectPart getPart();
	vector<cv::Mat> splitChannels();
	cv::Point findLimitPoint();
	cv::Point findReplacePoint();
	cv::Mat removingGrid(int minBrightness);

	vector<Line> lineSegment();
	vector<Line> lineSegment(SegmentMethod method,int &thresholdValue);
	vector<Line> getApproximateLines(vector<Edge> edges);
	void setShapeHistogram(ShapeHistogram shapeHistogram);
	ShapeHistogram getShapeHistogram();
	vector<Point> readLandmarksFile(string filePath);


	static QFileInfoList readImagesFolder(QString folderPath);
	QString getName();
	static QString getTimeName();
	static Mat rotateImage(Mat source, double angle, Point center);
	Mat loadOriginalLandmarks(Mat matImage, QString lmPath,vector<Point> &orgLandmarks);
	void drawSegment(Mat &output, vector<Line> lines);
};

} /* namespace impls_2015 */
#endif /* IMAGE_H_ */
