/*
 * EdgeSegmentation.cpp
 *
 *  Created on: Aug 13, 2015
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

#include "EdgeSegmentation.h"
#include <QtCore/QDebug>
#include <QtCore/qqueue.h>

namespace impls_2015 {

/**
 * Constructor an edgeSegmentation
 */
EdgeSegmentation::EdgeSegmentation() {
}

/*
 * Segment the image into a list of approximate lines
 * @parameter: image - the input image
 * @return: list of approximate lines
 */
vector<Line> EdgeSegmentation::lineSegment(Image image,
		Image::SegmentMethod sgmethod) {
	int thresh = 0;

	//clock_t t1, t2;
//	t1 = clock();
	vector<Line> lines = image.lineSegment(sgmethod, thresh);
	//t2 = clock();
	//qDebug() << "time to segmentation: "
	//		<< ((float) t2 - (float) t1) / CLOCKS_PER_SEC << " seconds";
	return lines;
}

/*
 * Presentation the result of segmentation
 * @parameter 1: resultImage - the result image (on Matrix)
 * @parameter 2: lines - list of approximate lines
 * @return: the image present for list of approximate lines
 */
cv::Mat EdgeSegmentation::rePresentation(cv::Mat resultImage,
		vector<Line> lines) {
	for (size_t i = 0; i < lines.size(); i++) {
		Line line = lines.at(i);
		line.drawing(resultImage);
	}
	return resultImage;
}

/*
 * Segment the images in a directory
 * @parameter 1: inputPath - the images folder
 * @parameter 2: savePath - the ouput folder
 */
void EdgeSegmentation::segmentDirectory(QString inputPath, QString savePath,
		Image::SegmentMethod method, int save) {
	QFileInfoList files = Image::readImagesFolder(inputPath);
	QString spath = savePath + "/thresholdValues.txt";
	ofstream of(spath.toStdString().c_str());

	clock_t t1, t2;
	t1 = clock();

	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image image(_name.toStdString());
		qDebug() << _name;
		int tvalue = 0;
		vector<Line> lines = image.lineSegment(method, tvalue);
		of << image.getName().toStdString().c_str() << "\t" << tvalue << "\n";

		// save PGH files
		QString pghName = savePath + "/" + image.getName() + ".PGH";
		if (save == 1)
			savePGHFile(lines, pghName);

		//save the images
		cv::Mat segImg(image.getMatrixImage().clone());
		segImg = rePresentation(segImg, lines);
		QString path = savePath + "/" + image.getName() + ".JPG";
		imwrite(path.toStdString().c_str(), segImg);
	}
	t2 = clock();
	of << "time to segmentation: " << ((float) t2 - (float) t1) / CLOCKS_PER_SEC
			<< " seconds";
	of.close();
}
void EdgeSegmentation::savePGHFile(vector<Line> lines, QString savePath) {
	int x1, y1, x2, y2;
	ofstream of(savePath.toStdString().c_str());
	for (size_t i = 0; i < lines.size(); i++) {
		Line line = lines.at(i);
		x1 = line.getP1().x;
		y1 = line.getP1().y;
		x2 = line.getP2().x;
		y2 = line.getP2().y;
		of << x1 << "," << y1 << "," << x2 << "," << y2 << "\n";
	}
	of.close();
}

void EdgeSegmentation::saveSegmentation(Image image,
		Image::SegmentMethod sgmethod, QString savePath) {
	vector<Line> lines = lineSegment(image,sgmethod);
	savePGHFile(lines,savePath);
}
} /* namespace impls_2015 */
