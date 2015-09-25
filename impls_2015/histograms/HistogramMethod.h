/*
 * HistogramMethod.h
 *
 *  Created on: Sep 17, 2015
 *      Author: linh
 */

#ifndef HISTOGRAMMETHOD_H_
#define HISTOGRAMMETHOD_H_

#include "../Image.h"

using namespace std;
using namespace cv;

namespace impls_2015 {

class HistogramMethod {
public:
	HistogramMethod();
	virtual ~HistogramMethod();
	virtual vector<LocalHistogram> shapeHistogram(Image image, LocalHistogram::AccuracyPGH angleAcc, int columns,cv::Mat &result) = 0;
};

} /* namespace impls_2015 */
#endif /* HISTOGRAMMETHOD_H_ */
