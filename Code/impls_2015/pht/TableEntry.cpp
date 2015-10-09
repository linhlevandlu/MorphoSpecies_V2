/*
 * TableEntry.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: linh
 */

#include "TableEntry.h"

namespace impls_2015 {

TableEntry::TableEntry() {
	// TODO Auto-generated constructor stub

}

TableEntry::~TableEntry() {
	// TODO Auto-generated destructor stub
}
void TableEntry::setRefLine(Line line){
	this->refLine = line;
}
void TableEntry::setHoughValue(HoughSpace hvalue){
	this->houghValue = hvalue;
}
Line TableEntry::getLine(){
	return this->refLine;
}
double TableEntry::lengthOfLine() const{
	return const_cast<TableEntry*> (this)->refLine.length();
}
HoughSpace TableEntry::getHoughValue(){
	return this->houghValue;
}
bool TableEntry::operator<(const TableEntry& entry2) const{
	const double ds1 = const_cast<TableEntry*> (this)->refLine.length();
	const double ds2 =  entry2.lengthOfLine();
	if(ds1 < ds2)
		return true;
	return false;
}
} /* namespace impls_2015 */
