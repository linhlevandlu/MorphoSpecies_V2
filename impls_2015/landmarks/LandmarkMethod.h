/*
 * LandmarkMethod.h
 *
 *  Created on: Sep 17, 2015
 *      Author: linh
 */

#ifndef LANDMARKMETHOD_H_
#define LANDMARKMETHOD_H_

#include "../Image.h"
#include "../Landmark.h"
using namespace std;
using namespace cv;

namespace impls_2015 {

class LandmarkMethod {
public:
	LandmarkMethod();
	virtual ~LandmarkMethod();
	virtual vector<Landmark> getLandmarks(Image image) = 0;
};

} /* namespace impls_2015 */
#endif /* LANDMARKMETHOD_H_ */
