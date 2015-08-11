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
#include <QtCore/qqueue.h>
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
	static QQueue<cv::Point> queuePoints ;
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
	static cv::Mat act2(cv::Mat inputImage);
	static cv::Mat calcHistogram(cv::Mat inputImage);
	static cv::Mat histogram(cv::Mat inputImage);
	static float otSu(cv::Mat histogram);

	static cv::Mat landmarkIndentify(cv::Mat inputImage);
	static cv::Point breakArc(cv::Mat &image,vector<cv::Point> lsPoints);
	static bool sortByXaxis( const cv::Point &a, const cv::Point &p);
	static bool sortByYaxis( const cv::Point &a, const cv::Point &p);
	static cv::Mat edgeSegmentation(cv::Mat inputImage,vector<cv::Point> contours);// ok
	static void getStepEdges(vector<cv::Point> listPoints);
	static int getFirstIndexPoint(vector<cv::Point> listPoints, cv::Point point);

private:
	static SpeciesType getType(QString path);
};

} /* namespace impls_2015 */
#endif /* YELLOWGRID_H_ */
