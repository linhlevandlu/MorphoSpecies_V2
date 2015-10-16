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
		QString refLandmark, int width, int height) {

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
	if (maxVector.size() > 0) {
		qDebug() << "matching finished";
		Point refPoint = pht.refPointInScene(entry, maxVector, angleDiff, width,
				height);

		cv::Mat mat(cv::Size(width, height), CV_8UC3, cv::Scalar(0, 0, 0));
		for (size_t t = 0; t < sceneLines.size(); t++) {
			Line l = sceneLines.at(t);
			if (l.length() > 50)
				mat = l.drawing(mat);
		}

		mat = entry.getLine1().drawing(mat);
		mat = entry.getLine2().drawing(mat);
		line(mat, entry.getLine1().getP1(), entry.getLine1().getP2(),
				Scalar(0, 255, 0), 1, 8);
		cv::putText(mat, "1", entry.getLine1().getP1(), FONT_HERSHEY_COMPLEX, 1,
				Scalar(0, 255, 0), 1, 8);
		line(mat, entry.getLine2().getP1(), entry.getLine2().getP2(),
				Scalar(0, 255, 0), 1, 8);
		cv::putText(mat, "2", entry.getLine2().getP1(), FONT_HERSHEY_COMPLEX, 1,
				Scalar(0, 255, 0), 1, 8);
		line(mat, maxVector.at(0).getP1(), maxVector.at(0).getP2(),
				Scalar::all(255), 1, 8);
		cv::putText(mat, "1", maxVector.at(0).getP1(), FONT_HERSHEY_COMPLEX, 1,
				Scalar(0, 0, 255), 1, 8);
		line(mat, maxVector.at(1).getP1(), maxVector.at(1).getP2(),
				Scalar::all(255), 1, 8);
		cv::putText(mat, "2", maxVector.at(1).getP1(), FONT_HERSHEY_COMPLEX, 1,
				Scalar(0, 0, 255), 1, 8);

		vector<Point> landmarks = pht.readLandmarksFile(refLandmark);

		for (size_t t = 0; t < landmarks.size(); t++) {
			Point landm = landmarks.at(t);
			circle(mat, cv::Point(landm.x, height - landm.y), 7,
					Scalar(0, 0, 255), 1, 4);
		}

		circle(mat, hPoint, 5, Scalar(0, 0, 255), 1, 8);
		circle(mat, cv::Point(refPoint.x, refPoint.y), 5, Scalar(0, 255, 0), 1,
				8);

		vector<Point> esLandmarks = pht.estimateLandmarks(hPoint, refPoint,
				angleDiff, landmarks, width, height);
		for (size_t t = 0; t < esLandmarks.size(); t++) {
			circle(mat, esLandmarks.at(t), 2, Scalar(0, 255, 255), 1, 8);
		}

		imwrite("test/images/scene.jpg", mat);
		cv::namedWindow("DetectLines", CV_WINDOW_AUTOSIZE);
		cv::imshow("DetectLines", mat);
	}
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
	if (line1.length() > 50 && line2.length() > 50
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
	double refscaleRatio = ref1.length() / ref2.length();
	Line temp(ref1.getP1(), ref2.getP1());
	double reflength = temp.length();

	double sceneAngle = scene1.angleBetweenLines(scene2);
	double scenescaleRatio = scene1.length() / scene2.length();
	Line temp2(scene1.getP1(), scene2.getP1());
	double scenelength = temp2.length();

	/*if (abs(refAngle - sceneAngle) <= 1
	 && abs(refscaleRatio - scenescaleRatio) < 0.5
	 && abs(reflength - scenelength) < 0.5) {
	 return true;
	 }*/
	if (abs(refAngle - sceneAngle) < 1
			&& (abs(
					ref1.length() / scene1.length()
							- ref2.length() / scene2.length()) < 2))
		return true;
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
			/*if (maxVector.size() == 2)
			 break;*/
		}

		/*if (maxVector.size() == 2)
		 break;*/
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
	angleDiff = angleDifference(lineEntry1, objl1); // lineEntry1.angleBetweenLines(objl1); objl1.angleBetweenLines(lineEntry1);
	double angleDiff2 = angleDifference(lineEntry2, objl2);
	if (abs(lineEntry1.length() - objl1.length())
			>= abs(lineEntry1.length() - objl2.length())) {
		hs1 = entry.getHoughSpaces().at(1);
		hs2 = entry.getHoughSpaces().at(0);
		angleDiff = angleDifference(lineEntry1, objl2); // lineEntry1.angleBetweenLines(objl2);
		angleDiff2 = angleDifference(lineEntry2,objl2);
	}
	if(angleDiff > 0 ){
		if(angleDiff2 < angleDiff)
			angleDiff = angleDiff2;
	}else{
		if(angleDiff2 > angleDiff)
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
Mat PHoughTransform::pht(Image refImage, Image sceneImage, QString refLandmark,
		int width, int height) {
	QString scenelmDir = "/home/linh/Desktop/landmarks";
	vector<Line> refLines = refImage.lineSegment();
	vector<Line> sceneLines = sceneImage.lineSegment();

	Point hPoint(width / 2, height / 2);
	qDebug() << "Reference model point: (" << hPoint.x << ", " << hPoint.y
			<< ")";

	vector<PHTEntry> entryTable = constructTable(refLines, hPoint);
	qDebug() << "finished table";
	vector<Line> maxVector;
	double angleDiff;
	PHTEntry entry = matchingInScene(entryTable, sceneLines, width, height,
			maxVector);

	cv::Mat mat(sceneImage.getMatrixImage().clone());
	if (maxVector.size() > 0) {
		Point refPoint = refPointInScene(entry, maxVector, angleDiff, width,
				height);
		qDebug()<< "angle Difference: " << angleDiff;
		for (size_t t = 0; t < sceneLines.size(); t++) {
			Line l = sceneLines.at(t);
			if (l.length() > 20)
				mat = l.drawing(mat);
		}

		int index2 = sceneImage.getFileName().lastIndexOf("/");
		QString scenename = sceneImage.getFileName().mid(index2 + 1,
				sceneImage.getFileName().length() - index2 - 5);
		QString scenelmPath = scenelmDir + "/" + scenename + ".TPS";
		vector<Point> landmarks = readLandmarksFile(scenelmPath);

		for (size_t t = 0; t < landmarks.size(); t++) {
			Point landm = landmarks.at(t);
			circle(mat, cv::Point(landm.x, mat.rows - landm.y), 7,
					Scalar(0, 0, 255), 2, 4);
		}

		circle(mat, hPoint, 5, Scalar(255, 0, 0), 2, 8);
		circle(mat, refPoint, 5, Scalar(0, 255, 255), 5, 8);

		vector<Point> esLandmarks = estimateLandmarks(hPoint, refPoint,
				angleDiff, landmarks, mat.cols, mat.rows);
		for (size_t t = 0; t < esLandmarks.size(); t++) {
			circle(mat, esLandmarks.at(t), 2, Scalar(0, 255, 255), 2, 8);
		}
	}
	return mat;
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
vector<Point> PHoughTransform::estimateLandmarks(Point refPoint, Point esPoint,
		double angleDiff, vector<Point> refLandmarks, int width, int height) {
	vector<Point> esLandmarks;
	for (size_t t = 0; t < refLandmarks.size(); t++) {
		Point temp = refLandmarks.at(t);
		Point lm(temp.x, height - temp.y);
		int px = refPoint.x - esPoint.x;
		int py = refPoint.y - esPoint.y;
		int x, refX;
		int y, refY;
		if (angleDiff != 0) {
			//  rotation
			if (angleDiff > 0) {
				x = lm.x * cos(round(angleDiff) * M_PI / 180)
						- lm.y * sin(round(angleDiff) * M_PI / 180)
						+ refPoint.x;
				y = lm.x * sin(round(angleDiff) * M_PI / 180)
						+ lm.y * cos(round(angleDiff) * M_PI / 180)
						+ refPoint.y;

				refX = refPoint.x * cos(round(angleDiff) * M_PI / 180)
						- refPoint.y * sin(round(angleDiff) * M_PI / 180)
						+ refPoint.x;
				refY = refPoint.x * sin(round(angleDiff) * M_PI / 180)
						+ refPoint.y * cos(round(angleDiff) * M_PI / 180)
						+ refPoint.y;
			} else {
				x = lm.x * cos(round(angleDiff) * M_PI / 180)
						+ lm.y * sin(round(angleDiff) * M_PI / 180)
						+ refPoint.x;
				y = -lm.x * sin(round(angleDiff) * M_PI / 180)
						+ lm.y * cos(round(angleDiff) * M_PI / 180)
						+ refPoint.y;

				refX = refPoint.x * cos(round(angleDiff) * M_PI / 180)
						+ refPoint.y * sin(round(angleDiff) * M_PI / 180)
						+ refPoint.x;
				refY = -refPoint.x * sin(round(angleDiff) * M_PI / 180)
						+ refPoint.y * cos(round(angleDiff) * M_PI / 180)
						+ refPoint.y;
			}
			// and translation
			px = refX - refPoint.x;
			py = refY - refPoint.y;

			x = x - px;
			if (py > 0)
				y = y - py;
			else
				y = y + py;

		} else {

			x = lm.x - px;
			if (y > 0)
				y = lm.y - py;
			else
				y = lm.y + py;
		}
		esLandmarks.push_back(Point(x, y));
	}
	return esLandmarks;
}

void PHoughTransform::phtDirectory(Image refImage, QString reflmPath,
		QString sceneDir, QString scenelmDir, QString saveDir) {
	vector<Line> refLines = refImage.lineSegment();
	Mat refMatrix = refImage.getMatrixImage();
	Point hPoint(refMatrix.cols / 2, refMatrix.rows / 2);

	QFileInfoList files = Image::readFolder(sceneDir);
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name);
		vector<Line> sceneLines = sceneImage.lineSegment();

		int index2 = sceneImage.getFileName().lastIndexOf("/");
		QString scenename = sceneImage.getFileName().mid(index2 + 1,
				sceneImage.getFileName().length() - index2 - 5);
		qDebug() << scenename;

		vector<PHTEntry> entryTable = constructTable(refLines, hPoint);
		vector<Line> maxVector;
		double angleDiff;
		PHTEntry entry = matchingInScene(entryTable, sceneLines, refMatrix.cols,
				refMatrix.rows, maxVector);
		if (maxVector.size() > 0) {
			qDebug() << "matching finished";
			Point refPoint = refPointInScene(entry, maxVector, angleDiff,
					refMatrix.cols, refMatrix.rows);

			cv::Mat mat(sceneImage.getMatrixImage().clone());
			for (size_t t = 0; t < sceneLines.size(); t++) {
				Line l = sceneLines.at(t);
				if (l.length() > 20)
					mat = l.drawing(mat);
			}
			QString scenelmPath = scenelmDir + "/" + scenename + ".TPS";
			vector<Point> landmarks = readLandmarksFile(scenelmPath);

			for (size_t t = 0; t < landmarks.size(); t++) {
				Point landm = landmarks.at(t);
				circle(mat, cv::Point(landm.x, mat.rows - landm.y), 7,
						Scalar(0, 0, 255), 2, 4);
			}

			circle(mat, hPoint, 5, Scalar(255, 0, 0), 2, 8);
			circle(mat, cv::Point(refPoint.x, refPoint.y), 5, Scalar(0, 255, 0),
					2, 8);

			vector<Point> esLandmarks = estimateLandmarks(hPoint, refPoint,
					angleDiff, landmarks, mat.cols, mat.rows);
			for (size_t t = 0; t < esLandmarks.size(); t++) {
				circle(mat, esLandmarks.at(t), 2, Scalar(0, 255, 255), 2, 8);
			}
			QString savePath = saveDir + "/" + scenename + ".JPG";
			imwrite(savePath.toStdString().c_str(), mat);
		}
	}
}
double PHoughTransform::angleDifference(Line refLine, Line sceneLine) {
	double angle = refLine.angleBetweenLines(sceneLine);
	if (angle > 90)
		angle = 180 - angle;

	Point p1 = sceneLine.getP1();
	Point p2 = sceneLine.getP2();
	if (p1.x < refLine.getP1().x || p2.x < refLine.getP2().x)
		return -angle;
	return angle;
}
} /* namespace impls_2015 */
