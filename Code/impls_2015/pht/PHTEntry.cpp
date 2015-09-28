/*
 * PHTEntry.cpp
 *
 *  Created on: Sep 27, 2015
 *      Author: linh
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
