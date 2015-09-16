/*
 * PGH.cpp
 *
 *  Created on: Sep 1, 2015
 *      Author: linh
 */

#include "ShapeHistogram.h"

namespace impls_2015 {

/*ShapeHistogram::ShapeHistogram() {
 max_distance = 0;

 }*/
ShapeHistogram::ShapeHistogram(vector<Line> lines) {
	this->max_distance = 0;
	this->lines = lines;
	//accuracy = Degree;
}
ShapeHistogram::~ShapeHistogram() {
	// TODO Auto-generated destructor stub
}

double ShapeHistogram::getTotalEntries() {
	return this->totalEntries;
}
void ShapeHistogram::setTotalEntries(double entries) {
	this->totalEntries = entries;
}
double ShapeHistogram::getMaxDistance() {
	return this->max_distance;
}
void ShapeHistogram::setMaxDistance(double distance) {
	this->max_distance = distance;
}

void ShapeHistogram::setMatrix(vector<vector<int> > matrix) {
	this->matrix = matrix;
}
vector<vector<int> > ShapeHistogram::getMatrix() {
	return this->matrix;
}

void ShapeHistogram::createTriangle() {
	// test tren tam giac
	qDebug() << "create triangle";
	Line t1(cv::Point(100, 100), cv::Point(400, 100));
	Line t2(cv::Point(400, 100), cv::Point(250, 350));
	Line t3(cv::Point(250, 350), cv::Point(100, 100));
	vector<Line> sett1;
	sett1.push_back(t1);
	sett1.push_back(t2);
	sett1.push_back(t3);

	Line t11(cv::Point(250, 250), cv::Point(400, 250));
	Line t12(cv::Point(400, 250), cv::Point(550, 250));
	Line t22(cv::Point(550, 250), cv::Point(400, 500));
	Line t33(cv::Point(400, 500), cv::Point(250, 250));
	vector<Line> sett2;
	sett2.push_back(t11);
	sett2.push_back(t12);
	sett2.push_back(t22);
	sett2.push_back(t33);

	Line t111(cv::Point(100, 150), cv::Point(100, 450));
	Line t222(cv::Point(100, 450), cv::Point(350, 300));
	Line t333(cv::Point(350, 300), cv::Point(100, 150));
	vector<Line> sett3;
	sett3.push_back(t111);
	sett3.push_back(t222);
	sett3.push_back(t333);

	cv::Mat mat(cv::Size(2000, 2000), CV_8UC3, cv::Scalar(0, 0, 0));
	for (size_t i = 0; i < sett1.size(); i++) {
		Line l = sett1.at(i);
		mat = l.drawing(mat);
	}
	for (size_t i = 0; i < sett2.size(); i++) {
		Line l = sett2.at(i);
		mat = l.drawing(mat);
	}
	for (size_t i = 0; i < sett3.size(); i++) {
		Line l = sett3.at(i);
		mat = l.drawing(mat);
	}
	cv::namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
	cv::imshow("Histogram", mat);
	ShapeHistogram hist1(sett1);
	ShapeHistogram hist2(sett2);
	ShapeHistogram hist3(sett3);
	hist1.constructPGH(ShapeHistogram::TwoTimeDegree,0);
	qDebug() << "Metric 1 - 1: " << hist1.intersectionMetric(hist1);
	hist2.constructPGH(ShapeHistogram::TwoTimeDegree,0);
	qDebug() << "Metric 2 - 2: " << hist2.intersectionMetric(hist2);
	hist3.constructPGH(ShapeHistogram::TwoTimeDegree,0);
	qDebug() << "Metric 3 - 3: " << hist3.intersectionMetric(hist3);
	hist2.constructPGH(ShapeHistogram::TwoTimeDegree,hist1.getMaxDistance());
	qDebug() << "Metric 1 - 2: " << hist1.intersectionMetric(hist2);
	hist3.constructPGH(ShapeHistogram::TwoTimeDegree,hist1.getMaxDistance());
	qDebug() << "Metric 1 - 3: " << hist1.intersectionMetric(hist3);
	hist2.constructPGH(ShapeHistogram::TwoTimeDegree,0);
	hist3.constructPGH(ShapeHistogram::TwoTimeDegree,hist2.getMaxDistance());
	qDebug() << "Metric 2 - 3: " << hist2.intersectionMetric(hist3);
}

void ShapeHistogram::createShape() {
	qDebug() << "Create shape";
	Line l1(cv::Point(450, 600), cv::Point(650, 550));
	Line l2(cv::Point(650, 550), cv::Point(600, 700));
	Line l3(cv::Point(600, 700), cv::Point(400, 800));
	Line l4(cv::Point(400, 800), cv::Point(450, 600));
	vector<Line> set1;
	set1.push_back(l1);
	set1.push_back(l2);
	set1.push_back(l3);
	set1.push_back(l4);

	Line l5(cv::Point(450, 650), cv::Point(600, 600));
	Line l6(cv::Point(600, 600), cv::Point(550, 800));
	Line l7(cv::Point(550, 800), cv::Point(350, 850));
	Line l8(cv::Point(350, 850), cv::Point(450, 650));
	vector<Line> set2;
	set2.push_back(l5);
	set2.push_back(l6);
	set2.push_back(l7);
	set2.push_back(l8);
	cv::Mat mat(cv::Size(2000, 2000), CV_8UC3, cv::Scalar(0, 0, 0));
	for (size_t i = 0; i < set1.size(); i++) {
		Line l = set1.at(i);
		mat = l.drawing(mat);
	}
	for (size_t i = 0; i < set2.size(); i++) {
		Line l = set2.at(i);
		mat = l.drawing(mat);
	}

	cv::namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
	cv::imshow("Histogram", mat);
	ShapeHistogram hist1(set1);
	ShapeHistogram hist2(set2);
	hist1.constructPGH(ShapeHistogram::TwoTimeDegree,0);
	qDebug() << "Metric 1 - 1: " << hist1.chiSquaredMetric(hist1);
	hist2.constructPGH(ShapeHistogram::TwoTimeDegree,0);
	qDebug() << "Metric 2 - 2: " << hist2.chiSquaredMetric(hist2);
	hist2.constructPGH(ShapeHistogram::TwoTimeDegree,hist1.getMaxDistance());
	qDebug() << "Metric 1 - 2: " << hist1.chiSquaredMetric(hist2);
}
/*vector<LocalHistogram> ShapeHistogram::constructPGH() {
	vector<Line> prLines = this->lines;
	vector<LocalHistogram> pwh;
	for (size_t t = 0; t < prLines.size(); t++) {
		Line refLine = prLines.at(t);
		LocalHistogram pwHistogram;
		pwHistogram.setLine(refLine);
		vector<GFeatures> pgh;
		for (size_t i = 0; i < prLines.size(); i++) {
			Line objLine = prLines.at(i);
			if (t != i) {
				GFeatures prH = refLine.pairwiseHistogram(objLine);
				pgh.push_back(prH);
				if (prH.getDmax() > max_distance)
					max_distance = round(prH.getDmax());
			}
		}
		pwHistogram.setpwHistogram(pgh);
		pwh.push_back(pwHistogram);
	}
	return pwh;
}*/
vector<LocalHistogram> ShapeHistogram::constructPGH(AccuracyPGH angleAcc,
		double distanceAxis) {
	if(distanceAxis != 0 )
		max_distance = distanceAxis;
	vector<Line> prLines = this->lines;
	int height = heightAngleAxis(angleAcc);
	double entries = 0;
	vector<vector<int> > vcResult;
	vcResult.resize(height + 1);
	for (int i = 0; i <= height; ++i) {
		vcResult[i].resize(max_distance + 1, 0);
	}
	vector<LocalHistogram> pwh;
	for (size_t t = 0; t < prLines.size(); t++) {
		Line refLine = prLines.at(t);
		LocalHistogram pwHistogram;
		pwHistogram.setLine(refLine);
		vector<GFeatures> pgh;
		for (size_t i = 0; i < prLines.size(); i++) {
			Line objLine = prLines.at(i);
			if (t != i) {
				GFeatures prH = refLine.pairwiseHistogram(objLine);
				pgh.push_back(prH);
				int x1 = round(prH.getDmin());
				int x2 = round(prH.getDmax());
				int y = binLocation(prH.getAngle(), angleAcc);
				if (prH.getDmax() > max_distance && distanceAxis == 0) {
					max_distance = round(prH.getDmax());
					// resize the size of matrix
					for (int i = 0; i <= height; ++i) {
						vcResult[i].resize(max_distance + 1, 0);
					}
				}
				// save the information into matrix
				if (!isnan(y)) {
					for (int k = x1; k <= x2; k++) {
						if (y >= 0) {
							if (k >= max_distance + 1) {
								vcResult[y][max_distance] += 1;
							} else {
								vcResult[y][k] += 1;
							}
							entries++;
						}
					}
				}
			}
		}
		pwHistogram.setpwHistogram(pgh);
		pwh.push_back(pwHistogram);
	}
	this->setTotalEntries(entries);
	this->setMatrix(vcResult);
	return pwh;
}
int ShapeHistogram::heightAngleAxis(int angleAcc) {
	return angleAcc * 180;
}
int ShapeHistogram::binLocation(double angle, int angleAcc) {
	int bin;
	bin = LocalHistogram::accuracyToTimeDegree(angle, angleAcc);
	return bin;
}
/*vector<LocalHistogram> ShapeHistogram::shapePGH() {
	return constructPGH();
}
vector<vector<int> > ShapeHistogram::savePGH(
		vector<LocalHistogram> pghHistograms, AccuracyPGH angleAcc) {

	int t_width = 0;
	int height = heightAngleAxis(angleAcc);
	double entries = 0;
	// get the maximal distance
	t_width = this->max_distance;

	vector<vector<int> > vcResult;
	vcResult.resize(height + 1);
	for (int i = 0; i <= height; ++i) {
		vcResult[i].resize(t_width + 1, 0);
	}
	for (size_t t = 0; t < pghHistograms.size(); t++) {
		LocalHistogram pwh = pghHistograms[t];
		for (size_t i = 0; i < pwh.getPWHistgoram().size(); i++) {
			GFeatures gfeature = pwh.getPWHistgoram().at(i);
			int x1 = round(gfeature.getDmin());
			int x2 = round(gfeature.getDmax());
			int y = binLocation(gfeature.getAngle(), angleAcc);
			if (!isnan(y)) {
				for (int k = x1; k <= x2; k++) {
					if (y >= 0) {
						if (k >= t_width + 1) {
							vcResult[y][t_width] += 1;
						} else {
							vcResult[y][k] += 1;
						}
						entries++;
					}
				}
			}

		}
	}
	qDebug() << "2. max distance: " << max_distance << ", entries: " << entries;
	this->setTotalEntries(entries);
	this->setMatrix(vcResult);
	return vcResult;
}*/
cv::Mat ShapeHistogram::presentation(AccuracyPGH angleAcc) {
	vector<LocalHistogram> pghHistograms = this->constructPGH(angleAcc,0);
	int t_width = this->max_distance;
	int height = heightAngleAxis(angleAcc);
	cv::Mat result(cv::Size(t_width + 10, height + 1), CV_8UC3,
			cv::Scalar(0, 0, 0));
	for (size_t t = 0; t < pghHistograms.size(); t++) {
		LocalHistogram pwh = pghHistograms[t];
		for (size_t i = 0; i < pwh.getPWHistgoram().size(); i++) {
			GFeatures gfeature = pwh.getPWHistgoram().at(i);
			int x1 = round(gfeature.getDmin());
			int x2 = round(gfeature.getDmax());
			int y = binLocation(gfeature.getAngle(), angleAcc);
			cv::line(result, cv::Point(x1, y), cv::Point(x2, y),
					cv::Scalar(255, 255, 255), 1, 8);
		}
	}
	return result;
}
double ShapeHistogram::bhattacharyaMetric(ShapeHistogram sceneHist) {
	vector<vector<int> > pointer_ref = this->matrix;
	vector<vector<int> > pointer_scene = sceneHist.getMatrix();
	double size1 = this->totalEntries;
	double size2 = sceneHist.totalEntries;
	int distance_size = pointer_ref[0].size();

	double distance = 0;
	for (size_t i = 0; i < pointer_ref.size(); i++) {
		for (int j = 0; j < distance_size; j++) {
			double value1 = sqrt(pointer_ref[i][j] / size1);
			double value2 = sqrt(pointer_scene[i][j] / size2);
			distance += value1 * value2;
		}
	}
	return distance;
}
double ShapeHistogram::chiSquaredMetric(ShapeHistogram sceneHist) {
	vector<vector<int> > pointer_ref = this->matrix;
	vector<vector<int> > pointer_scene = sceneHist.getMatrix();
	double size1 = this->totalEntries;
	double size2 = sceneHist.totalEntries;
	int distance_size = pointer_ref[0].size();

	double distance = 0;
	for (size_t i = 0; i < pointer_ref.size(); i++) {
		for (int j = 0; j < distance_size; j++) {
			if (pointer_ref[i][j] != 0 || pointer_scene[i][j] != 0) {
				double xi = double(pointer_ref[i][j]) / size1;
				double yi = double(pointer_scene[i][j]) / size2;
				distance += (((xi - yi) * (xi - yi)) / (xi + yi));
			}
		}
	}
	return distance / 2;
}
double ShapeHistogram::intersectionMetric(ShapeHistogram sceneHist) {
	vector<vector<int> > pointer_ref = this->matrix;
	vector<vector<int> > pointer_scene = sceneHist.getMatrix();
	double size1 = this->totalEntries;
	double size2 = sceneHist.totalEntries;
	int distance_size = pointer_ref[0].size();

	double distance = 0;
	for (size_t i = 0; i < pointer_ref.size(); i++) {
		for (int j = 0; j < distance_size; j++) {
			double xi = double(pointer_ref[i][j]) / size1;
			double yi = double(pointer_scene[i][j]) / size2;
			distance += (xi > yi ? yi : xi);
		}
	}
	return distance;
}
vector<Edge> ShapeHistogram::getEdges(Image image) {
	vector<Edge> edges;
	return edges;
}
QList<Landmark> ShapeHistogram::getLandmarks() {
	QList<Landmark> landmarks;
	return landmarks;
}
} /* namespace impls_2015 */
