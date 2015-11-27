/*
 * PHoughTransform.cpp
 *
 *  Created on: Sep 23, 2015
 *  Image processing for morphometrics (IPM) Version 0.2
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
	resources = ReadResouces::readResources("data/resources/pht.rc");
}

PHoughTransform::~PHoughTransform() {
	// TODO Auto-generated destructor stub
}

/*
 * Construct the reference table around a reference point
 * @parameter 1: lines - list of lines
 * @parameter 2: refPoint - the reference point
 * @return: the list of entry contains the hough information of model
 */
vector<PHTEntry> PHoughTransform::constructTable(vector<Line> lines,
		Point refPoint) {
	vector<PHTEntry> phtransform;
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
			}
		}
	}
	return phtransform;
}

/*
 * Check the closet lines
 * @parameter 1: line1  - the first line
 * @parameter 2: line2 - the second line
 * @return: the first line and the second line are closet or not
 */
bool PHoughTransform::closetLine(Line line1, Line line2) {
	int cond1 = resources["PHTClosetC1"];
	int cond2 = resources["PHTClosetC2"];
	int cond3 = resources["PHTClosetC3"];

	cond1 = (cond1 == 0) ? 60 : cond1;
	cond2 = (cond2 == 0) ? 15 : cond2;
	cond3 = (cond3 == 0) ? 5 : cond3;

	double distance1 = line2.perpendicularDistance(line1.getP1());
	double distance2 = line2.perpendicularDistance(line1.getP2());
	if (line1.length() > cond1 && line2.length() > cond1
			&& line1.angleBetweenLines(line2) >= cond2
			&& (distance1 <= cond3 || distance2 <= cond3))
		return true;
	return false;
}

/**
 * Check the similarity between two pair of lines (ref1, ref2) and (scene1, scene2)
 * @parameter 1: ref1 - the first reference line
 * @parameter 2: ref2 - the second reference line
 * @parameter 3: scene1 - the first scene line
 * @parameter 4: scene2 - the second scene line
 * @return: check pair (ref1, ref2) similar with (scene1, scene2) or not
 */
bool PHoughTransform::similarPairLines(Line ref1, Line ref2, Line scene1,
		Line scene2) {
	int cond1 = resources["PHTSimilarC1"];
	int cond2 = resources["PHTSimilarC2"];
	int cond3 = resources["PHTSimilarC3"];

	cond1 = (cond1 == 0) ? 1 : cond1;
	cond2 = (cond2 == 0) ? 1 : cond2;
	cond3 = (cond3 == 0) ? 2 : cond3;

	double refAngle = ref1.angleBetweenLines(ref2);
	double rd1 = ref1.perpendicularDistance(ref2.getP1());
	double rd2 = ref1.perpendicularDistance(ref2.getP2());
	double rd = rd1 + rd2;

	double sceneAngle = scene1.angleBetweenLines(scene2);
	double sd1 = scene1.perpendicularDistance(scene2.getP1());
	double sd2 = scene1.perpendicularDistance(scene2.getP2());
	double sd = sd1 + sd2;

	if (abs(refAngle - sceneAngle) < cond1
			&& (abs(
					ref1.length() / scene1.length()
							- ref2.length() / scene2.length()) < cond2)
			&& (abs(rd - sd) < cond3)) {
		return true;
	}
	return false;

}

/**
 * Find the best similar pair of reference lines in table with pair of scene lines
 * @parameter 1: entryTable - the reference table
 * @parameter 2: line1 - the first scene line
 * @parameter 3: line2 - the second scene line
 * @return: the list of entries in table, which have best similarity
 */
vector<PHTEntry> PHoughTransform::findHoughSpace(vector<PHTEntry> entryTable,
		Line line1, Line line2) {
	vector<PHTEntry> entries;
	for (size_t i = 0; i < entryTable.size(); i++) {
		Line ref1 = entryTable.at(i).getLine1();
		Line ref2 = entryTable.at(i).getLine2();
		if (similarPairLines(ref1, ref2, line1, line2)) {
			PHTEntry entry = entryTable.at(i);
			entries.push_back(entry);
		}
	}
	return entries;
}

vector<vector<Line> > PHoughTransform::getClosetLines(vector<Line> sceneLines) {
	vector<vector<Line> > closetLines;
	for (size_t i = 0; i < sceneLines.size(); i++) {
		Line objLine1 = sceneLines.at(i);
		for (size_t j = 0; j < sceneLines.size(); j++) {
			Line objLine2 = sceneLines.at(j);
			if (i != j && closetLine(objLine1, objLine2)) {
				vector<Line> lines;
				lines.push_back(objLine1);
				lines.push_back(objLine2);

				closetLines.push_back(lines);
			}
		}
	}
	return closetLines;
}

/*
 * Find the pair of scene lines have the best matching with a pair of reference lines
 * @parameter 1: entryTable - the reference table
 * @parameter 2: sceneLines - list of scene lines
 * @parameter 3: width - width of image
 * @parameter 4: height - height of image
 * @parameter 5 - output: maxVector - pair of scene lines
 * @return: the entry in reference table, which have best match.
 */
PHTEntry PHoughTransform::matchingInScene(vector<PHTEntry> entryTable,
		vector<Line> sceneLines, int width, int height,
		vector<Line> &maxVector) {

	vector<vector<Line> > closetLines = getClosetLines(sceneLines);

	clock_t t1, t2;
	t1 = clock();

	// initialization an accumulator
	vector<vector<int> > acc;
	int rows = floor(sqrt(width * width + height * height));
	acc.resize(rows);
	for (int t = 0; t < rows; t++) {
		acc[t].resize(361, 0);
	}

	int maxValue = 0;
	vector<PHTEntry> maxEntries;

	for (size_t i = 0; i < closetLines.size(); i++) {
		vector<Line> lines = closetLines.at(i);
		if (lines.size() > 0) {
			Line objLine1 = lines.at(0);
			Line objLine2 = lines.at(1);
			vector<PHTEntry> entries = findHoughSpace(entryTable, objLine1,
					objLine2);
			for (size_t j = 0; j < entries.size(); j++) {
				PHTEntry entry = entries.at(j);
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
								maxEntries.clear();
								maxVector.push_back(objLine1);
								maxVector.push_back(objLine2);
								maxValue = acc[distance][angle];
								maxEntries.push_back(entry);
							} else {
								if (k == 0
										&& acc[distance][angle] == maxValue) {
									maxVector.push_back(objLine1);
									maxVector.push_back(objLine2);
									maxEntries.push_back(entry);
								}
							}
						}
					}
				}
			}
		}
	}
	cout << "\n Number of maxVector: " << maxVector.size();
	cout << "\n Number of max entry: " << maxEntries.size();
	t2 = clock();
	cout << "\nTime find matching in scene: "
			<< ((float) t2 - (float) t1) / CLOCKS_PER_SEC << " seconds";
	return maxEntries.at(maxEntries.size()-1);
}

/*
 * Estimate the reference point in a scene image
 * @parameter 1: entry - the matching entry
 * @parameter 2: matchLines - the pair of scene lines
 * @parameter 3 - output: angleDiff - the difference angle between two images
 * @parameter 4: refLandmarks - list of reference landmarks
 * @parameter 5: width - width of image
 * @parameter 6: height - height of image
 * @return: the location of reference point on scene image
 */
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
	cout <<"Reference point in scene: " << inter.x << ", " << inter.y;
	return inter;
}

/*
 * Presentation the estimated landmarks
 * @parameter 1: refImage - reference image
 * @parameter 2: sceneImage - scene image
 * @parameter 3: reflmPath - the reference landmarks file
 * @parameter 4 - output: esLandmarks - list of estimated landmarks
 * @return: the matrix presented the estimated landmarks
 */
Mat PHoughTransform::phtPresentation(Image refImage, Image sceneImage,
		string reflmPath, vector<Point> &esLandmarks,
		Image::SegmentMethod sgmethod) {

	QString scenename = sceneImage.getName();
	double angleDiff;
	Point ePoint;
	esLandmarks = estimateLandmarks(refImage, sceneImage, reflmPath, angleDiff,
			ePoint, sgmethod);

	Mat sceneMat = sceneImage.getMatrixImage();
	cv::Mat mat(sceneMat.clone());

	cout << "\n"<< "Angle Diff: " << angleDiff;
	for (size_t t = 0; t < esLandmarks.size(); t++) {
		circle(mat, esLandmarks.at(t), 3, Scalar(0, 255, 255), 2, 8);

		stringstream ss;
		string s;
		ss << t;
		s = ss.str();
		cv::putText(mat, s, esLandmarks.at(t), FONT_HERSHEY_COMPLEX, 1,
				Scalar(255, 0, 0), 1, 8);
	}
	return mat;
}

/*
 * Estimate the reference landmarks on a scene image
 * @parameter 1: mImage - model image
 * @parameter 2: sImage - scene image
 * @parameter 3: mlmPath - reference landmarks file
 * @parameter 4 - output: angleDiff - the angle difference between two images
 * @paremeter 5 - output: ePoint - the reference point on scene image
 * @return: list of estimated landmarks
 */
vector<Point> PHoughTransform::estimateLandmarks(Image mImage, Image sImage,
		string mlmPath, double &angleDiff, Point &ePoint,
		Image::SegmentMethod sgmethod) {
	vector<Point> eLandmarks;
	Mat mMatrix = mImage.getMatrixImage();
	int width = mMatrix.cols;
	int height = mMatrix.rows;

	vector<Point> mLandmarks = mImage.readLandmarksFile(mlmPath);

	int mthresh = 0, sthresh = 0;
	vector<Line> mLines = mImage.lineSegment(sgmethod, mthresh);
	vector<Line> sLines = sImage.lineSegment(sgmethod, sthresh);

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

/*
 * Find the estimated landmarks based on the relative of reference landmarks and reference point
 * @parameter 1: refPoint - the reference point
 * @parameter 2: esPoint - the reference point on the scene image
 * @parameter 3: refLandmarks - list of reference landmarks
 * @parameter 4: width - width of image
 * @parameter 5: height - height of image
 * @parameter 6 - output: positive - number of estimated landmarks, which have positive coordinate
 * @return: list of estimated landmarks
 */
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

/*
 * Find the new location of a point when image rotated
 * @parameter 1: point - the object point
 * @parameter 2: angleDiff - the angle rotate
 * @parameter 3: refPoint - the center to rotate
 * @return: new coordinate of object point
 */
Point PHoughTransform::newLocation(Point point, double angleDiff,
		Point refPoint) {
	int x, y;
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

/*
 * Estimate the landmarks of a reference image on a list of image in folder
 * @parameter 1: refImage - the reference image
 * @parameter 2: reflmPath - the reference landmarks file
 * @parameter 3: sceneDir - the scene images folder
 * @parameter 4: scenelmDir - the folder contains the original landmarks file of scene images
 * @parameter 5: saveDir - the save folder
 */
void PHoughTransform::phtDirectory(Image refImage, string reflmPath,
		string sceneDir, string scenelmDir, string saveDir,
		Image::SegmentMethod sgmethod, int save) {

	QFileInfoList files = Image::readImagesFolder(sceneDir);
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name.toStdString());

		QString scenename = sceneImage.getName();
		qDebug() << scenename;

		vector<Point> esLandmarks;
		Mat mat = phtPresentation(refImage, sceneImage, reflmPath, esLandmarks,
				sgmethod);
		string saveRoot = saveDir;
		string savePath =
				saveRoot.append("/").append(scenename.toStdString()).append(
						".JPG");
		imwrite(savePath.c_str(), mat);
		if (save == 1) {
			string tpsRoot = saveDir;
			string fileTPS =
					tpsRoot.append("/").append(scenename.toStdString()).append(
							".TPS");
			saveEstLandmarks(esLandmarks, fileTPS);
		}
	}
}
void PHoughTransform::saveEstLandmarks(vector<Point> esLandmarks,
		string savePath) {

	ofstream of(savePath.c_str());
	of << "LM = " << esLandmarks.size() << "\n";
	for (size_t t = 0; t < esLandmarks.size(); t++) {
		of << esLandmarks.at(t).x << " " << esLandmarks.at(t).y << "\n";
	}
	of << "FILE = " << savePath.c_str();
	of.close();
}

/*
 * Compute the angle difference between two lines
 * @parameter 1: refLine - the reference line
 * @parameter 2: sceneLine - the scene line
 * @return: angle between two lines
 */
double PHoughTransform::angleDifference(Line refLine, Line sceneLine) {
	double angle = refLine.angleBetweenLines(sceneLine);
	if (angle > 90)
		angle = 180 - angle;
	return angle;
}
} /* namespace impls_2015 */
