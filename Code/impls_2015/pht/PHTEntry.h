/*
 * PHTEntry.h
 *
 *  Created on: Sep 27, 2015
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

#ifndef PHTENTRY_H_
#define PHTENTRY_H_

#include "../Line.h"
#include "HoughSpace.h"

using namespace std;
using namespace cv;

namespace impls_2015 {

class PHTEntry {
private:
	Line line1;
	Line line2;
	vector<HoughSpace> listHoughSpace;
public:
	PHTEntry();
	virtual ~PHTEntry();
	void setLine1(Line line);
	void setLine2(Line line);
	Line getLine1();
	Line getLine2();
	vector<HoughSpace> getHoughSpaces();
	void addHoughSpace(HoughSpace houghSpace);



};

} /* namespace impls_2015 */
#endif /* PHTENTRY_H_ */
