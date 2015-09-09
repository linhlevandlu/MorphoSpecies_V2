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

ShapeHistogram::ShapeHistogram(vector<Line> lines) {
	this->lines = lines;
	max_distance = 0;
}

ShapeHistogram::~ShapeHistogram() {
	// TODO Auto-generated destructor stub
}
void ShapeHistogram::setLines(vector<Line> lines) {
	this->lines = lines;
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

void ShapeHistogram::setMatrix(vector<vector<int> > matrix) {
	this->matrix = matrix;
}
vector<vector<int> > ShapeHistogram::getMatrix() {
	return this->matrix;
}

void ShapeHistogram::createSquare() {
	qDebug() << "Create square";
	Line l1(cv::Point(500, 250), cv::Point(1250, 250));
	Line l2(cv::Point(1250, 250), cv::Point(1250, 1000));
	Line l3(cv::Point(1250, 1000), cv::Point(500, 1000));
	Line l4(cv::Point(500, 1000), cv::Point(500, 250));
	vector<Line> set1;
	set1.push_back(l1);
	set1.push_back(l2);
	set1.push_back(l3);
	set1.push_back(l4);

	Line l5(cv::Point(600, 350), cv::Point(1350, 350));
	Line l6(cv::Point(1350, 350), cv::Point(1350, 1100));
	Line l7(cv::Point(1350, 1100), cv::Point(600, 1100));
	Line l8(cv::Point(600, 1100), cv::Point(600, 350));
	vector<Line> set2;
	set2.push_back(l5);
	set2.push_back(l6);
	set2.push_back(l7);
	set2.push_back(l8);

	Line l9(cv::Point(600, 150), cv::Point(1350, 350));
	Line l10(cv::Point(1350, 350), cv::Point(1150, 1100));
	Line l11(cv::Point(1150, 1100), cv::Point(400, 900));
	Line l12(cv::Point(400, 900), cv::Point(600, 150));
	vector<Line> set3;
	set3.push_back(l9);
	set3.push_back(l10);
	set3.push_back(l11);
	set3.push_back(l12);
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
	cv::namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
	cv::imshow("Histogram", mat);
	ShapeHistogram hist1(set1);
	ShapeHistogram hist2(set2);
	ShapeHistogram hist3(set3);
	qDebug() << "Metric 1 - 1: " << hist1.bhattacharyaMetric(hist1);
	qDebug() << "Metric 2 - 2: " << hist2.bhattacharyaMetric(hist2);
	qDebug() << "Metric 3 - 3: " << hist3.bhattacharyaMetric(hist3);
	qDebug() << "Metric 1 - 2: " << hist1.bhattacharyaMetric(hist2);
	qDebug() << "Metric 1 - 3: " << hist1.bhattacharyaMetric(hist3);
	qDebug() << "Metric 2 - 3: " << hist2.bhattacharyaMetric(hist3);
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

	Line t11(cv::Point(250, 250), cv::Point(550, 250));
	Line t22(cv::Point(550, 250), cv::Point(400, 500));
	Line t33(cv::Point(400, 500), cv::Point(250, 250));
	vector<Line> sett2;
	sett2.push_back(t11);
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
	qDebug() << "Metric 1 - 1: " << hist1.bhattacharyaMetric(hist1);
	qDebug() << "Metric 2 - 2: " << hist2.bhattacharyaMetric(hist2);
	qDebug() << "Metric 3 - 3: " << hist3.bhattacharyaMetric(hist3);
	qDebug() << "Metric 1 - 2: " << hist1.bhattacharyaMetric(hist2);
	qDebug() << "Metric 1 - 3: " << hist1.bhattacharyaMetric(hist3);
	qDebug() << "Metric 2 - 3: " << hist2.bhattacharyaMetric(hist3);
}
void ShapeHistogram::createTrapezoid() {
	qDebug() << "Create trapezoid";
	Line l1(cv::Point(150, 500), cv::Point(300, 500));
	Line l2(cv::Point(300, 500), cv::Point(350, 700));
	Line l3(cv::Point(350, 700), cv::Point(100, 700));
	Line l4(cv::Point(100, 700), cv::Point(150, 500));
	vector<Line> set1;
	set1.push_back(l1);
	set1.push_back(l2);
	set1.push_back(l3);
	set1.push_back(l4);

	Line l5(cv::Point(200, 600), cv::Point(350, 600));
	Line l6(cv::Point(350, 600), cv::Point(400, 800));
	Line l7(cv::Point(400, 800), cv::Point(150, 800));
	Line l8(cv::Point(150, 800), cv::Point(200, 600));
	vector<Line> set2;
	set2.push_back(l5);
	set2.push_back(l6);
	set2.push_back(l7);
	set2.push_back(l8);

	Line l9(cv::Point(150, 550), cv::Point(350, 600));
	Line l10(cv::Point(350, 600), cv::Point(350, 750));
	Line l11(cv::Point(350, 750), cv::Point(150, 800));
	Line l12(cv::Point(150, 800), cv::Point(150, 550));
	vector<Line> set3;
	set3.push_back(l9);
	set3.push_back(l10);
	set3.push_back(l11);
	set3.push_back(l12);
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
	cv::namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
	cv::imshow("Histogram", mat);
	ShapeHistogram hist1(set1);
	ShapeHistogram hist2(set2);
	ShapeHistogram hist3(set3);
	qDebug() << "Metric 1 - 1: " << hist1.bhattacharyaMetric(hist1);
	qDebug() << "Metric 2 - 2: " << hist2.bhattacharyaMetric(hist2);
	qDebug() << "Metric 3 - 3: " << hist3.bhattacharyaMetric(hist3);
	qDebug() << "Metric 1 - 2: " << hist1.bhattacharyaMetric(hist2);
	qDebug() << "Metric 1 - 3: " << hist1.bhattacharyaMetric(hist3);
	qDebug() << "Metric 2 - 3: " << hist2.bhattacharyaMetric(hist3);
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
	qDebug() << "Metric 1 - 1: " << hist1.bhattacharyaMetric(hist1);
	qDebug() << "Metric 2 - 2: " << hist2.bhattacharyaMetric(hist2);
	qDebug() << "Metric 1 - 2: " << hist1.bhattacharyaMetric(hist2);
}
vector<LocalHistogram> ShapeHistogram::constructPGH(vector<Line> prLines) {
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
}
vector<LocalHistogram> ShapeHistogram::shapePGH(vector<Line> prLines) {
	return constructPGH(prLines);
}
vector<vector<int> > ShapeHistogram::savePGH(
		vector<LocalHistogram> pghHistograms) {
	//clock_t t1, t2;
	//t1 = clock();

	int t_width = 0;
	int height = 180;
	double entries = 0;
	//vector<LocalHistogram> pghHistograms = shapePGH();
	// get the maximal distance

	t_width = this->max_distance;

	qDebug()<<"matrix size: " <<t_width<<", "<< height;
	vector<vector<int> > vcResult;
	vcResult.resize(height + 1);
	for (int i = 0; i <= height; ++i) {
		vcResult[i].resize(t_width + 10, 0);
	}

	for (size_t t = 0; t < pghHistograms.size(); t++) {
		LocalHistogram pwh = pghHistograms[t];
		for (size_t i = 0; i < pwh.getPWHistgoram().size(); i++) {
			GFeatures gfeature = pwh.getPWHistgoram().at(i);
			int x1 = round(gfeature.getDmin());
			int x2 = round(gfeature.getDmax());
			int y = LocalHistogram::convertAngle(gfeature.getAngle());
			if (!isnan(y)) {
				for (int k = x1; k <= x2; k++) {
					if (y >= 0) {
						vcResult[y][k] += 1;
						entries++;
					}
				}
			}

		}
	}
	//t2 = clock();
	/*qDebug() << "time save PGH: " << ((float) t2 - (float) t1) / CLOCKS_PER_SEC
	 << " seconds";*/
	this->setTotalEntries(entries);
	this->setMatrix(vcResult);
	return vcResult;
}

double ShapeHistogram::bhattacharyaMetric(ShapeHistogram sceneHist) {
	vector<vector<int> > pointer_ref; //= this->savePGH();
	if (this->matrix.size() > 0) {
		pointer_ref = this->matrix;
	} else {
		pointer_ref = this->savePGH(constructPGH(this->lines));
	}
	vector<vector<int> > pointer_scene = sceneHist.savePGH(
			sceneHist.constructPGH(sceneHist.lines));
	double size1 = this->totalEntries;
	double size2 = sceneHist.totalEntries;

	int max1 = pointer_ref[0].size();
	int max2 = pointer_scene[0].size();
	int distance_size = (max1 > max2) ? max1 : max2;
	if (max1 > max2) {
		pointer_scene = resizeMatrix(pointer_scene, pointer_ref);
	} else {
		pointer_ref = resizeMatrix(pointer_ref, pointer_scene);
	}

	double distance = 0;
	for (int i = 0; i < 181; i++) {
		for (int j = 0; j < distance_size; j++) {
			double value1 = sqrt(pointer_ref[i][j] / size1);
			double value2 = sqrt(pointer_scene[i][j] / size2);
			distance += value1 * value2;
		}
	}

	return distance;
}
/**
 * Resize the matrix 1 follows the size of matrix2
 */
vector<vector<int> > ShapeHistogram::resizeMatrix(vector<vector<int> > matrix1,
		vector<vector<int> > matrix2) {
	int new_cols = matrix2[0].size();
	for (int i = 0; i <= 180; ++i) {
		matrix1[i].resize(new_cols, 0);
	}
	return matrix1;
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
