/*
 * SegementationMethod.h
 *
 *  Created on: Sep 17, 2015
 *      Author: linh
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
	virtual vector<Line> lineSegment(Image image) = 0;
};

} /* namespace impls_2015 */
#endif /* SEGMENTMETHOD_H_ */
