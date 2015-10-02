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

void PHoughTransform::test(vector<Line> refLines, vector<Line> sceneLines,
		int width, int height) {

	qDebug() << "test accumulator";
	PHoughTransform pht;
	vector<PHTEntry> entryTable = pht.constructTable(refLines,
			cv::Point(width / 2, height / 2));
	vector<Line> maxVector;
	double angleDiff;
	PHTEntry entry = pht.matchingInScene(entryTable, sceneLines,
			width,height, maxVector);
	Point refPoint = pht.refPointInScene(entry,maxVector,angleDiff);

	int maxValue = 0;

	cv::Mat mat(cv::Size(2000, 2000), CV_8UC3, cv::Scalar(0, 0, 0));
	for (size_t t = 0; t < sceneLines.size(); t++) {
		Line l = sceneLines.at(t);
		mat = l.drawing(mat);
	}
	vector<Point> landmarks;
	landmarks.push_back(cv::Point(450, 600));
	landmarks.push_back(cv::Point(650, 550));
	landmarks.push_back(cv::Point(600, 700));
	landmarks.push_back(cv::Point(400, 800));

	circle(mat, refPoint, 2, Scalar(0, 0, 255), 1, 8);
	for (size_t t = 0; t < landmarks.size(); t++) {
		Point lm = landmarks.at(t);
		int px = width / 2 - lm.x;
		int py = height / 2 - lm.y;
		int x = lm.x;
		int y = lm.y;
		if(angleDiff != 0){
			x = x*cos(round(angleDiff) * M_PI/180) + y*sin(round(angleDiff) * M_PI/180);
			y = -x*sin(round(angleDiff) * M_PI/180) + y*cos(round(angleDiff) * M_PI/180);
			qDebug()<<x<<","<<y;
		}else{
			x = refPoint.x - px;
			y = refPoint.y - py;
		}
		circle(mat, cv::Point(x,y), 2,
				Scalar(0, 255, 255), 1, 8);
	}
	imwrite("test/images/scene.jpg", mat);
	cv::namedWindow("DetectLines", CV_WINDOW_AUTOSIZE);
	cv::imshow("DetectLines", mat);

	/*vector<Line> detectLines;

	 for (size_t t = 0; t < peaks.size(); t++) {
	 HoughSpace hs = peaks.at(t);

	 int p1x = 0;
	 int p1y = round(( hs.getDistance() - p1x * cos(hs.getAngle() * M_PI / 180))/sin(hs.getAngle() * M_PI / 180));
	 int p2x = width;
	 int p2y = round(( hs.getDistance() - p2x * cos(hs.getAngle() * M_PI / 180))/sin(hs.getAngle() * M_PI / 180));
	 detectLines.push_back(Line(Point(p1x, p1y), Point(p2x, p2y)));
	 qDebug() << "peak index: " << hs.getDistance() << ", " << hs.getAngle();
	 }
	 cv::Mat mat(cv::Size(2000, 2000), CV_8UC3, cv::Scalar(0, 0, 0));
	 for(size_t t = 0; t < detectLines.size();t++){
	 Line l = detectLines.at(t);
	 mat = l.drawing(mat);
	 for(size_t k = 0; k < detectLines.size();k++){
	 if(k != t){
	 Line l2 = detectLines.at(k);
	 cv::Point point = l.intersection(l2);
	 qDebug()<<"Intersect: "<<point.x<<", "<<point.y;
	 }
	 }
	 }
	 cv::namedWindow("DetectLines", CV_WINDOW_AUTOSIZE);
	 cv::imshow("DetectLines", mat);*/
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
		HoughSpace hs1;
		hs1.setAngle(hs1.computeAngle(line1, refPoint));
		hs1.setDistance(hs1.computeDistance(line1, refPoint));
		for (size_t j = 0; j < lines.size(); j++) {
			Line line2 = lines.at(j);
			HoughSpace hs2;
			hs2.setAngle(hs2.computeAngle(line2, refPoint));
			hs2.setDistance(hs2.computeDistance(line2, refPoint));
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

/**
 * Indicate the similarity of two pair lines
 */
bool PHoughTransform::similarPairLines(Line ref1, Line ref2, Line scene1,
		Line scene2) {
	double refAngle = ref1.angleBetweenLines(ref2);
	double refscaleRatio = ref1.length() / ref2.length();
	Line temp(ref1.getP1(), ref2.getP1());
	double reflength = temp.length();

	double sceneAngle = scene1.angleBetweenLines(scene2);
	double scenescaleRatio = scene1.length() / scene2.length();
	Line temp2(scene1.getP1(), scene2.getP1());
	double scenelength = temp2.length();

	if (refAngle == sceneAngle && (refscaleRatio - scenescaleRatio) >= 0
			&& refscaleRatio - scenescaleRatio < 0.5
			&& (reflength - scenelength) >= 0
			&& (reflength - scenelength) < 0.5) {
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
			return entryTable.at(i);
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
			if (i != j) {
				PHTEntry entry = findHoughSpace(entryTable, objLine1, objLine2);
				vector<HoughSpace> hspace = entry.getHoughSpaces();
				for (size_t k = 0; k < hspace.size(); k++) {
					HoughSpace hsp = hspace.at(k);
					int angle = round(hsp.getAngle());
					int distance = round(hsp.getDistance());
					acc[distance][angle] += 1;
					if (acc[distance][angle] > maxValue) {
						maxVector.clear();
						maxVector.push_back(objLine1);
						maxVector.push_back(objLine2);
						maxValue = acc[distance][angle];
						maxEntry = entry;
					}
				}
			}
		}
	}

	return maxEntry;
	/*qDebug() << "Size of vector: " << maxVector.size();
	 for (size_t t = 0; t < maxVector.size(); t++) {
	 maxVector[t].toString();
	 }
	 Line objl1 = maxVector.at(0);
	 Line objl2 = maxVector.at(1);
	 qDebug() << "Point intersection: " << objl1.intersection(objl2).x
	 << ", " << objl1.intersection(objl2).y;
	 qDebug() << "Entry";
	 Line lineEntry1 = maxEntry.getLine1();
	 Line lineEntry2 = maxEntry.getLine2();

	 HoughSpace hs1, hs2;
	 lineEntry1.toString();
	 lineEntry2.toString();
	 cv::Point inter = objl1.intersection(lineEntry1);
	 Line lineOrient;
	 if (inter.x == -1 && inter.y == -1) { // parallel lines
	 hs1 = maxEntry.getHoughSpaces().at(0);
	 hs2 = maxEntry.getHoughSpaces().at(1);
	 lineOrient = lineEntry1;
	 } else {
	 hs1 = maxEntry.getHoughSpaces().at(1);
	 hs2 = maxEntry.getHoughSpaces().at(0);
	 lineOrient = lineEntry2;
	 }
	 qDebug() << "angle: " << objl1.angleBetweenLines(lineOrient);
	 vector<double> pline1 = objl1.parallelLine(hs1.getDistance());
	 vector<double> pline2 = objl2.parallelLine(hs2.getDistance());
	 cv::Point intersect = objl1.intersection(pline1, pline2);
	 qDebug() << "Reference point: " << intersect.x << ", " << intersect.y;

	 return intersect;*/
}

Point PHoughTransform::refPointInScene(PHTEntry entry, vector<Line> matchLines,
		double &angleDiff) {
	Line objl1 = matchLines.at(0);
	Line objl2 = matchLines.at(1);
	qDebug() << "Point intersection: " << objl1.intersection(objl2).x << ", "
			<< objl1.intersection(objl2).y;
	qDebug() << "Entry";
	Line lineEntry1 = entry.getLine1();
	Line lineEntry2 = entry.getLine2();

	HoughSpace hs1, hs2;
	lineEntry1.toString();
	lineEntry2.toString();
	cv::Point inter = objl1.intersection(lineEntry1);
	Line lineOrient;
	if (inter.x == -1 && inter.y == -1) { // parallel lines
		hs1 = entry.getHoughSpaces().at(0);
		hs2 = entry.getHoughSpaces().at(1);
		lineOrient = lineEntry1;
	} else {
		hs1 = entry.getHoughSpaces().at(1);
		hs2 = entry.getHoughSpaces().at(0);
		lineOrient = lineEntry2;
	}
	angleDiff = objl1.angleBetweenLines(lineOrient);
	qDebug() << "angle: " << angleDiff;
	vector<double> pline1 = objl1.parallelLine(hs1.getDistance());
	vector<double> pline2 = objl2.parallelLine(hs2.getDistance());
	cv::Point intersect = objl1.intersection(pline1, pline2);
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
	return refPointInScene(entry, maxVector, angleDiff);
}
vector<vector<int> > PHoughTransform::createAccumulator(Point refPoint,
		vector<Line> sceneLines, int width, int height, int &maxValue) {
	maxValue = 0;
	vector<vector<int> > acc;
	int rows = floor(sqrt(width * width + height * height));
	acc.resize(rows);
	HoughSpace hsp;
	for (int t = 0; t < rows; t++) {
		acc[t].resize(361, 0);
	}
	for (size_t i = 0; i < sceneLines.size(); i++) {
		Line line = sceneLines.at(i);
		int angle = round(hsp.computeAngle(line, refPoint));
		int distance = round(hsp.computeDistance(line, refPoint));
		acc[distance][angle] += 1;
		if (acc[distance][angle] > maxValue) {
			maxValue = acc[distance][angle];
		}
	}
	return acc;
}

vector<vector<int> > PHoughTransform::createAccumulator2(Point refPoint,
		vector<Line> sceneLines, int width, int height, int &maxValue) {
	maxValue = 0;
	vector<vector<int> > acc;
	int rows = floor(sqrt(width * width + height * height));
	acc.resize(rows);
	HoughSpace hsp;
	for (int t = 0; t < rows; t++) {
		acc[t].resize(181, 0);
	}
	for (size_t i = 0; i < sceneLines.size(); i++) {
		Line line = sceneLines.at(i);
		Point p1 = line.getP1();
		for (int ang = 0; ang <= 180; ang++) {
			int distance = p1.x * cos(ang * M_PI / 180)
					+ p1.y * sin(ang * M_PI / 180);
			if (distance >= 0) {
				acc[distance][ang] += 1;
				if (acc[distance][ang] > maxValue) {
					maxValue = acc[distance][ang];
				}
			}
		}
		Point p2 = line.getP2();
		for (int ang = 0; ang <= 180; ang++) {
			int distance = p2.x * cos(ang * M_PI / 180)
					+ p2.y * sin(ang * M_PI / 180);
			if (distance >= 0) {
				acc[distance][ang] += 1;
				if (acc[distance][ang] > maxValue) {
					maxValue = acc[distance][ang];
				}
			}
		}
	}
	return acc;
}
vector<HoughSpace> PHoughTransform::peaksOfAccumulator(
		vector<vector<int> > accumulator, int maxValue) {
	vector<HoughSpace> peaks;
	for (size_t i = 0; i < accumulator.size(); i++) {
		for (size_t j = 0; j < accumulator[0].size(); j++) {
			if (accumulator[i][j] == maxValue) {
				HoughSpace hs;
				hs.setDistance(i);
				hs.setAngle(j);
				peaks.push_back(hs);
			}
		}
	}
	return peaks;
}
} /* namespace impls_2015 */
