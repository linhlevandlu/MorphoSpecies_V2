/*
 * Image.h
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

#ifndef IMAGE_H_
#define IMAGE_H_

#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include <QtCore/qlist.h>
#include <QtCore/qstring.h>
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
#include "Landmark.h"

namespace impls_2015 {

class Image: public IDrawOperation {
private:
	QString fileName;
	vector<Edge> listOfEdges;
	//QList<Landmark> listOfLandmarks;
	int histogramSize;
public:
	enum InsectPart {
		ELYTRE, MDROITE, MGAUCHE, PRONOTUM, TETE
	};

	Image();
	Image(QString filePath);
	virtual ~Image();

	QString getFileName();
	void setFileName(QString filePath);
	vector<Edge> getEdges();
	void setEdges(vector<Edge> edges);
	//QList<Landmark> getLandmarks();
	//void setLandmarks(QList<Landmark> landmarks);
	int getHistSize();
	void setHistSize(int histSize);

	cv::Mat getMatImage();
	void addEdge(Edge edge);
	//void addLandmark(Landmark landmark);

	cv::Mat histogram();
	double medianHistogram();
	float meanHistogram();
	cv::Mat drawingHistogram();
	cv::Mat drawing(cv::Mat outputImage);

	// removing yellow grid
	InsectPart getPart();
	vector<cv::Mat> splitChannels();
	cv::Point findLimitPoint();
	cv::Point findReplacePoint();
	cv::Mat removingGrid(int minBrightness);
};

} /* namespace impls_2015 */
#endif /* IMAGE_H_ */
