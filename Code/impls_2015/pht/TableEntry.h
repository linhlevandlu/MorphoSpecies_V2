/*
 * TableEntry.h
 *
 *  Created on: Oct 7, 2015
 *      Author: linh
 */

#ifndef TABLEENTRY_H_
#define TABLEENTRY_H_

#include "../Line.h"
#include "HoughSpace.h"

using namespace std;
using namespace cv;

namespace impls_2015 {

class TableEntry {
private:
	Line refLine;
	HoughSpace houghValue;
public:
	TableEntry();
	virtual ~TableEntry();
	void setRefLine(Line line);
	void setHoughValue(HoughSpace hvalue);
	Line getLine();
	double lengthOfLine() const;
	HoughSpace getHoughValue();
	bool operator<(const TableEntry& l2) const;
};

} /* namespace impls_2015 */
#endif /* TABLEENTRY_H_ */
