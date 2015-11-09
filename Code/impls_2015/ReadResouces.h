/*
 * ReadResouces.h
 *
 *  Created on: Nov 8, 2015
 *      Author: linh
 */

#ifndef READRESOUCES_H_
#define READRESOUCES_H_
#include <iostream>
#include <fstream>

#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include <QtCore/qlist.h>
#include <QtGui/QFileDialog>
#include <QtCore/qdebug.h>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <time.h>
using namespace std;

namespace impls_2015 {

class ReadResouces {
public:
	//static map<string,int> resourceMap;
	ReadResouces();
	virtual ~ReadResouces();
	static map<string,int> readResources(string filePath);
};

} /* namespace impls_2015 */
#endif /* READRESOUCES_H_ */
