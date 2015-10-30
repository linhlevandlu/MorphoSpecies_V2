/*
 * PHoughTransform.cpp
 *
 *  Created on: Sep 23, 2015
 *  Author: linh
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
	ofstream of("results/table_acc.txt");
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
			&& line1.angleBetweenLines(line2) >= 15
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
	double rd1 = ref1.perpendicularDistance(ref2.getP1());
	double rd2 = ref1.perpendicularDistance(ref2.getP2());
	double rd = rd1 + rd2;

	double sceneAngle = scene1.angleBetweenLines(scene2);
	Line temp2(scene1.getP1(), scene2.getP1());
	double sd1 = scene1.perpendicularDistance(scene2.getP1());
	double sd2 = scene1.perpendicularDistance(scene2.getP2());
	double sd = sd1 + sd2;

	if (abs(refAngle - sceneAngle) < 1
			&& (abs(
					ref1.length() / scene1.length()
							- ref2.length() / scene2.length()) < 1)
			&& (abs(rd - sd) < 2)) {
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
		double &angleDiff, vector<Point> refLandmarks, int width, int height) {

	Point inter(0, 0);
	Point refPoint(width / 2, height / 2);
	if (matchLines.size() <= 0)
		return inter;
	Line objl1 = matchLines.at(0);
	Line objl2 = matchLines.at(1);

	Line lineEntry1 = entry.getLine1();
	Line lineEntry2 = entry.getLine2();

	HoughSpace hs1, hs2;
	hs1 = entry.getHoughSpaces().at(0);
	hs2 = entry.getHoughSpaces().at(1);

	// display the angle
	//double angle1 = lineEntry1.angleBetweenLines(lineEntry2);
	//double angle2 = objl1.angleBetweenLines(objl2);
	double angle3 = lineEntry1.angleBetweenLines(objl1);
	double angle4 = lineEntry1.angleBetweenLines(objl2);

	vector<Point> intersects1 = objl1.interParallel(objl1, objl2,
			hs1.getDistance(), hs2.getDistance(), width, height);
	int max = 0;
	vector<Point> estLM;
	vector<double> angles;
	for (size_t i = 0; i < intersects1.size(); i++) {
		Point esPoint = intersects1.at(i);
		int positive = 0;
		vector<Point> lms = findLandmarks(refPoint, esPoint, refLandmarks,
				width, height, positive);
		if (positive > max) {
			estLM.clear();
			estLM.push_back(esPoint);
			angles.clear();
			angles.push_back(angle3);
			max = positive;
		} else {
			if (positive == max) {
				estLM.push_back(esPoint);
				angles.push_back(angle3);
			}
		}
	}

	vector<Point> intersects2 = objl1.interParallel(objl1, objl2,
			hs2.getDistance(), hs1.getDistance(), width, height);
	for (size_t i = 0; i < intersects2.size(); i++) {
		Point esPoint = intersects2.at(i);
		int positive = 0;
		vector<Point> lms = findLandmarks(refPoint, esPoint, refLandmarks,
				width, height, positive);
		if (positive > max) {
			estLM.clear();
			estLM.push_back(esPoint);
			angles.clear();
			angles.push_back(angle4);
			max = positive;
		} else {
			if (positive == max) {
				estLM.push_back(esPoint);
				angles.push_back(angle4);
			}
		}
	}

	qDebug() << "Size of intersections: " << estLM.size();
	if (estLM.size() == 1) {
		inter = estLM.at(0);
		angleDiff = angles.at(0);
	} else {
		double angleds = 180;
		for (size_t i = 0; i < estLM.size(); i++) {
			Point es = estLM.at(i);
			double angle = angles.at(i);
			if (angle <= angleds) {
				angleds = angle;
				angleDiff = angleds;
				inter = es;
			}
		}
	}

	qDebug() << "Angle diff: " << angleDiff;
	qDebug() << "Point intersection: " << inter.x << ", " << inter.y;
	return inter;
}

Mat PHoughTransform::phtPresentation(Image refImage, Image sceneImage,
		string reflmPath, vector<Point> &esLandmarks) {

	QString scenename = sceneImage.getName();
	double angleDiff;
	Point ePoint;
	esLandmarks = estimateLandmarks(refImage, sceneImage, reflmPath, angleDiff,
			ePoint);

	Mat sceneMat = sceneImage.getMatrixImage();
	cv::Mat mat(sceneMat.clone());

	qDebug() << "angle Diff: " << angleDiff;
	for (size_t t = 0; t < esLandmarks.size(); t++) {
		circle(mat, esLandmarks.at(t), 3, Scalar(0, 255, 255), 2, 8);
		stringstream ss;
		ss << t;
		string s;
		s = ss.str();
		cv::putText(mat, s, esLandmarks.at(t), FONT_HERSHEY_COMPLEX, 1,
				Scalar(255, 0, 0), 1, 8);

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
		ePoint = refPointInScene(entry, maxVector, angleDiff, mLandmarks, width,
				height);

		double angle1 = entry.getLine1().angleBetweenLines(entry.getLine2());
		double angle2 = maxVector.at(0).angleBetweenLines(maxVector.at(1));
		angleDiff += abs(angle1 - angle2);
		int positive = 0;
		eLandmarks = findLandmarks(mPoint, ePoint, mLandmarks, width, height,
				positive);
	}
	return eLandmarks;

}
vector<Point> PHoughTransform::findLandmarks(Point refPoint, Point esPoint,
		vector<Point> refLandmarks, int width, int height, int &positive) {
	vector<Point> esLandmarks;
	positive = 0;
	for (size_t t = 0; t < refLandmarks.size(); t++) {
		Point temp = refLandmarks.at(t);
		Point lm(temp.x, height - temp.y);
		int px = refPoint.x - esPoint.x;
		int py = refPoint.y - esPoint.y;
		int x;
		int y;
		x = lm.x - px;
		if (py > 0)
			y = lm.y - py;
		else
			y = lm.y + py;
		esLandmarks.push_back(Point(x, y));
		if (x >= 0 && x <= width && y >= 0 && y <= height) {
			positive += 1;
		}
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

		vector<Point> esLandmarks;
		Mat mat = phtPresentation(refImage, sceneImage, reflmPath.toStdString(),
				esLandmarks);
		QString fileTPS(saveDir + "/" + scenename + ".TPS");
		ofstream of(fileTPS.toStdString().c_str());
		of << "LM = " << esLandmarks.size() << "\n";
		for (size_t t = 0; t < esLandmarks.size(); t++) {
			of << esLandmarks.at(t).x << " " << esLandmarks.at(t).y << "\n";
		}
		of << "IMAGE = " << scenename.toStdString().c_str() << ".JPG";
		of.close();
		QString savePath = saveDir + "/" + scenename + ".JPG";
		imwrite(savePath.toStdString().c_str(), mat);
		if (i == 293)
			break;
	}
}
double PHoughTransform::angleDifference(Line refLine, Line sceneLine) {
	double angle = refLine.angleBetweenLines(sceneLine);
	if (angle > 90)
		angle = 180 - angle;
	return angle;
}

/*Mat PHoughTransform::testPHT(Image mImage, Image sImage, string mlmPath,
		double &angleDiff, Point &ePoint) {
	vector<Point> eLandmarks;
	Mat mMatrix = mImage.getMatrixImage();
	int width = mMatrix.cols;
	int height = mMatrix.rows;

	vector<Point> mLandmarks = mImage.readLandmarksFile(mlmPath);
	qDebug() << "total ref landmarks: " << mLandmarks.size();

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
		vector<double> angles;
		ePoint = refPointInScene(entry, maxVector, angleDiff, mLandmarks, width,
				height);

		double angle1 = entry.getLine1().angleBetweenLines(entry.getLine2());
		double angle2 = maxVector.at(0).angleBetweenLines(maxVector.at(1));

		qDebug() << "angle Difference: " << angleDiff << ", subtraction: "
				<< abs(angle1 - angle2);
		angleDiff += abs(angle1 - angle2);
		circle(result, ePoint, 5, Scalar(0, 255, 255), 5, 8);

		int positive = 0;
		eLandmarks = findLandmarks(mPoint, ePoint, mLandmarks, width, height,
				positive);

		for (size_t i = 0; i < eLandmarks.size(); i++) {
			circle(result, eLandmarks.at(i), 3, Scalar(0, 255, 255), 2, 8);
			stringstream ss;
			ss << i;
			string s;
			s = ss.str();
			cv::putText(result, s, eLandmarks.at(i), FONT_HERSHEY_COMPLEX, 1,
					Scalar(255, 0, 0), 1, 8);
		}
	}
	return result;
}*/
} /* namespace impls_2015 */
