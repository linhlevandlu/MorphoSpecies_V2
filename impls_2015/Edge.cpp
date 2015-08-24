/*
 * Edge.cpp
 *
 *  Created on: Aug 20, 2015
 *  Image processing for morphometrics (IPM) Version 2
 *	Copyright (C) 2015 LE Van Linh (linhlevandlu@gmail.com)
 *
 *	This program is free software: you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License as published by
 * 	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include "Edge.h"

namespace impls_2015 {

/**
 * Create an empty edge
 */
Edge::Edge(){

}

/**
 * Create an edge from a list of lines
 */
Edge::Edge(QList<Line> lines) {
	this->listOfLines = lines;
}

/**
 * Create an edge from a list of points
 */
Edge::Edge(QList<cv::Point> points) {
	this->listOfPoints = points;

}

/**
 * Destroy an edge
 */
Edge::~Edge() {
}

/**
 * Get lines of edge
 * @return: list of lines of edge
 */
QList<Line> Edge::getLines() {
	if(listOfLines.size() > 0)
		return this->listOfLines;
	return getLines(listOfPoints);
}

/**
 * Set the lines of edge
 * @parameter: lines - a list of lines
 */
void Edge::setLines(QList<Line> lines) {
	this->listOfLines = lines;
}

/**
 * Get the points of edge
 * @return: a list of points on edge
 */
QList<cv::Point> Edge::getPoints() {
	return this->listOfPoints;
}

/**
 * Set the points of edge
 * @parameter: points - a list of points
 */
void Edge::setPoints(QList<cv::Point> points) {
	this->listOfPoints = points;
}

/**
 * Add an line into the lines of edge
 * @parameter: line - the line need to append into edge
 */
void Edge::addLine(Line line){
	this->listOfLines.append(line);
}

/**
 * Break edge into several straight line
 *
 */
void Edge::breakEdge(){
	cv::Point p0; // = contours[0];
	cv::Point pend;// = contours[contours.size() - 1];
	if(listOfPoints.size()>0){
		p0 = listOfPoints.at(0);
		pend = listOfPoints.at(listOfPoints.size() -1 );

		Line line(p0,pend);
		double length = line.length();// Helper::distanceBetweenPoints(p0, pend);

		double distance = 0;
		double maxDistance = 0;
		double imax = -1;
		for (int i = 0; i < listOfPoints.size(); i++) {
			cv::Point pointi = listOfPoints.at(i);
			distance = line.perpendicularDistance(pointi) ; //Helper::perpendicularDistance(p0, pend, pointi);
			if (distance > maxDistance) {
				maxDistance = distance;
				imax = i;
			}
		}

		double lamda = maxDistance / length;

		Line stepLine(p0, listOfPoints.at(imax));
		double l1 = stepLine.length() ;// Helper::distanceBetweenPoints(p0, contours[imax]);
		double ratio = (maxDistance / l1);
		if (ratio > lamda) {
			QList<Point> part1 = this->listOfPoints.mid(0,imax + 1);
			QList<Point> part2 = this->listOfPoints.mid(imax,listOfPoints.size() - imax - 1);

			Edge edge1(part1);
			Edge edge2(part2);
			edge1.segment();
			edge2.segment();
		}
		RNG rng(12345);
		if (imax != -1) {
			if (!checkPointInList(p0))
				breakPoints.append(p0);
			if(!checkPointInList(listOfPoints.at(imax)))
				breakPoints.append(listOfPoints.at(imax));
			if(!checkPointInList(pend))
				breakPoints.append(pend);
		}
	}else{
		return ;
	}
}

/**
 * Check a point belong to this edge
 * @parameter: point - the point need to check
 * @return: true, if the point on the edge; otherwise, false
 */
bool Edge::checkPointInList(cv::Point point){
	QList<cv::Point> temp = breakPoints;
	foreach(const cv::Point &p, temp) {
		if (p.x == point.x && p.y == point.y)
			return true;
	}
	return false;
}

/**
 * Create a list of continued lines from a list of points
 * @parameter: listPoints - list of points
 * @return: list of continued line
 */
QList<Line> Edge::getLines(QList<cv::Point> listPoints){
	QList<Line> listLines;
	cv::Point p0;
	cv::Point p1;

	if(listPoints.size() > 0){
		p0 = listPoints.at(0);
		for(int i = 1; i < listPoints.size();i++){
			p1 = listPoints.at(i);
			Line l(p0,p1);
			listLines.append(l);
			p0 = p1;
		}
	}

	return listLines;
}

/**
 * Segment an edge
 * @return: list of points, which are intersection between straight lines construct edge
 */
QList<cv::Point> Edge::segment(){
	breakPoints.clear();
	breakEdge();
	return breakPoints;
}

/**
 * Get the pairwise lines from a list of lines
 * @parameter: listLines - list of lines
 * @return: list of pairwise lines.
 */
QList<QList<Line> > Edge::pairwiseLines(QList<Line> listLines){
	QList<QList<Line> > pairwiseLines;
	Line line1; // reference line
	Line line2; // object line

	if (!listLines.isEmpty())
		line1 = listLines.at(0);
	for (int i = 1; i < listLines.size(); i++) {
		line2 = listLines.at(i);
		if (!line1.isNull() && !line2.isNull()) {
			QList<Line> pairwise;
			pairwise.append(line1);
			pairwise.append(line2);
			pairwiseLines.append(pairwise);
		}
		line1 = line2;
	}

	return pairwiseLines;
}

/**
 * Drawing an edge
 * @parameter: outputImage - image contains the result
 * @result: an image in Matrix
 */
cv::Mat Edge::drawing(cv::Mat outputImage) {
	//Mat output = Mat::zeros(inputImage.size(), CV_8UC3);
	QString title;
	if(listOfLines.size() > 0){
		qDebug() << "Drawing the edges from a list of lines.";
		for (int i = 0; i < listOfLines.size(); i++) {
			Line linei = listOfLines.at(i);
			linei.drawing(outputImage);
		}
		title = "Edge By Lines";
	}else{
		if(listOfPoints.size() >0 ){
			qDebug() << "Drawing the edges from list of points.";
			cv::Point p;
			p = listOfPoints.at(0);
			for (int i = 1; i < listOfPoints.size(); i++) {
				cv::Point q = listOfPoints.at(i);
				cv::line(outputImage, p, q, cv::Scalar(0, 255, 0), 1, 8);
				p = q;
			}
			title = "Edge by Points";
		}
	}
	return outputImage;
}
} /* namespace impls_2015 */
