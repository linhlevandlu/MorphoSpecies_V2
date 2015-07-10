/*
 Image processing for morphometrics (IPM)
 Copyright (C) 2014  Nguyen Hoang Thao (hoangthao.ng@gmail.com)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#ifndef IMAGECONVERT_H_
#define IMAGECONVERT_H_

#include <QtGui/QImage>
#include <opencv2/opencv.hpp>

class ImageConvert
{
public:
	static QImage convertGrayScale(QImage src);
	static QImage convertBinary(QImage src, int threshold=128);
	static QImage cvMatToQImage(cv::Mat inMat);
	static cv::Mat QImageToCvMat(QImage inImage, bool inCloneImageData = true );
};

#endif /* IMAGECONVERT_H_ */
