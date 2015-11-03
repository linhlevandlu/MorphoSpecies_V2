/*
 * ShapeHistogram.cpp
 *
 *  Created on: Sep 1, 2015
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

#include "ShapeHistogram.h"

namespace impls_2015 {

ShapeHistogram::ShapeHistogram() {
	max_distance = 0;

}
ShapeHistogram::~ShapeHistogram() {
	// TODO Auto-generated destructor stub
}

/*
 * Get total entries of shape PGH
 */
double ShapeHistogram::getTotalEntries() {
	return this->totalEntries;
}

/*
 * Set the total entries of shape PGH
 */
void ShapeHistogram::setTotalEntries(double entries) {
	this->totalEntries = entries;
}

/*
 * Get the maximum distance of shape PGH
 */
double ShapeHistogram::getMaxDistance() {
	return this->max_distance;
}

/*
 * Set the maximum distance of shape PGH
 */
void ShapeHistogram::setMaxDistance(double distance) {
	this->max_distance = distance;
}

/*
 * Set the matrix presented shape PGH
 */
void ShapeHistogram::setMatrix(vector<vector<int> > matrix) {
	this->matrix = matrix;
}

/*
 * Get the matrix presented shape PGH
 */
vector<vector<int> > ShapeHistogram::getMatrix() {
	return this->matrix;
}

/*
 * Get the list of local PGH
 */
vector<LocalHistogram> ShapeHistogram::getListLocalHistogram() {
	return listLocalHistogram;
}

/*
 * Construct the shape PGH from a list of lines
 * @parameter: prLines - list of lines
 * @return: list of local histogram
 */
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

/*
 * Construct the matrix present for shape PGH
 * @parameter 1: angleAcc - angle accuracy
 * @parameter 2: cols - distance accuracy
 */
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

/*
 * Save the presented matrix into file
 * @parameter: fileName - save file path
 */
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

/*
 * Get the height of presented matrix
 * @parameter: angleAcc - the angle accurarcy
 * @return: height of matrix
 */
int ShapeHistogram::heightAngleAxis(LocalHistogram::AccuracyPGH angleAcc) {
	return LocalHistogram::heightOfAngleAxis(angleAcc);
}

/*
 * Find the offset of angle in matrix
 * @parameter 1: angle - the angle
 * @parameter 2: angleAcc - the angle accuracy
 * @return: the offset of angle in matrix
 */
int ShapeHistogram::angleOffset(double angle,
		LocalHistogram::AccuracyPGH angleAcc) {
	int bin;
	bin = LocalHistogram::accuracyToTimeDegree(angle, angleAcc);
	return bin;
}

/*
 * Find the offset of distance in matrix
 * @parameter 1: distance - the distance
 * @parameter 2: cols - the distance accuaracy
 * @return: the offset of distance in matrix
 */
int ShapeHistogram::distanceOffset(double distance, int cols) {
	return LocalHistogram::distanceOffset(this->max_distance, distance, cols);
}

/*
 * Presentation the shape PGH
 * @parameter 1: pghHistogram - list of local PGHs
 * @parameter 2: angleAcc - angle accuracy
 * @parameter 3: cols - distance accuracy
 * @return: image present for shape PGH
 */
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

/*
 * Compute the measure distance between two shape PGHs by Bhattacharyya metric
 * @parameter: sceneHist - the scene shape PGH
 * @return: the distance between two shape PGH
 */
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

/*
 * Compute the measure distance between two shape PGHs by Chi-Squared metric
 * @parameter: sceneHist - the scene shape PGH
 * @return: the distance between two shape PGH
 */
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

/*
 * Compute the measure distance between two shape PGHs by Intersection metric
 * @parameter: sceneHist - the scene shape PGH
 * @return: the distance between two shape PGH
 */
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
