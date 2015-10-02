/*
 * PHTEntry.h
 *
 *  Created on: Sep 27, 2015
 *      Author: linh
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
