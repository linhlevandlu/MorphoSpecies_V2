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

#include <QtCore/QDebug>
#include "ImageConvert.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"

QImage ImageConvert::convertGrayScale(QImage src){
	QRgb col;
	int gray;
	int width = src.width();
	int height = src.height();
	QImage dest(width,height,src.format());
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			col = src.pixel(i, j);
			gray = qGray(col);
			dest.setPixel(i, j, qRgb(gray, gray, gray));
		}
	}
	return dest;
}

QImage ImageConvert::convertBinary(QImage src, int threshold){
	QRgb col;
	int gray;
	int width = src.width();
	int height = src.height();
	QImage dest(width,height,src.format());
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			col = src.pixel(i, j);
			gray = qGray(col);
			if (gray>=threshold) dest.setPixel(i, j, qRgb(255,255,255));
			else dest.setPixel(i, j, qRgb(0,0,0));
		}
	}
	return dest;
}

QImage ImageConvert::cvMatToQImage(cv::Mat inMat) {
 	switch (inMat.type()) {
 	// 8-bit, 4 channel
 	case CV_8UC4: {
 		qDebug()
 		 				<< "CV_8UC4";
 		QImage image(inMat.data, inMat.cols, inMat.rows, inMat.step,
 				QImage::Format_RGB32);
 		return image;
 	}

 		// 8-bit, 3 channel
 	case CV_8UC3: {
 		QImage image(inMat.data, inMat.cols, inMat.rows, inMat.step,
 				QImage::Format_RGB888);

 		return image.rgbSwapped();
 	}

 		// 8-bit, 1 channel
 	case CV_8UC1: {
 		static QVector<QRgb> sColorTable;

 		// only create our color table once
 		if (sColorTable.isEmpty()) {
 			for (int i = 0; i < 256; ++i)
 				sColorTable.push_back(qRgb(i, i, i));
 		}

 		QImage image(inMat.data, inMat.cols, inMat.rows, inMat.step,
 				QImage::Format_Indexed8);

 		image.setColorTable(sColorTable);

 		return image;
 	}

 	default:
 		qDebug()
 				<< "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:"
 				<< inMat.type();
 		break;
 	}

 	return QImage();
 }

 cv::Mat ImageConvert::QImageToCvMat(QImage inImage,
 		bool inCloneImageData) {
 	switch (inImage.format()) {
 	// 8-bit, 4 channel
 	case QImage::Format_RGB32: {
 		qDebug() << "Format_RGB32";
 		cv::Mat mat(inImage.height(), inImage.width(), CV_8UC4,
 				const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine());

 		return (inCloneImageData ? mat.clone() : mat);
 	}

 		// 8-bit, 3 channel
 	case QImage::Format_RGB888: {
 		if (!inCloneImageData)
 			qWarning()
 					<< "ASM::QImageToCvMat() - Conversion requires cloning since we use a temporary QImage";

 		QImage swapped = inImage.rgbSwapped();

 		return cv::Mat(swapped.height(), swapped.width(), CV_8UC3,
 				const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine()).clone();
 	}

 		// 8-bit, 1 channel
 	case QImage::Format_Indexed8: {
 		cv::Mat mat(inImage.height(), inImage.width(), CV_8UC1,
 				const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine());

 		return (inCloneImageData ? mat.clone() : mat);
 	}

 	default:
 		qWarning()
 				<< "ASM::QImageToCvMat() - QImage format not handled in switch:"
 				<< inImage.format();
 		break;
 	}

 	return cv::Mat();
 }

/*
		  QPixmap pixmap("logo.png");
    	QImage image = pixmap.toImage();
    QRgb col;
    int gray;
    int width = pixmap.width();
    int height = pixmap.height();
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            col = image.pixel(i, j);
            gray = qGray(col);
            image.setPixel(i, j, qRgb(gray, gray, gray));
        }
    }
    pixmap = pixmap.fromImage(image);
    QLabel label;
    label.setPixmap(pixmap);
		  */


/*
	 BMP	Windows Bitmap	Read/write
	 GIF	Graphic Interchange Format (optional)	Read
	 JPG	Joint Photographic Experts Group	Read/write
	 JPEG	Joint Photographic Experts Group	Read/write
	 PNG	Portable Network Graphics	Read/write
	 PBM	Portable Bitmap	Read
	 PGM	Portable Graymap	Read
	 PPM	Portable Pixmap	Read/write
	 TIFF	Tagged Image File Format	Read/write
	 XBM	X11 Bitmap	Read/write
	 XPM	X11 Pixmap	Read/write
	 */

