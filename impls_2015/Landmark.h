/*
 * Landmark.h
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

#ifndef LANDMARK_H_
#define LANDMARK_H_

#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include <QtCore/qlist.h>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

#include "IDrawOperation.h"
namespace impls_2015 {

class Landmark: public IDrawOperation {
private:
	cv::Point location;
public:
	Landmark();
	Landmark(cv::Point location);
	virtual ~Landmark();
	cv::Point getLocation();
	void setLocation(cv::Point location);
	cv::Mat drawing(cv::Mat outputImage);
};

} /* namespace impls_2015 */
#endif /* LANDMARK_H_ */
