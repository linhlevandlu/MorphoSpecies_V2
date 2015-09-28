/*
 * HoughSpace.h
 *
 *  Created on: Sep 27, 2015
 *      Author: linh
 */

#ifndef HOUGHSPACE_H_
#define HOUGHSPACE_H_


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <time.h>

#include "../Line.h"

using namespace std;
using namespace cv;

namespace impls_2015 {

class HoughSpace {
private:
	double distance;
	double angle;
public:
	HoughSpace();
	virtual ~HoughSpace();
	void setDistance(Line objectLine, Point refPoint);
	void setAngle(Line objectLine, Point refPoint);
	double getDistance();
	double getAngle();
};

} /* namespace impls_2015 */
#endif /* HOUGHSPACE_H_ */
