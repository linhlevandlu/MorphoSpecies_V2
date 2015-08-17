/*
 * Helper.cpp
 * This file contains the helper functions for program.
 *  Created on: Aug 12, 2015
 *      Author: linh
 */

#include "Helper.h"

namespace impls_2015 {

Helper::Helper() {
	// TODO Auto-generated constructor stub

}

/**
 * Get the first position of a point in a list of points
 * @parameter 1: listPoints - list of points
 * @parameter 2: point - the point need to check
 * @return: the first position of the point in the list of points. If it does not exist in list, return -1.
 */
int Helper::getFirstIndexPoint(vector<cv::Point> listPoints, cv::Point point){
	for(int i =0;i<listPoints.size();i++){
		if(listPoints[i].x == point.x
			&& listPoints[i].y == point.y)
			return i;
	}
	return -1;
}

/**
 * Calculate the distance between 2 points
 * @parameter 1: point1 - the first point
 * @parameter 2: point2 - the second point
 * @return: distance from point1 to point2
 */
double Helper::distanceBetweenPoints(cv::Point point1,cv::Point point2){
	double distance = sqrt(pow(point2.x - point1.x,2) + pow(point2.y - point1.y,2));
	return distance;
}

/**
 * Check the existing of a point in a queue of points
 * @parameter 1: queue - the queue of points
 * @parameter 2: point - the point need to check
 * @return: true, if the point exists in queue. Otherwise, return false
 */
bool Helper::checkPointInQueue(QQueue<cv::Point> queue, cv::Point point){
	QQueue<cv::Point> temp = queue;
	foreach(const cv::Point &p, temp){
		if(p.x == point.x
			&& p.y == point.y)
			return true;
	}
	return false;
}

} /* namespace impls_2015 */
