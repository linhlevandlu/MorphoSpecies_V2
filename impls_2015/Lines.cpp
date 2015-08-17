/*
 * Lines.cpp
 *
 *  Created on: Jun 24, 2015
 *      Author: linh
 */

#include "Lines.h"
#include <QtCore/qdebug.h>
#include <QtCore/QDebug>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>
#include <QtGui/qslider.h>
#include <QtCore/qset.h>
#include <QtCore/qlist.h>


namespace impls_2015 {

Lines::Lines() {
}

/**
 * Constructor a set of line from a queue of points
 * @paramater: a queue of points
 * @return: set of line
 */
QList<QLine> Lines::getLines(QQueue<cv::Point> queuePoints){

	QList<QLine> listLines;
	cv::Point p0;
	cv::Point p1;
	if(!queuePoints.isEmpty()){
		p0 = queuePoints.dequeue();
	}
	while(!queuePoints.isEmpty()){
		p1 = queuePoints.dequeue();
		QLine line(p0.x, p0.y,p1.x,p1.y);
		listLines.append(line);
		p0 = p1;
	}
	return listLines;
}
} /* namespace impls_2015 */
