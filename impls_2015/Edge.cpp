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
vector<cv::Point> Edge::breakPoints;
/**
 * Create an empty edge
 */
Edge::Edge() {

}

/**
 * Create an edge from a list of lines
 */
Edge::Edge(vector<Line> lines) {
	this->listOfLines = lines;
}

/**
 * Create an edge from a list of points
 */
Edge::Edge(vector<cv::Point> points) {
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
vector<Line> Edge::getLines() {
	if (listOfLines.size() > 0)
		return this->listOfLines;
	return getLines(listOfPoints);
}

/**
 * Set the lines of edge
 * @parameter: lines - a list of lines
 */
void Edge::setLines(vector<Line> lines) {
	this->listOfLines = lines;
}

/**
 * Get the points of edge
 * @return: a list of points on edge
 */
vector<cv::Point> Edge::getPoints() {
	return this->listOfPoints;
}

/**
 * Set the points of edge
 * @parameter: points - a list of points
 */
void Edge::setPoints(vector<cv::Point> points) {
	this->listOfPoints = points;
}

/**
 * Add an line into the lines of edge
 * @parameter: line - the line need to append into edge
 */
void Edge::addLine(Line line) {
	this->listOfLines.push_back(line);
}

/**
 * Break edge into several straight line
 *
 */
void Edge::breakEdge() {
	cv::Point p0; // = contours[0];
	cv::Point pend; // = contours[contours.size() - 1];
	if (listOfPoints.size() > 0) {
		p0 = listOfPoints.at(0);
		pend = listOfPoints.at(listOfPoints.size() - 1);

		if (listOfPoints.size() > 2) {
			Line line(p0, pend);
			double length = line.length();

			double distance = 0;
			double maxDistance = 0; // ??????????????????
			double imax = 0;
			for (size_t i = 1; i < listOfPoints.size()-1; i++) {
				cv::Point pointi = listOfPoints.at(i);
				distance = abs(line.perpendicularDistance(pointi));
				if (distance > maxDistance) {
					maxDistance = distance;
					imax = i;
				}
			}

			//double lamda = maxDistance / length;

			//Line stepLine(p0, listOfPoints.at(imax));
			//double l1 = stepLine.length();
			//double ratio = (maxDistance / l1);
			if (maxDistance > 3) {
				vector<cv::Point> part1(this->listOfPoints.begin(),
						this->listOfPoints.begin() + imax + 1);
				vector<cv::Point> part2(this->listOfPoints.begin() + imax,
						this->listOfPoints.end());
				Edge edge1(part1);
				Edge edge2(part2);
				edge1.breakEdge();
				edge2.breakEdge();
			}
		}

		if (!checkPointInList(p0))
			breakPoints.push_back(p0);
		//if (!checkPointInList(listOfPoints.at(imax)))
		//	breakPoints.append(listOfPoints.at(imax));
		if (!checkPointInList(pend))
			breakPoints.push_back(pend);

	} else {
		return;
	}
}

vector<Point> Edge::breakEdge2(vector<Point> contours){
	vector<Point> appContour;
	approxPolyDP(contours,appContour,3,false);
	return appContour;
}
/**
 * Check a point belong to this edge
 * @parameter: point - the point need to check
 * @return: true, if the point on the edge; otherwise, false
 */
bool Edge::checkPointInList(cv::Point point) {
	vector<cv::Point> temp = breakPoints;
	for (size_t i = 0; i < temp.size(); i++) {
		cv::Point p = temp[i];
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
vector<Line> Edge::getLines(vector<cv::Point> listPoints) {
	vector<Line> listLines;
	cv::Point p0;
	cv::Point p1;

	if (listPoints.size() > 0) {
		p0 = listPoints.at(0);
		for (size_t i = 1; i < listPoints.size(); i++) {
			p1 = listPoints.at(i);
			Line l(p0, p1);
			listLines.push_back(l);
			p0 = p1;
		}
	}

	return listLines;
}

/**
 * Segment an edge
 * @return: list of points, which are intersection between straight lines construct edge
 */
vector<cv::Point> Edge::segment() {
	/*breakPoints.clear();
	breakEdge();
	return breakPoints;*/
	return breakEdge2(this->listOfPoints);
}

/**
 * Get the pairwise lines from a list of lines
 * @parameter: listLines - list of lines
 * @return: list of pairwise lines.
 */
vector<vector<Line> > Edge::pairwiseLines(vector<Line> listLines) {
	vector<vector<Line> > pairwiseLines;
	Line line1; // reference line
	Line line2; // object line

	if (!listLines.size()> 0)
		line1 = listLines.at(0);
	for (size_t i = 1; i < listLines.size(); i++) {
		line2 = listLines.at(i);
		if (!line1.isNull() && !line2.isNull()) {
			vector<Line> pairwise;
			pairwise.push_back(line1);
			pairwise.push_back(line2);
			pairwiseLines.push_back(pairwise);
		}
		line1 = line2;
	}

	return pairwiseLines;
}

vector<Edge> Edge::splitEdge(int numOfParts) {
	vector<Edge> edges;
	if (listOfPoints.size() > 0) {
		int splitSize = listOfPoints.size() / numOfParts;
		int indexSplit = 0;
		while (indexSplit < numOfParts) {
			int begin = indexSplit * splitSize;
			int end = ((indexSplit + 1) * splitSize) - 1;
			if (indexSplit == numOfParts - 1) {
				end = listOfPoints.size() - 1;
			}
			cv::Point p0 = listOfPoints.at(begin);
			cv::Point pend = listOfPoints.at(end);
			Line line(p0, pend);
			double distance = line.perpendicularDistance(
					listOfPoints.at(begin + 1));
			bool negative = false;
			if (distance > 0)
				negative = false;
			else
				negative = true;
			QQueue<int> cutPoints;
			for (int j = begin + 1; j < end; j++) {
				distance = line.perpendicularDistance(listOfPoints.at(j));
				if ((distance > 0 && negative) || (distance < 0 && !negative)) {
					cutPoints.enqueue(j - 1);
					if (distance < 0)
						negative = true;
					else
						negative = false;
				}
			}
			int k0 = begin;
			int k1 = 0;
			while (!cutPoints.isEmpty()) {
				k1 = cutPoints.dequeue();
				vector<Point> vp(listOfPoints.begin() + k0,listOfPoints.begin() + k1);
				Edge ed(vp);
				edges.push_back(ed);
				k0 = k1;
			}
			vector<Point> vp2(listOfPoints.begin() + k0,listOfPoints.begin() + end);
			Edge ed2(vp2);
			edges.push_back(ed2);
			indexSplit++;
		}
	}
	return edges;
}

/**
 * Drawing an edge
 * @parameter: outputImage - image contains the result
 * @result: an image in Matrix
 */
cv::Mat Edge::drawing(cv::Mat outputImage) {
	//Mat output = Mat::zeros(inputImage.size(), CV_8UC3);
	QString title;
	if (listOfLines.size() > 0) {
		//qDebug() << "Drawing the edges from a list of lines.";
		for (size_t i = 0; i < listOfLines.size(); i++) {
			Line linei = listOfLines.at(i);
			linei.drawing(outputImage);
		}
		title = "Edge By Lines";
	} else {
		if (listOfPoints.size() > 0) {
			//qDebug() << "Drawing the edges from list of points.";
			cv::Point p;
			p = listOfPoints.at(0);
			for (size_t i = 1; i < listOfPoints.size(); i++) {
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
