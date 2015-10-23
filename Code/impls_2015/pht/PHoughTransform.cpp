/*
 * PHoughTransform.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: linh
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

#include "PHoughTransform.h"

namespace impls_2015 {

PHoughTransform::PHoughTransform() {

}

PHoughTransform::~PHoughTransform() {
	// TODO Auto-generated destructor stub
}

/**
 * Construct the table to store the angle and distance of pair lines to a reference point
 */
vector<PHTEntry> PHoughTransform::constructTable(vector<Line> lines,
		Point refPoint) {
	vector<PHTEntry> phtransform;
	ofstream of("table_acc.txt");
	for (size_t i = 0; i < lines.size(); i++) {
		Line line1 = lines.at(i);
		for (size_t j = 0; j < lines.size(); j++) {
			Line line2 = lines.at(j);
			if (i != j && closetLine(line1, line2)) {
				HoughSpace hs1;
				hs1.setAngle(hs1.computeAngle(line1, refPoint));
				hs1.setDistance(hs1.computeDistance(line1, refPoint));

				HoughSpace hs2;
				hs2.setAngle(hs2.computeAngle(line2, refPoint));
				hs2.setDistance(hs2.computeDistance(line2, refPoint));

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

bool PHoughTransform::closetLine(Line line1, Line line2) {

	double distance1 = line2.perpendicularDistance(line1.getP1());
	double distance2 = line2.perpendicularDistance(line1.getP2());
	if (line1.length() > 60 && line2.length() > 60
			&& line1.intersection(line2).x != -1
			&& line1.intersection(line2).y != -1
			&& (distance1 <= 5 || distance2 <= 5))
		return true;
	return false;
}

/**
 * Indicate the similarity of two pair lines
 */
bool PHoughTransform::similarPairLines(Line ref1, Line ref2, Line scene1,
		Line scene2) {

	double refAngle = ref1.angleBetweenLines(ref2);
	Line temp(ref1.getP1(), ref2.getP1());

	double sceneAngle = scene1.angleBetweenLines(scene2);
	Line temp2(scene1.getP1(), scene2.getP1());

	if (abs(refAngle - sceneAngle) < 1
			&& (abs(
					ref1.length() / scene1.length()
							- ref2.length() / scene2.length()) < 2)) {
		return true;
	}
	return false;

}

/**
 * Find a similar pair lines in table
 */
PHTEntry PHoughTransform::findHoughSpace(vector<PHTEntry> entryTable,
		Line line1, Line line2) {
	PHTEntry entry;
	for (size_t i = 0; i < entryTable.size(); i++) {
		Line ref1 = entryTable.at(i).getLine1();
		Line ref2 = entryTable.at(i).getLine2();
		if (similarPairLines(ref1, ref2, line1, line2))
			entry = entryTable.at(i);
	}
	return entry;
}

PHTEntry PHoughTransform::matchingInScene(vector<PHTEntry> entryTable,
		vector<Line> sceneLines, int width, int height,
		vector<Line> &maxVector) {
	// initialization an accumulator
	vector<vector<int> > acc;
	int rows = floor(sqrt(width * width + height * height));
	acc.resize(rows);
	for (int t = 0; t < rows; t++) {
		acc[t].resize(361, 0);
	}

	int maxValue = 0;
	//vector<Line> maxVector;
	PHTEntry maxEntry;
	for (size_t i = 0; i < sceneLines.size(); i++) {
		Line objLine1 = sceneLines.at(i);
		for (size_t j = 0; j < sceneLines.size(); j++) {
			Line objLine2 = sceneLines.at(j);
			if (i != j && closetLine(objLine2, objLine1)) {
				PHTEntry entry = findHoughSpace(entryTable, objLine1, objLine2);
				if (entry.getHoughSpaces().size() > 0) {
					vector<HoughSpace> hspace = entry.getHoughSpaces();
					for (size_t k = 0; k < hspace.size(); k++) {
						HoughSpace hsp = hspace.at(k);
						int angle = round(hsp.getAngle());
						int distance = round(hsp.getDistance());
						if (!isnan(angle) && !isnan(distance) && angle >= 0
								&& distance >= 0) {
							acc[distance][angle] += 1;
							if (acc[distance][angle] > maxValue) {
								maxVector.clear();
								maxVector.push_back(objLine1);
								maxVector.push_back(objLine2);
								maxValue = acc[distance][angle];
								maxEntry = entry;
							} else {
								if (k == 0
										&& acc[distance][angle] == maxValue) {
									maxVector.push_back(objLine1);
									maxVector.push_back(objLine2);
								}
							}
						}
					}
				}
			}
		}
	}
	qDebug() << "Number of maxVector: " << maxVector.size();
	return maxEntry;
}

Point PHoughTransform::refPointInScene(PHTEntry entry, vector<Line> matchLines,
		double &angleDiff, int width, int height) {
	if (matchLines.size() <= 0)
		return Point(0, 0);
	Line objl1 = matchLines.at(0);
	Line objl2 = matchLines.at(1);

	qDebug() << "scene pairs: ";
	objl1.toString();
	objl2.toString();
	qDebug() << "Entry";
	Line lineEntry1 = entry.getLine1();
	Line lineEntry2 = entry.getLine2();

	HoughSpace hs1, hs2;
	lineEntry1.toString();
	lineEntry2.toString();
	hs1 = entry.getHoughSpaces().at(0);
	hs2 = entry.getHoughSpaces().at(1);
	angleDiff = angleDifference(lineEntry1, objl1);
	double angleDiff2 = angleDifference(lineEntry2, objl2);
	if (abs(lineEntry1.length() - objl1.length())
			>= abs(lineEntry1.length() - objl2.length())) {
		hs1 = entry.getHoughSpaces().at(1);
		hs2 = entry.getHoughSpaces().at(0);
		angleDiff = angleDifference(lineEntry1, objl2);
		angleDiff2 = angleDifference(lineEntry2, objl2);
	}
	if (angleDiff > 0) {
		if (angleDiff2 < angleDiff)
			angleDiff = angleDiff2;
	} else {
		if (angleDiff2 > angleDiff)
			angleDiff = angleDiff2;
	}

	cv::Point intersect;

	intersect = objl1.interParallel(objl1, objl2, hs1.getDistance(),
			hs2.getDistance(), width, height);

	qDebug() << "Point intersection: " << intersect.x << ", " << intersect.y;
	return intersect;
}

Point PHoughTransform::refPointInScene(Image modelImage, Image sceneImage) {
	cv::Mat modelMat = modelImage.getMatrixImage();
	cv::Mat sceneMat = sceneImage.getMatrixImage();
	vector<PHTEntry> tableEntry = constructTable(modelImage.lineSegment(),
			cv::Point(modelMat.cols / 2, modelMat.rows / 2));
	vector<Line> maxVector;
	PHTEntry entry = matchingInScene(tableEntry, sceneImage.lineSegment(),
			sceneMat.cols, sceneMat.rows, maxVector);
	double angleDiff;
	return refPointInScene(entry, maxVector, angleDiff, modelMat.cols,
			modelMat.rows);
}
Mat PHoughTransform::phtPresentation(Image refImage, Image sceneImage,
		string reflmPath) {

	//QString scenelmDir = "/home/linh/Desktop/mg/landmarks";
	QString scenename = sceneImage.getName();
	//QString scenelmPath = scenelmDir + "/" + scenename + ".TPS";
	//vector<Point> orglandmarks = sceneImage.readLandmarksFile(scenelmPath.toStdString());
	double angleDiff;
	Point ePoint;
	vector<Point> esLandmarks = estimateLandmarks(refImage, sceneImage,
			reflmPath, angleDiff, ePoint);

	Mat sceneMat = sceneImage.getMatrixImage();
	cv::Mat mat(sceneMat.clone());
	/*for (size_t t = 0; t < esLandmarks.size(); t++) {
		Point landm = orglandmarks.at(t);
		circle(mat, cv::Point(landm.x, mat.rows - landm.y), 7,
				Scalar(0, 0, 255), 2, 4);
	}*/

	qDebug() << "angle Diff: " << angleDiff;
	for (size_t t = 0; t < esLandmarks.size(); t++) {
		circle(mat, esLandmarks.at(t), 2, Scalar(0, 255, 255), 2, 8);
		//qDebug()<<"x, y"<<esLandmarks.at(t).x<<", "<< esLandmarks.at(t).y;
	}
	return mat;
}

vector<Point> PHoughTransform::estimateLandmarks(Image mImage, Image sImage,
		string mlmPath, double &angleDiff, Point &ePoint) {
	vector<Point> eLandmarks;
	Mat mMatrix = mImage.getMatrixImage();
	int width = mMatrix.cols;
	int height = mMatrix.rows;

	vector<Point> mLandmarks = mImage.readLandmarksFile(mlmPath);

	vector<Line> mLines = mImage.lineSegment();
	vector<Line> sLines = sImage.lineSegment();

	Point mPoint(width / 2, height / 2);
	vector<PHTEntry> entryTable = constructTable(mLines, mPoint);
	vector<Line> maxVector;
	PHTEntry entry = matchingInScene(entryTable, sLines, width, height,
			maxVector);
	if (maxVector.size() > 0) {
		ePoint = refPointInScene(entry, maxVector, angleDiff, width, height);

		double angle1 = entry.getLine1().angleBetweenLines(entry.getLine2());
		double angle2 = maxVector.at(0).angleBetweenLines(maxVector.at(1));
		angleDiff += abs(angle1 - angle2);

		eLandmarks = findLandmarks(mPoint, ePoint, angleDiff, mLandmarks, width,
				height);
	}
	return eLandmarks;

}
vector<Point> PHoughTransform::findLandmarks(Point refPoint, Point esPoint,
		double angleDiff, vector<Point> refLandmarks, int width, int height) {
	vector<Point> esLandmarks;
	for (size_t t = 0; t < refLandmarks.size(); t++) {
		Point temp = refLandmarks.at(t);
		Point lm(temp.x, height - temp.y);
		int px = refPoint.x - esPoint.x;
		int py = refPoint.y - esPoint.y;
		int x;
		int y;
		x = lm.x - px;
		if (y > 0)
			y = lm.y - py;
		else
			y = lm.y + py;
		//}
		esLandmarks.push_back(Point(x, y));
	}
	return esLandmarks;
}
Point PHoughTransform::newLocation(Point point, double angleDiff,
		Point refPoint) {
	int x, y;
	qDebug() << "rotate angle: " << angleDiff;
	if (angleDiff >= 0) {
		x = point.x * cos(round(angleDiff) * M_PI / 180)
				- point.y * sin(round(angleDiff) * M_PI / 180) + refPoint.x;
		y = point.x * sin(round(angleDiff) * M_PI / 180)
				+ point.y * cos(round(angleDiff) * M_PI / 180) + refPoint.y;
	} else {
		x = point.x * cos(round(angleDiff) * M_PI / 180)
				+ point.y * sin(round(angleDiff) * M_PI / 180) + refPoint.x;
		y = -point.x * sin(round(angleDiff) * M_PI / 180)
				+ point.y * cos(round(angleDiff) * M_PI / 180) + refPoint.y;
	}
	return Point(x, y);
}
void PHoughTransform::phtDirectory(Image refImage, QString reflmPath,
		QString sceneDir, QString scenelmDir, QString saveDir) {

	QFileInfoList files = Image::readImagesFolder(sceneDir);
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name);

		QString scenename = sceneImage.getName();
		qDebug() << scenename;
		/*
		 Mat mat = phtPresentation(refImage, sceneImage,
		 reflmPath.toStdString());*/
		Point ePoint;
		double angle;
		Mat mat = testPHT(refImage, sceneImage, reflmPath.toStdString().c_str(),
				angle, ePoint);
		QString savePath = saveDir + "/" + scenename + ".JPG";
		imwrite(savePath.toStdString().c_str(), mat);
	}
}
double PHoughTransform::angleDifference(Line refLine, Line sceneLine) {
	double angle = refLine.angleBetweenLines(sceneLine);
	if (angle > 90)
		angle = 180 - angle;

	/*Point p1 = sceneLine.getP1();
	 Point p2 = sceneLine.getP2();
	 if (p1.x < refLine.getP1().x || p2.x < refLine.getP2().x)
	 return -angle;*/
	return angle;
}

Mat PHoughTransform::testPHT(Image mImage, Image sImage, string mlmPath,
		double &angleDiff, Point &ePoint) {
	vector<Point> eLandmarks;
	Mat mMatrix = mImage.getMatrixImage();
	int width = mMatrix.cols;
	int height = mMatrix.rows;

	vector<Point> mLandmarks = mImage.readLandmarksFile(mlmPath);

	vector<Line> mLines = mImage.lineSegment();
	vector<Line> sLines = sImage.lineSegment();

	Point mPoint(width / 2, height / 2);
	vector<PHTEntry> entryTable = constructTable(mLines, mPoint);
	vector<Line> maxVector;
	PHTEntry entry = matchingInScene(entryTable, sLines, width, height,
			maxVector);
	Mat result(sImage.getMatrixImage().clone());

	entry.getLine1().drawing(result);
	cv::putText(result, "line 1", entry.getLine1().getP1(),
			FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255), 1, 8);
	entry.getLine2().drawing(result);
	cv::putText(result, "line 2", entry.getLine2().getP1(),
			FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255), 1, 8);
	circle(result, mPoint, 5, Scalar(0, 0, 255), 5, 8);
	if (maxVector.size() > 0) {
		cv::line(result, maxVector[0].getP1(), maxVector[0].getP2(),
				Scalar(0, 255, 0), 2, 8);
		cv::putText(result, "line 1", maxVector[0].getP1(),
				FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0), 1, 8);
		cv::line(result, maxVector[1].getP1(), maxVector[1].getP2(),
				Scalar(0, 255, 0), 2, 8);
		cv::putText(result, "line 2", maxVector[1].getP1(),
				FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0), 1, 8);
		ePoint = refPointInScene(entry, maxVector, angleDiff, width, height);
		double angle1 = entry.getLine1().angleBetweenLines(entry.getLine2());
		double angle2 = maxVector.at(0).angleBetweenLines(maxVector.at(1));

		qDebug() << "angle Difference: " << angleDiff << ", subtraction: "
				<< abs(angle1 - angle2);
		angleDiff += abs(angle1 - angle2);
		circle(result, ePoint, 5, Scalar(0, 255, 255), 5, 8);
		eLandmarks = findLandmarks(mPoint, ePoint, angleDiff, mLandmarks, width,
				height);
		//LandmarkDetection lmd;
		//result = lmd.rotateImage(result,angleDiff, ePoint);

		for (size_t j = 0; j < mLines.size(); j++) {
			Line line = mLines.at(j);
			if (line.length() > 50)
				line.drawing(result);
		}

		for (size_t i = 0; i < eLandmarks.size(); i++) {
			circle(result, eLandmarks.at(i), 3, Scalar(0, 255, 255), 2, 8);
		}
	}
	return result;

}
} /* namespace impls_2015 */
