/*
 * PairwiseHistogram.h
 *
 *  Created on: Aug 24, 2015
 *      Author: linh
 */

#ifndef GFEATURES_H_
#define GFEATURES_H_

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

namespace impls_2015 {

class GFeatures {
private:
	//Line refLine;
	//Line objLine;
	double angle; // angle in degree
	double dmin;
	double dmax;
public:
	GFeatures();
	virtual ~GFeatures();
	double getAngle();
	double getDmin();
	double getDmax();
	void setAngle(double degree);
	void setDmin(double dmin);
	void setDmax(double dmax);
};

} /* namespace impls_2015 */
#endif /* PAIRWISEHISTOGRAM_H_ */
