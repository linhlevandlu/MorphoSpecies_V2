/*
 * Scenario.cpp
 *
 *  Created on: Aug 21, 2015
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

#include "Scenario.h"

namespace impls_2015 {

/**
 * Constructor
 */
Scenario::Scenario(IExtraction* extraction) {
	this->extraction = extraction;

}

Scenario::~Scenario() {

}

/**
 * Detect the landmarks on image automatically
 * @return: the image contains the landmarks
 */
cv::Mat Scenario::landmarksAutoDetect(Image image) {
	cv::Mat result(image.getMatImage().size(), CV_8UC3);

	// detect the step edges

	EdgeSegmentation* edgeSeg =
			dynamic_cast<EdgeSegmentation *>(this->extraction);
	//result = cv::Mat(image.getMatImage().size(), CV_8UC3);

	edgeSeg->pgh();

	vector<Edge> listEdge = edgeSeg->getEdges(image);
	image.setEdges(listEdge);
	for (size_t i = 0; i < listEdge.size(); i++) {
		Edge ed = listEdge.at(i);

		if (ed.getPoints().size() > 5) {
			// draw the break edge
			//result = ed.drawing(result);
			vector<cv::Point> breakPoints = ed.segment();
			vector<Line> lines = ed.getLines(breakPoints);
			for (size_t k = 0; k < lines.size(); k++) {
				result = lines.at(k).drawing(result);
			}
		}
	}

	return result;
}
} /* namespace impls_2015 */
