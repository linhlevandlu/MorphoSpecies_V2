/*
 * EdgeSegmentation.cpp
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

#include "EdgeSegmentation.h"
#include <QtCore/QDebug>
#include <QtCore/qqueue.h>

namespace impls_2015 {

/**
 * Constructor an edgeSegmentation
 */
EdgeSegmentation::EdgeSegmentation() {
}

vector<Line> EdgeSegmentation::lineSegment(Image image) {
	return image.lineSegment();
}
cv::Mat EdgeSegmentation::rePresentation(cv::Mat resultImage,
		vector<Line> lines) {
	for (size_t i = 0; i < lines.size(); i++) {
		Line line = lines.at(i);
		line.drawing(resultImage);
	}
	return resultImage;
}
void EdgeSegmentation::segmentDirectory(QString inputPath, QString savePath) {
	QFileInfoList files = Image::readImagesFolder(inputPath);

	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image image(_name);

		qDebug() << _name;
		int index2 = _name.lastIndexOf("/");
		QString scenename =
				_name.mid(index2 + 1, _name.length() - index2 - 5).replace(" ",
						"");
		QString pghName = savePath + scenename + ".PGH";
		vector<Line> lines = image.lineSegment();

		//write the coordinates
		/*int x1, y1, x2, y2;
		 ofstream of(pghName.toStdString().c_str());
		 for (size_t i = 0; i < lines.size(); i++) {
		 Line line = lines.at(i);
		 // rotation 15 degree
		 x1 = line.getP1().x * cos(round(15) * M_PI / 180)
		 - line.getP1().y * sin(round(15) * M_PI / 180);
		 y1 = line.getP1().x * sin(round(15) * M_PI / 180)
		 + line.getP1().y * cos(round(15) * M_PI / 180);
		 x2 = line.getP2().x * cos(round(15) * M_PI / 180)
		 - line.getP2().y * sin(round(15) * M_PI / 180);
		 y2 = line.getP2().x * sin(round(15) * M_PI / 180)
		 + line.getP2().y * cos(round(15) * M_PI / 180);
		 of << "(" << x1 << "," << y1 << ")"
		 << " (" << x2 << "," << y2
		 << ")" << "\n";
		 }
		 of.close();*/

		//save the image
		/*cv::Mat segImg(image.getMatrixImage().size(),image.getMatrixImage().type(),Scalar::all(0));
		 segImg = rePresentation(segImg,lines);
		 QString path = savePath + scenename + ".JPG";
		 imwrite(path.toStdString().c_str(),segImg);*/
	}
}
} /* namespace impls_2015 */
