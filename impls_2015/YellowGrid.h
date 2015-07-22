/*
 * YellowGrid.h
 *
 *  Created on: Jul 9, 2015
 *      Author: linh
 */

#ifndef YELLOWGRID_H_
#define YELLOWGRID_H_

#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

using namespace cv;
using namespace std;
namespace impls_2015 {



class YellowGrid {

public:
	enum SpeciesType {
			ELYTRE,
			MDROITE,
			MGAUCHE,
			PRONOTUM,
			TETE
		};
	YellowGrid();
	static cv::Mat removeYellowLines(cv::Mat matImage, int minBrightness,
			QString pathImage);
	//static cv::Mat usingHistogram(cv::Mat input);
	//static float otSu(cv::Mat histogram);
	//static cv::Mat tryRemove(cv::Mat input);

	static cv::Mat landmarkIndentify(cv::Mat inputImage);

private:
	static SpeciesType getType(QString path);
};

} /* namespace impls_2015 */
#endif /* YELLOWGRID_H_ */
