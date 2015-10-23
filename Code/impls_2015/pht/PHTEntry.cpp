/*
 * PHTEntry.cpp
 *
 *  Created on: Sep 27, 2015
 *      Author: linh
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

#include "PHTEntry.h"

namespace impls_2015 {

PHTEntry::PHTEntry() {
	// TODO Auto-generated constructor stub

}

PHTEntry::~PHTEntry() {
	// TODO Auto-generated destructor stub
}
void PHTEntry::setLine1(Line line){
	this->line1 = line;
}
void PHTEntry::setLine2(Line line){
	this->line2 = line;
}
Line PHTEntry::getLine1(){
	return this->line1;
}
Line PHTEntry::getLine2(){
	return this->line2;
}
vector<HoughSpace> PHTEntry::getHoughSpaces(){
	return this->listHoughSpace;
}

void PHTEntry::addHoughSpace(HoughSpace houghSpace) {
	this->listHoughSpace.push_back(houghSpace);
}
} /* namespace impls_2015 */
