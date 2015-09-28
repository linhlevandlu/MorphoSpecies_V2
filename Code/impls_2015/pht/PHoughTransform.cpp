/*
 * PHoughTransform.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: linh
 */

#include "PHoughTransform.h"

namespace impls_2015 {

PHoughTransform::PHoughTransform() {

}

PHoughTransform::~PHoughTransform() {
	// TODO Auto-generated destructor stub
}

cv::Mat PHoughTransform::pHoughTransform(cv::Mat inputImage) {
	cv::Mat dst;
	cv::Canny(inputImage, dst, 50, 255, 3);
	vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI / 180, 60, 20, 10);
	for (size_t i = 0; i < lines.size(); i++) {
		Vec4i line = lines[i];
		cv::line(inputImage, Point(line[0], line[1]), Point(line[2], line[3]),
				Scalar(0, 0, 255), 1, 8);
	}

	return inputImage;
}

void PHoughTransform::test(vector<Line> refLines, vector<Line> sceneLines){

}

void PHoughTransform::addPHTEntry(PHTEntry entry) {
	this->accTable.push_back(entry);
}

vector<PHTEntry> PHoughTransform::constructTable(vector<Line> lines) {
	vector<PHTEntry> phtransform;
	ofstream of("accumultor.txt");
	for (size_t i = 0; i < lines.size(); i++) {
		Line line1 = lines.at(i);
		HoughSpace hs1;
		hs1.setAngle(line1, refPoint);
		hs1.setDistance(line1, refPoint);
		for (size_t j = 0; j < lines.size(); j++) {
			Line line2 = lines.at(j);
			HoughSpace hs2;
			hs2.setAngle(line2, refPoint);
			hs2.setDistance(line2, refPoint);
			if (i != j) {
				PHTEntry entry;
				entry.setLine1(line1);
				entry.setLine2(line2);
				entry.addHoughSpace(hs1);
				entry.addHoughSpace(hs2);
				phtransform.push_back(entry);
				of << "(" << (line1.getP1().x) << "," << (line1.getP1().y)
						<< ")" << " (" << (line1.getP2().x) << ","
						<< (line1.getP2().y) << ")" << "(" << (line2.getP1().x)
						<< "," << (line2.getP1().y) << ")" << " ("
						<< (line2.getP2().x) << "," << (line2.getP2().y) << ")"
						<< "(" << hs1.getAngle() << "," << hs1.getDistance()
						<< ")" << " (" << (hs2.getAngle()) << ","
						<< (hs2.getDistance()) << ")" << "\n";
			}
		}
	}
	of.close();
	return phtransform;
}

PHTEntry PHoughTransform::findHoughSpace(vector<PHTEntry> entryTable,
		Line line1, Line line2) {
	PHTEntry entry;
	Point pt1(0, 0), pt2(0, 0), pt3(0, 0), pt4(0, 0);
	Point temp(0, 0);
	for (size_t i = 0; i < entryTable.size(); i++) {
		Line ref1 = entryTable.at(i).getLine1();
		Line ref2 = entryTable.at(i).getLine2();

		Point p1 = line1.getP1() - ref1.getP1();
		Point p2 = line1.getP2() - ref1.getP2();
		Point p3 = line2.getP1() - ref2.getP1();
		Point p4 = line2.getP2() - ref2.getP2();

		if (pt1 == temp && pt2 == temp && pt3 == temp && pt4 == temp) {
			entry = entryTable.at(i);
			pt1 = p1;
			pt2 = p2;
			pt3 = p3;
			pt4 = p4;
		} else {
			if ((p1.x <= pt1.x && p1.y <= pt1.y)
					&& (p2.x <= pt2.x && p2.y <= pt2.y)
					&& (p3.x <= pt3.x && p3.y <= pt3.y)
					&& (p4.x <= pt4.x && p4.y <= pt4.y)) {
				entry = entryTable.at(i);
				pt1 = p1;
				pt2 = p2;
				pt3 = p3;
				pt4 = p4;
			}
		}

	}
	return entry;
}


vector<vector<int> > PHoughTransform::accumulator(Image refimage,
		Image sceneImage) {
	int x = refimage.getMatrixImage().cols / 2;
	int y = refimage.getMatrixImage().rows / 2;
	this->refPoint.x = x;
	this->refPoint.y = y;

	this->accTable = constructTable(refimage.lineSegment());

	// initialization an accumulator
	vector<vector<int> > acc;
	int rows = floor(
			sqrt(
					(refimage.getMatrixImage().cols
							* refimage.getMatrixImage().cols)
							+ (refimage.getMatrixImage().rows
									* refimage.getMatrixImage().rows)));
	acc.resize(rows);
	for (int t = 0; t < rows; t++) {
		acc[t].resize(181, 0);
	}
	vector<Line> objectLines = sceneImage.lineSegment();
	for (size_t i = 0; i < objectLines.size(); i++) {
		Line objLine1 = objectLines.at(i);
		for (size_t j = 0; j < objectLines.size(); j++) {
			Line objLine2 = objectLines.at(j);
			if (i != j) {
				PHTEntry entry = findHoughSpace(this->accTable, objLine1,
						objLine2);
				vector<HoughSpace> hspace = entry.getHoughSpaces();
				for (size_t k = 0; k < hspace.size(); k++) {
					HoughSpace hsp = hspace.at(k);
					int angle = round(hsp.getAngle());
					int distance = round(hsp.getDistance());
					acc[distance][angle]++;
				}
			}
		}
	}
	return acc;
}

int PHoughTransform::maxOfAccumulator(vector<vector<int> > accumulator) {
	int max = 0;
	for (size_t i = 0; i < accumulator.size(); i++) {
		for (size_t j = 0; j < accumulator[i].size(); j++) {
			if (accumulator[i][j] > max)
				max = accumulator[i][j];
		}
	}
	return max;
}

} /* namespace impls_2015 */
