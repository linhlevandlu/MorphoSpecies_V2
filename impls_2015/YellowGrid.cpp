/*
 *  YellowGrid.cpp
 *
 *  Created on: Jul 9, 2015
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

#include "YellowGrid.h"
#include <QtCore/QDebug>
#include <QtCore/qqueue.h>
#include "../ImageConvert.h"
#include "../ImageViewer.h"

namespace impls_2015 {

/**
 * Constructor
 */
YellowGrid::YellowGrid() {
}

/**
 * Constructor with the limit point
 */
YellowGrid::YellowGrid(cv::Point lmPoint) {
	this->limitPoint = lmPoint;
}

/**
 * Get the limit point of yellow grid
 * @return: the limit point of the grid
 */
cv::Point YellowGrid::getLimitPoint() {
	return this->limitPoint;
}

/**
 * Set the limit point of yellow grid
 * @parameter: lmPoint - the limit point
 */
void YellowGrid::setLimitPoint(cv::Point lmPoint) {
	this->limitPoint = lmPoint;
}
} /* namespace impls_2015 */

