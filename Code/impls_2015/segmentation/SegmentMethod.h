/*
 * SegementationMethod.h
 *
 *  Created on: Sep 17, 2015
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

#ifndef SEGMENTMETHOD_H_
#define SEGMENTMETHOD_H_

#include "../Image.h"
#include "../Line.h"


using namespace std;
using namespace cv;

namespace impls_2015 {

class SegmentMethod {
public:
	SegmentMethod();
	virtual ~SegmentMethod();
	virtual vector<Line> lineSegment(Image image,Image::SegmentMethod sgmethod) = 0;
};

} /* namespace impls_2015 */
#endif /* SEGMENTMETHOD_H_ */
