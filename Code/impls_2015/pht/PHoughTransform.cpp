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

void PHoughTransform::test(vector<Line> refLines, vector<Line> sceneLines,
		int width, int height) {

	std::sort(refLines.begin(), refLines.end());
	std::sort(sceneLines.begin(), sceneLines.end());
	qDebug() << "test accumulator";
	PHoughTransform pht;
	Point hPoint(width / 2, height / 2);
	qDebug() << "Reference model point: (" << hPoint.x << ", " << hPoint.y
			<< ")";

	vector<PHTEntry> entryTable = pht.constructTable(refLines, hPoint);
	qDebug() << "finished table";
	vector<Line> maxVector;
	double angleDiff;
	PHTEntry entry = pht.matchingInScene(entryTable, sceneLines, width, height,
			maxVector);
	qDebug() << "matching finished";
	Point refPoint = pht.refPointInScene(entry, maxVector, angleDiff, width,
			height);

	cv::Mat mat(cv::Size(width, height), CV_8UC3, cv::Scalar(0, 0, 0));
	for (size_t t = 0; t < sceneLines.size(); t++) {
		Line l = sceneLines.at(t);
		if (l.length() > 20)
			mat = l.drawing(mat);
	}

	vector<Point> landmarks;
	landmarks = pht.readLandmarksFile(
			"/home/linh/Desktop/landmarks/test.TPS");

	for (size_t t = 0; t < landmarks.size(); t++) {
		Point landm = landmarks.at(t);
		circle(mat, cv::Point(landm.x, landm.y), 7, Scalar(0, 0, 255),
				1, 4);
	}

	circle(mat, hPoint, 5, Scalar(0, 0, 255), 1, 8);
	circle(mat, cv::Point(refPoint.x, refPoint.y), 5, Scalar(0, 255, 0), 1, 8);

	for (size_t t = 0; t < landmarks.size(); t++) {
		Point lm = landmarks.at(t);
		int px = hPoint.x - refPoint.x;
		int py = hPoint.y - refPoint.y;
		int x, refX;
		int y, refY;
		if (angleDiff != 0) {
			//  rotation
			x = lm.x * cos(round(angleDiff) * M_PI / 180)
					- lm.y * sin(round(angleDiff) * M_PI / 180) + hPoint.x;
			y = lm.x * sin(round(angleDiff) * M_PI / 180)
					+ lm.y * cos(round(angleDiff) * M_PI / 180) + hPoint.y;

			// and translation
			refX = hPoint.x * cos(round(angleDiff) * M_PI / 180)
					- hPoint.y * sin(round(angleDiff) * M_PI / 180) + hPoint.x;
			refY = hPoint.x * sin(round(angleDiff) * M_PI / 180)
					+ hPoint.y * cos(round(angleDiff) * M_PI / 180) + hPoint.y;
			qDebug() << refX << ", " << refY;
			px = refX - refPoint.x;
			py = refY - refPoint.y;
			x = x - px;
			y = y - py;

		} else {
			x = lm.x - px;
			y = lm.y - py;
		}
		qDebug() << x << "," << y;
		circle(mat, cv::Point(x, y), 2, Scalar(0, 255, 255), 1, 8);
	}
	imwrite("test/images/scene.jpg", mat);
	cv::namedWindow("DetectLines", CV_WINDOW_AUTOSIZE);
	cv::imshow("DetectLines", mat);

}

/**
 * Getting the landmarks from a file and saving into a vector
 */
vector<Point> PHoughTransform::readLandmarksFile(QString filePath) {
	ifstream openFile(filePath.toStdString().c_str());
	string lineText;
	vector<Point> landmarks;
	if (openFile.is_open()) {
		getline(openFile, lineText);
		QString firsLine = (lineText.c_str());
		int numOfLandmarks = firsLine.split("=").at(1).toInt();
		int i = 0;
		while (i < numOfLandmarks) {
			getline(openFile, lineText);
			QString line = lineText.c_str();
			QStringList listString = line.split(" ");
			cv::Point point1(listString.at(0).toDouble(),
					listString.at(1).toDouble());
			landmarks.push_back(point1);
			i++;
		}
		openFile.close();
	}
	return landmarks;
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
			if (closetLine(line1, line2)) {
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
							<< (line1.getP2().y) << ")" << "("
							<< (line2.getP1().x) << "," << (line2.getP1().y)
							<< ")" << " (" << (line2.getP2().x) << ","
							<< (line2.getP2().y) << ")" << "(" << hs1.getAngle()
							<< "," << hs1.getDistance() << ")" << " ("
							<< (hs2.getAngle()) << "," << (hs2.getDistance())
							<< ")" << "\n";
				}
			}
		}
	}
	of.close();
	return phtransform;
}

bool PHoughTransform::closetLine(Line line1, Line line2) {

	double distance1 = line2.perpendicularDistance(line1.getP1());
	double distance2 = line2.perpendicularDistance(line1.getP2());
	if ((line1.length() > 20 && line2.length() > 20)
			&& (distance1 <= 10 || distance2 <= 10))
		return true;
	return false;
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

	if (abs(refAngle - sceneAngle) < 1
			&& abs(refscaleRatio - scenescaleRatio) < 2.5
			&& abs(reflength - scenelength) < 0.5) {
		return true;
	}
	return false;

	/*int dx11 = abs(ref1.getP1().x - ref1.getP2().x);
	 int dy11 = abs(ref1.getP1().y - ref1.getP2().y);
	 int dx12 = abs(ref2.getP1().x - ref2.getP1().x);
	 int dy12 = abs(ref2.getP1().y - ref2.getP2().y);

	 int dx21 = abs(scene1.getP1().x - scene1.getP1().x);
	 int dy21 = abs(scene1.getP2().y - scene1.getP2().y);
	 int dx22 = abs(scene2.getP2().x - scene2.getP1().x);
	 int dy22 = abs(scene2.getP2().y - scene2.getP2().y);
	 if(abs(dx11 - dx21) <= 5 && abs(dy11 - dy21) <= 5 && abs(dx12 - dx22) <= 5 && abs(dy12 - dy22) <= 5)
	 return true;
	 return false;*/

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
	/*qDebug() << "maximum value in HS: " << maxValue;
	 if (maxValue > 0) {
	 cv::Mat matImg(cv::Size(361, rows), CV_8UC3, cv::Scalar(0, 0, 0));
	 for (int t = 0; t < rows; t++) {
	 for (int k = 0; k < acc[t].size(); k++) {
	 if (acc[t][k] == maxValue) {
	 circle(matImg, cv::Point(k, t), 2, Scalar(255, 255, 255), 1,
	 8);
	 qDebug() << "x-y: " << k << ", " << t;
	 }
	 }
	 }
	 imwrite("test/images/accumulator.jpg", matImg);
	 cv::namedWindow("Accumulator", CV_WINDOW_AUTOSIZE);
	 cv::imshow("Accumulator", matImg);
	 qDebug() << "max vector: " << maxVector.size();
	 }*/
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
	cv::Point inter = objl1.intersection(lineEntry1);
	Line lineOrient;
	if ((inter.x == -1 && inter.y == -1)
			|| (abs(lineEntry1.length() - objl1.length())
					<= abs(lineEntry1.length() - objl2.length()))) { // matching lines
		hs1 = entry.getHoughSpaces().at(0);
		hs2 = entry.getHoughSpaces().at(1);
		lineOrient = lineEntry1;
		qDebug() << "1";
	} else {
		hs1 = entry.getHoughSpaces().at(1);
		hs2 = entry.getHoughSpaces().at(0);
		lineOrient = lineEntry2;
		qDebug() << "2";
	}
	lineOrient.toString();
	angleDiff = objl1.angleBetweenLines(lineOrient);
	qDebug() << "angle: " << angleDiff;

	vector<vector<double> > pline1 = objl1.parallelLine(hs1.getDistance());
	vector<vector<double> > pline2 = objl2.parallelLine(hs2.getDistance());
	cv::Point intersect;
	for (size_t i = 0; i < pline1.size(); i++) {
		vector<double> line1 = pline1.at(i);
		for (size_t j = 0; j < pline2.size(); j++) {
			vector<double> line2 = pline2.at(j);
			Point temp = objl1.intersection(line1, line2);
			//qDebug()<<"i,j "<<i<<", "<<j;
			if (temp.x >= 0 && temp.x <= width && temp.y >= 0
					&& temp.y <= height) {
				qDebug() << hs1.getAngle();
				qDebug() << hs1.computeAngle(objl1, temp) + round(angleDiff);

				if (round(hs1.computeAngle(objl1, temp) + round(angleDiff))
						== round(hs1.getAngle())
						&& round(
								hs1.computeAngle(objl2, temp)
										+ round(angleDiff))
								== round(hs2.getAngle())) {
					intersect = temp;
					//break;
				}
			}
		}
	}

	//cv::Point intersect = objl1.intersection(pline1, pline2);
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

vector<TableEntry> PHoughTransform::referenceTable(vector<Line> refLines,
		Point refPoint) {
	vector<TableEntry> refTable;
	for (size_t i = 0; i < refLines.size(); i++) {
		Line line = refLines.at(i);
		TableEntry entry;
		HoughSpace hsp;
		hsp.setAngle(hsp.computeAngle(line, refPoint));
		hsp.setDistance(hsp.computeDistance(line, refPoint));

		entry.setRefLine(line);
		entry.setHoughValue(hsp);

		refTable.push_back(entry);
	}
	return refTable;
}

bool PHoughTransform::similarLine(Line line1, Line line2, double &diff) {
	// orientation and position
	Line refLine(Point(0, 0), Point(100, 0));
	double angle1 = refLine.angleBetweenLines(line1);
	double angle2 = refLine.angleBetweenLines(line2);
	double ratio = angle1 / angle2;
	if (abs(ratio) > 0.8 && abs(ratio) < 1.2) {
		diff = abs(ratio - 1);
		return true;
	}
	return false;
}

TableEntry PHoughTransform::matchingEntry(vector<TableEntry> entryTable,
		Line sceneLine) {

	std::sort(entryTable.begin(), entryTable.end());

	TableEntry entry;
	double diff, match = 1;
	for (size_t i = 0; i < entryTable.size(); i++) {
		Line ref1 = entryTable.at(i).getLine();
		if (similarLine(ref1, sceneLine, diff)) {
			if (diff <= match) {
				match = diff;
				entry = entryTable.at(i);
			}
		}
	}
	return entry;
}

vector<TableEntry> PHoughTransform::matchingInScene(
		vector<TableEntry> entryTable, vector<Line> sceneLines, int width,
		int height) {
	vector<vector<int> > acc;
	int rows = floor(sqrt(width * width + height * height));
	acc.resize(rows);
	for (int t = 0; t < rows; t++) {
		acc[t].resize(361, 0);
	}

	int maxValue = 0;

	vector<TableEntry> entries;
	for (size_t i = 0; i < sceneLines.size(); i++) {
		Line line1 = sceneLines.at(i);
		for (size_t j = 0; j < sceneLines.size(); j++) {
			if (i != j) {
				Line line2 = sceneLines.at(j);
				if (closetLine(line1, line2)) {

					TableEntry entry1 = matchingEntry(entryTable, line1);
					HoughSpace hsp1 = entry1.getHoughValue();
					double angle1 = hsp1.getAngle();
					double distance1 = hsp1.getDistance();
					if (entry1.getLine().isNull() && !isnan(angle1)
							&& angle1 > 0 && !isnan(distance1)
							&& distance1 > 0) {
						acc[distance1][angle1] += 1;
						TableEntry tentry1;
						tentry1.setRefLine(line1);
						tentry1.setHoughValue(hsp1);

						if (acc[distance1][angle1] > maxValue) {
							entries.clear();
							maxValue = acc[distance1][angle1];
							entries.push_back(tentry1);
						} else {
							if (acc[distance1][angle1] == maxValue) {
								entries.push_back(tentry1);
							}
						}
					}

					TableEntry entry2 = matchingEntry(entryTable, line2);
					HoughSpace hsp2 = entry2.getHoughValue();
					double angle2 = hsp2.getAngle();
					double distance2 = hsp2.getDistance();
					if (!entry2.getLine().isNull() && !isnan(angle2)
							&& angle2 > 0 && !isnan(distance2)
							&& distance2 > 0) {
						acc[distance2][angle2] += 1;
						TableEntry tentry2;
						tentry2.setRefLine(line2);
						tentry2.setHoughValue(hsp2);

						if (acc[distance2][angle2] > maxValue) {
							entries.clear();
							maxValue = acc[distance2][angle2];
							entries.push_back(tentry2);
						} else {
							if (acc[distance2][angle2] == maxValue) {
								entries.push_back(tentry2);
							}
						}
					}
				}
			}
		}
	}
	return entries;
}

} /* namespace impls_2015 */
