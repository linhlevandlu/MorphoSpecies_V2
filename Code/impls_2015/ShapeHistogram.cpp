/*
 * PGH.cpp
 *
 *  Created on: Sep 1, 2015
 *      Author: linh
 */

#include "ShapeHistogram.h"

namespace impls_2015 {

ShapeHistogram::ShapeHistogram() {
	max_distance = 0;

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
vector<LocalHistogram> ShapeHistogram::getListLocalHistogram() {
	return listLocalHistogram;
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

	Line l31(cv::Point(200, 600), cv::Point(400, 550));
	Line l32(cv::Point(400, 550), cv::Point(350, 700));
	Line l33(cv::Point(350, 700), cv::Point(150, 800));
	Line l34(cv::Point(150, 800), cv::Point(200, 600));
	vector<Line> set3;
	set3.push_back(l31);
	set3.push_back(l32);
	set3.push_back(l33);
	set3.push_back(l34);

	Line l41(cv::Point(550, 700), cv::Point(750, 650));
	Line l42(cv::Point(750, 650), cv::Point(700, 800));
	Line l43(cv::Point(700, 800), cv::Point(500, 900));
	Line l44(cv::Point(500, 900), cv::Point(550, 700));
	vector<Line> set4;
	set4.push_back(l41);
	set4.push_back(l42);
	set4.push_back(l43);
	set4.push_back(l44);

	cv::Mat mat(cv::Size(2000, 2000), CV_8UC3, cv::Scalar(0, 0, 0));
	for (size_t i = 0; i < set1.size(); i++) {
		Line l = set1.at(i);
		mat = l.drawing(mat);
	}
	for (size_t i = 0; i < set2.size(); i++) {
		Line l = set2.at(i);
		mat = l.drawing(mat);
	}
	for (size_t i = 0; i < set3.size(); i++) {
		Line l = set3.at(i);
		mat = l.drawing(mat);
	}
	for (size_t i = 0; i < set4.size(); i++) {
		Line l = set4.at(i);
		mat = l.drawing(mat);
	}

	cv::namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
	cv::imshow("Histogram", mat);
	ShapeHistogram hist1;
	vector<LocalHistogram> refHist = hist1.constructPGH(set1);
	hist1.constructMatPGH(LocalHistogram::Degree, 250);
	ShapeHistogram hist2;
	vector<LocalHistogram> sceneHist = hist2.constructPGH(set2);
	hist2.constructMatPGH(LocalHistogram::Degree, 250);
	ShapeHistogram hist3;
	vector<LocalHistogram> sceneHist2 = hist3.constructPGH(set3);
	hist3.constructMatPGH(LocalHistogram::Degree, 250);
	ShapeHistogram hist4;
	vector<LocalHistogram> sceneHist4 = hist4.constructPGH(set4);
	hist4.constructMatPGH(LocalHistogram::Degree, 250);

	qDebug() << "Metric 1 - 2: " << hist1.bhattacharyaMetric(hist2);
	qDebug() << "Metric 1 - 3: " << hist1.bhattacharyaMetric(hist3);
	qDebug() << "Metric 1 - 4: " << hist1.bhattacharyaMetric(hist4);
	qDebug() << "Metric 2 - 3: " << hist2.bhattacharyaMetric(hist3);
	qDebug() << "Metric 2 - 4: " << hist2.bhattacharyaMetric(hist4);
	qDebug() << "Metric 3 - 4: " << hist3.bhattacharyaMetric(hist4);
	qDebug() << "Metric 4 - 1: " << hist4.bhattacharyaMetric(hist1);
}

// xay dung the pairwise histogram of shape
vector<LocalHistogram> ShapeHistogram::constructPGH(vector<Line> prLines) {
	vector<LocalHistogram> shapeHistogram;
	double maxDistance = 0;
	for (size_t t = 0; t < prLines.size(); t++) {
		Line refLine = prLines.at(t);
		LocalHistogram pwHistogram;
		for (size_t i = 0; i < prLines.size(); i++) {
			Line objLine = prLines.at(i);
			if (t != i) {
				GFeatures prH = refLine.pairwiseHistogram(objLine);
				pwHistogram.addGFeatures(prH);
			}
		}
		shapeHistogram.push_back(pwHistogram);
		if (pwHistogram.getMaxDistance() > maxDistance)
			maxDistance = pwHistogram.getMaxDistance();
	}
	this->max_distance = maxDistance;
	this->listLocalHistogram = shapeHistogram;
	//qDebug() << "max distance: " << maxDistance;
	return shapeHistogram;
}

void ShapeHistogram::constructMatPGH(LocalHistogram::AccuracyPGH angleAcc,
		int cols) {
	int rows = heightAngleAxis(angleAcc);
	//qDebug() << "Matrix r-c: " << rows << ", " << cols;
	double entries = 0;

	//Initialization the matrix
	vector<vector<int> > matrixResult;
	this->matrix.resize(rows + 1);
	for (int i = 0; i <= rows; i++) {
		matrix[i].resize(cols, 0);
	}

	vector<LocalHistogram> shapeHistogram = this->listLocalHistogram;
	for (size_t i = 0; i < shapeHistogram.size(); i++) {
		LocalHistogram lcHist = shapeHistogram.at(i);
		vector<GFeatures> lsFeatures = lcHist.getPWHistgoram();
		for (size_t j = 0; j < lsFeatures.size(); j++) {
			GFeatures feature = lsFeatures.at(j);
			int dmin = distanceOffset(feature.getDmin(), cols);
			int dmax = distanceOffset(feature.getDmax(), cols);
			int rowId = angleOffset(feature.getAngle(), angleAcc);
			if (!isnan(rowId)) {
				for (int k = dmin; k <= dmax; k++) {
					if (rowId >= 0 && k < cols) {
						matrix[rowId][k] += 1;
						entries++;
					}
				}
			}
		}
	}
	this->setTotalEntries(entries);
}
void ShapeHistogram::writeMatrix(QString fileName) {
	vector<vector<int> > matrixPGH = this->matrix;
	ofstream of(fileName.toStdString().c_str());
	int rows = matrixPGH.size();
	int cols = matrixPGH[0].size();
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			of << matrixPGH[i][j] << "\t";
		}
		of << "\n";
	}
	of.close();
}

vector<LocalHistogram> ShapeHistogram::constructPGH(vector<Line> prLines,
		LocalHistogram::AccuracyPGH angleAcc, double distanceAxis) {
	if (distanceAxis != 0)
		max_distance = distanceAxis;
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

		for (size_t i = 0; i < prLines.size(); i++) {
			Line objLine = prLines.at(i);
			if (t != i) {
				GFeatures prH = refLine.pairwiseHistogram(objLine);
				pwHistogram.addGFeatures(prH);

				int x1 = round(prH.getDmin());
				int x2 = round(prH.getDmax());
				int y = angleOffset(prH.getAngle(), angleAcc);
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
		//pwHistogram.setpwHistogram(pgh);
		pwh.push_back(pwHistogram);
	}
	this->setTotalEntries(entries);
	this->setMatrix(vcResult);
	this->listLocalHistogram = pwh;
	return pwh;
}
int ShapeHistogram::heightAngleAxis(LocalHistogram::AccuracyPGH angleAcc) {
	return LocalHistogram::heightOfAngleAxis(angleAcc);
}
int ShapeHistogram::angleOffset(double angle,
		LocalHistogram::AccuracyPGH angleAcc) {
	int bin;
	bin = LocalHistogram::accuracyToTimeDegree(angle, angleAcc);
	return bin;
}
int ShapeHistogram::distanceOffset(double distance, int cols) {
	double binSize = this->max_distance / cols;
	double bin = round(distance / binSize);
	return (bin > 0) ? (bin - 1) : bin;
}

cv::Mat ShapeHistogram::presentation(vector<LocalHistogram> pghHistograms,
		LocalHistogram::AccuracyPGH angleAcc, int cols) {
	int t_width = cols;
	int height = heightAngleAxis(angleAcc);
	cv::Mat result(cv::Size(t_width + 10, height + 1), CV_8UC3,
			cv::Scalar(0, 0, 0));
	for (size_t t = 0; t < pghHistograms.size(); t++) {
		LocalHistogram pwh = pghHistograms[t];
		for (size_t i = 0; i < pwh.getPWHistgoram().size(); i++) {
			GFeatures gfeature = pwh.getPWHistgoram().at(i);
			int x1 = distanceOffset(gfeature.getDmin(), cols);
			int x2 = distanceOffset(gfeature.getDmax(), cols);
			int y = angleOffset(gfeature.getAngle(), angleAcc);
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

} /* namespace impls_2015 */