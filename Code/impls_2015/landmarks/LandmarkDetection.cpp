/*
 * LandmarkDetection.cpp
 *
 *  Created on: Aug 21, 2015
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

#include "LandmarkDetection.h"

namespace impls_2015 {

/**
 * Constructor the landmarks detection
 */
LandmarkDetection::LandmarkDetection() {
}

LandmarkDetection::~LandmarkDetection() {
	// TODO Auto-generated destructor stub
}

/*
 * Create the bounding box around a point
 * @parameter 1: matImage - the image
 * @parameter 2: landmark - the point
 * @parameter 3: tsize - the size of bounding box
 * @parameter 4: location - the real location of box on image
 * @parameter 5: distance - the distance (x,y) from the point to the box position
 * @return: the matrix presented bounding box around a point
 */
Mat LandmarkDetection::createTemplate(Mat matImage, Point landmark, int tsize,
		Point &location, Point &distance) {
	Mat temp(matImage.clone());
	if (landmark.x <= 0 || landmark.y <= 0)
		return temp;
	int lx = (landmark.x - tsize / 2) < 0 ? 0 : (landmark.x - tsize / 2);
	int ly = (landmark.y - tsize / 2) < 0 ? 0 : (landmark.y - tsize / 2);
	location.x = lx;
	location.y = ly;
	distance.x = landmark.x - lx;
	distance.y = landmark.y - ly;
	int lx2 =
			(landmark.x + tsize / 2) > matImage.cols ?
					matImage.cols : (landmark.x + tsize / 2);
	int ly2 =
			(landmark.y + tsize / 2) > matImage.rows ?
					matImage.rows : (landmark.y + tsize / 2);

	Mat refTemplate(matImage, Rect(lx, ly, lx2 - lx, ly2 - ly));
	return refTemplate;
}

/*
 * Cross-correlation between two matrix
 * @parameter 1: templ - the template matrix
 * @parameter 2: scene - the scene matrix(image)
 * @return: point on scene, where have the best matching of template on image
 */
Point LandmarkDetection::matCrossCorrelation(Mat templ, Mat scene) {
	int width = scene.cols - templ.cols + 1;
	int height = scene.rows - templ.rows + 1;
	if (width > 0 && height > 0) {
		Mat result(width, height, CV_32FC1);
		cv::matchTemplate(scene, templ, result, CV_TM_CCORR_NORMED);
		double maxValue, minValue;
		Point maxLoc, minLoc;
		cv::minMaxLoc(result, &minValue, &maxValue, &minLoc, &maxLoc, Mat());
		return maxLoc;
	}
	return Point(0, 0);
}

/*
 * Cross-correlation between two images
 * @parameter 1: refImage - the reference image
 * @parameter 2: sceneImage - the scene image
 * @parameter 3: lmPath - the reference landmarks file
 * @parameter 4: templSize - the size of bounding box
 * @return: a list estimated landmarks of reference landmarks on scene image
 */
vector<Point> LandmarkDetection::crossCorrelation(Image refImage,
		Image sceneImage, string lmPath, int templSize) {
	vector<Point> refLandmarks = refImage.readLandmarksFile(lmPath.c_str());
	vector<Point> sceneLandmarks;
	Point tLocation, tDistance, iLocation, iDistance;
	for (size_t i = 0; i < refLandmarks.size(); i++) {
		Point temp = refLandmarks.at(i);
		Point lm(temp.x, refImage.getMatrixImage().rows - temp.y);

		Mat templ = createTemplate(refImage.getMatrixImage(), lm, templSize,
				tLocation, tDistance);
		Mat img = sceneImage.getMatrixImage();
		Point maxLoc = matCrossCorrelation(templ, img);
		sceneLandmarks.push_back(iLocation + maxLoc + tDistance);
	}
	return sceneLandmarks;
}

/*
 * Cross-correlation between a reference image and the images in a folder
 * @parameter 1: refImage - the reference image
 * @parameter 2: folderpath - the images folder
 * @parameter 3: savepath - the saving folder
 * @parameter 4: lmPath - the reference landmarks file
 */
void LandmarkDetection::cCorrelationByDirectory(Image refImage,
		string folderpath, string savepath, string lmPath) {

	QFileInfoList files = Image::readImagesFolder(folderpath);

	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name.toStdString());

		cv::Mat result(sceneImage.getMatrixImage().clone());
		vector<Point> landmarks = crossCorrelation(refImage, sceneImage, lmPath,
				400);
		QString scenename = sceneImage.getName();
		qDebug() << scenename;

		for (size_t i = 0; i < landmarks.size(); i++) {
			Point lm = landmarks.at(i);
			circle(result, Point(lm.x, lm.y), 5, Scalar(0, 0, 255), 2, 4);
		}
		string imgRoot = savepath;
		string saveImg =
				imgRoot.append("/").append(scenename.toStdString()).append(
						".JPG");
		imwrite(saveImg.c_str(), result);
	}
	//meanDiff.close();
}

/*
 * Compute the centroid of estimated landmarks based on cross-correlation
 * @parameter 1: refImage - reference image
 * @parameter 2: sceneImage - scene image
 * @parameter 3: lmPath - the reference landmarks file
 * @parameter 4: size - the size of bounding box
 * @parameter 5 - output: ebary - the position of estimated center point
 * @return: the centroid of estimated landmarks on scene image
 */
double LandmarkDetection::centroidCCorrelation(Image refImage, Image sceneImage,
		string lmPath, int size, Point &ebary) {

	/*QString scenename = sceneImage.getName();
	 qDebug() << scenename;
	 QString slmPath = "/home/linh/Desktop/landmarks/" + scenename + ".TPS";
	 vector<Point> orgLandmarks = refImage.readLandmarksFile(
	 slmPath.toStdString().c_str());*/

	vector<Point> eLandmarks = crossCorrelation(refImage, sceneImage, lmPath,
			size);
	double eCentroid;
	eCentroid = measureDistance(eLandmarks, ebary);

	return eCentroid;
}

/*
 * Compute the centroid of estimated landmarks based on cross-correlation of the images folders
 * @parameter 1: refImage - reference image
 * @parameter 2: lmPath - the reference landmarks file
 * @parameter 3: folderImages - the images folder
 * @parameter 4: folderlandmarks - the original landmarks folder
 */
void LandmarkDetection::centroidCCorrelations(Image refImage, string lmPath,
		string folderImages, string folderlandmarks) {

	QFileInfoList files = Image::readImagesFolder(folderImages);
	ofstream of("results/cc_ecentroid.PHG");
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name.toStdString());

		QString scenename = sceneImage.getName();
		string fdRoot = folderlandmarks;
		string spath =
				fdRoot.append("/").append(scenename.toStdString()).append(
						".TPS");
		vector<Point> orgLandmarks = sceneImage.readLandmarksFile(
				spath.c_str());
		vector<Point> eLandmarks = crossCorrelation(refImage, sceneImage,
				lmPath, 400);
		of << scenename.toStdString().c_str() << ".JPG" << "\t";
		qDebug() << scenename;
		Point mBary, eBary;
		double mCentroid, eCentroid;
		double distance = measureDistance(orgLandmarks, eLandmarks, mCentroid,
				eCentroid, mBary, eBary);

		of << "(" << mBary.x << "," << mBary.y << ")" << "\t" << "(" << eBary.x
				<< "," << eBary.y << ")" << "\t";

		of << mCentroid << "\t" << eCentroid << "\t" << distance << "\n";
	}
	of.close();
}
/*
 * Compute the centroid of estimated landmarks based on cross-correlation of the images folders
 * @parameter 1: refImage - reference image
 * @parameter 2: lmPath - the reference landmarks file
 * @parameter 3: folderImages - the images folder
 */
void LandmarkDetection::centroidCCorrelations(Image refImage, string lmPath,
		string folderImages) {
	qDebug() << "Compute centroid by correlation";
	QFileInfoList files = Image::readImagesFolder(folderImages);
	ofstream of("results/cc_ecentroid.PHG");
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name.toStdString());

		QString scenename = sceneImage.getName();
		vector<Point> eLandmarks = crossCorrelation(refImage, sceneImage,
				lmPath, 400);
		of << scenename.toStdString().c_str() << ".JPG" << "\t";
		qDebug() << scenename;
		Point eBary;
		double eCentroid;
		eCentroid = measureDistance(eLandmarks, eBary);
		of << "(" << eBary.x << "," << eBary.y << ")" << "\t";
		of << eCentroid << "\n";
	}
	of.close();
}

/*
 * Compute the centroid of a list of points
 * @parameter 1: landmarks - list of points
 * @parameter 2 - output: bary - the center point
 * @return: the centroid of list points
 */
double LandmarkDetection::measureDistance(vector<Point> landmarks,
		Point &bary) {

	int totalX = 0, totalY = 0;
	size_t lmSize = landmarks.size();
	for (size_t j = 0; j < landmarks.size(); j++) {
		Point lm = landmarks.at(j);
		totalX += lm.x;
		totalY += lm.y;
	}
	if (lmSize > 0) {
		bary.x = totalX / lmSize;
		bary.y = totalY / lmSize;
	} else {
		bary.x = 0;
		bary.y = 0;
	}
	double totalDistance = 0;
	for (size_t j = 0; j < landmarks.size(); j++) {
		Point lm = landmarks.at(j);
		Line line(lm, bary);
		totalDistance += (line.length() * line.length());
	}
	return sqrt(totalDistance);
}

/*
 * Compute the difference between original centroid and estimated centroid
 * @parameter 1: orgLandmarks - the original landmarks
 * @parameter 2: esLandmarks - the estimated landmarks
 * @parameter 3 - output: mCentroid - the original centroid
 * @parameter 4 - output: eCentroid - the estiamted centroid
 * @parameter 5 - output: mBary - the original center point
 * @parameter 6 - output: eBary - the estimated center point
 * @return: the difference between original centroid and estimated centroid
 */
double LandmarkDetection::measureDistance(vector<Point> orgLandmarks,
		vector<Point> esLandmarks, double &mCentroid, double &eCentroid,
		Point &mBary, Point &eBary) {
	mCentroid = measureDistance(orgLandmarks, mBary);
	eCentroid = measureDistance(esLandmarks, eBary);
	double subtraction = abs(mCentroid - eCentroid);
	return subtraction;
}

/*
 * Estimate the landmarks based on the proposed method between two images
 * @parameter 1: refImage - the reference image
 * @parameter 2: sceneImaeg - the scene image
 * @parameter 3: lmPath - the reference landmarks file
 * @parameter 4: templSize - size of box around reference landmarks
 * @parameter 5: sceneSize - size of box around estimated landmarks
 * @parameter 6: angleDiff - the difference angle between two images
 * @parameter 7 - output: mcResult - list of estimated landmarks
 * @return: the list of estimated landmarks and present it on the scene image
 */
Mat LandmarkDetection::estimateLandmarks(Image refImage, Image sceneImage,
		string lmPath, int templSize, int sceneSize, double &angleDiff,
		Image::SegmentMethod sgmethod, vector<Point> &mcResult, Point &ePoint) {
	vector<Point> refLandmarks = refImage.readLandmarksFile(lmPath);
	Mat sMatrix = sceneImage.getMatrixImage();
	Mat mMatrix = refImage.getMatrixImage();
	clock_t t1, t2;
	t1 = clock();
	PHoughTransform pht;
	vector<Point> esLandmarks = pht.estimateLandmarks(refImage, sceneImage,
			lmPath, angleDiff, ePoint, sgmethod);

	/*save to file*/
	/*QString scenename = sceneImage.getName();
	 QString sceneLMPath = "/home/linh/Desktop/mg/landmarks/";
	 QString savePath = "results/estimated/";
	 QString spath = sceneLMPath + "/" + scenename + ".TPS";
	 vector<Point> orgLandmarks = sceneImage.readLandmarksFile(
	 spath.toStdString().c_str());*/

	Mat sRotateImg = Image::rotateImage(sMatrix, angleDiff, ePoint);

	for (size_t i = 0; i < esLandmarks.size(); i++) {
		Point lm(refLandmarks.at(i).x, mMatrix.rows - refLandmarks.at(i).y);
		Point tLocation, tDistance, iLocation, iDistance;
		Mat templ = createTemplate(mMatrix, lm, templSize, tLocation,
				tDistance);
		Mat sceneM = createTemplate(sRotateImg, esLandmarks.at(i), sceneSize,
				iLocation, iDistance);
		Point maxLoc = matCrossCorrelation(templ, sceneM);
		mcResult.push_back(iLocation + maxLoc + tDistance);
	}

	t2 = clock();
	qDebug() << "Time matching: " << ((float) t2 - (float) t1) / CLOCKS_PER_SEC
			<< " seconds";
	Mat sDisplay(sMatrix.clone());
	//drawing the original landmarks
	/*for (size_t k = 0; k < orgLandmarks.size(); k++) {
	 Point orgPoint(orgLandmarks.at(k).x,
	 sMatrix.rows - orgLandmarks.at(k).y);
	 circle(sDisplay, orgPoint, 5, Scalar(0, 0, 255), 2, 8);

	 stringstream ss;
	 ss << k;
	 string s;
	 s = ss.str();
	 cv::putText(sDisplay, s, orgPoint, FONT_HERSHEY_COMPLEX, 1,
	 Scalar(255, 0, 0), 1, 8);
	 }*/

	Mat result(Image::rotateImage(sDisplay, angleDiff, ePoint).clone());
	// drawing the estimated landmarks
	for (size_t j = 0; j < mcResult.size(); j++) {
		Point esPoint = mcResult.at(j);
		circle(result, esPoint, 5, Scalar(0, 255, 255), 2, 8);

		stringstream ss;
		ss << j;
		string s;
		s = ss.str();
		cv::putText(result, s, esPoint, FONT_HERSHEY_COMPLEX, 1,
				Scalar(0, 0, 255), 1, 8);
	}

	return result;
}

vector<Point> LandmarkDetection::matchingTemplate(Image mImage, Image sImage,
		vector<Point> manualLM, vector<Point> esLandmarks, int templSize,
		int sceneSize, double angleDiff, Point ePoint) {
	Mat mMatrix = mImage.getMatrixImage();
	Mat sRotateImg = Image::rotateImage(sImage.getMatrixImage(), angleDiff, ePoint);
	vector<Point> mcResult;
	for (size_t i = 0; i < esLandmarks.size(); i++) {
		Point lm(manualLM.at(i).x, mMatrix.rows - manualLM.at(i).y);
		Point tLocation, tDistance, iLocation, iDistance;
		Mat templ = createTemplate(mMatrix, lm, templSize, tLocation,
				tDistance);
		Mat sceneM = createTemplate(sRotateImg, esLandmarks.at(i), sceneSize,
				iLocation, iDistance);
		Point maxLoc = matCrossCorrelation(templ, sceneM);
		mcResult.push_back(iLocation + maxLoc + tDistance);
	}
	return mcResult;
}
void LandmarkDetection::save_Estimated_Landmarks_To_File(
		vector<Point> esLandmarks, string path) {
	ofstream of(path.c_str());
	of << "LM = " << esLandmarks.size() << "\n";
	for (size_t i = 0; i < esLandmarks.size(); i++) {
		Point esPoint = esLandmarks.at(i);
		of << esPoint.x << " " << esPoint.y << "\n";
	}
	of << "FILE = " << path.c_str();
	of.close();
}
void LandmarkDetection::save_Original_And_Estimated_Landmarks_To_File(
		vector<Point> orgLandmarks, vector<Point> esLandmarks, string path) {
	ofstream of(path.c_str());
	size_t size =
			(orgLandmarks.size() > esLandmarks.size()) ?
					esLandmarks.size() : orgLandmarks.size();
	for (size_t i = 0; i < size; i++) {
		Point esPoint = esLandmarks.at(i);
		Point orgPoint = orgLandmarks.at(i);
		of << orgPoint.x << " " << orgPoint.y << "\t" << esPoint.x << " "
				<< esPoint.y << "\t";
		Line ldistance(esPoint, orgPoint);
		of << ldistance.length() << "\n";
	}
	of << "FILE = " << path.c_str() << "\n";
	of.close();
}
/*
 * Estimate the landmarks based on the proposed method between reference image an the images in folder
 * @parameter 1: refImage - the reference image
 * @parameter 2: folderImages - the images folder
 * @parameter 3: lmPath - the reference landmarks file
 * @parameter 4: savePath - the save folder path
 * @parameter 5: templSize - size of box around reference landmarks
 * @parameter 6: sceneSize - size of box around estimated landmarks
 * @parameter 7: angleDiff - the difference angle between two images
 * @return: the list of estimated landmarks and present it on the scene image
 */
void LandmarkDetection::matchingDirectory(Image refImage, string folderImages,
		string lmPath, string savePath, int templSize, int sceneSize,
		double angleDiff, Image::SegmentMethod sgmethod, int save) {
	QFileInfoList files = Image::readImagesFolder(folderImages);

	string runRoot = savePath;
	string spath = runRoot.append("/TimeToRun.txt");
	ofstream of(spath.c_str());
	clock_t t1, t2;
	t1 = clock();

	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name.toStdString());

		QString scenename = sceneImage.getName();
		qDebug() << scenename;

		vector<Point> mcresult;
		Point ePoint;
		Mat result = estimateLandmarks(refImage, sceneImage, lmPath, templSize,
				sceneSize, angleDiff, sgmethod, mcresult, ePoint);
		string imgRoot = savePath;
		string saveImg =
				imgRoot.append("/").append(scenename.toStdString()).append(
						".JPG");
		imwrite(saveImg.c_str(), result);

		if (save == 1) {
			string tpsRoot = savePath;
			string estTPS = tpsRoot.append("/E_").append(
					scenename.toStdString()).append(".TPS");
			save_Estimated_Landmarks_To_File(mcresult, estTPS);
		}
		/*QString spath = sceneLMPath + "/" + scenename + ".TPS";
		 vector<Point> orgLandmarks = sceneImage.readLandmarksFile(
		 spath.toStdString().c_str());

		 QString estTPS(savePath + "/E_" + scenename + ".TPS");
		 ofstream of(estTPS.toStdString().c_str());
		 of << "LM = " << mcresult.size() << "\n";

		 QString diffEMTPS(savePath + "/DiffEM " + scenename + ".TPS");
		 ofstream diffFile(diffEMTPS.toStdString().c_str());
		 diffFile << "IMAGE = " << scenename.toStdString().c_str() << ".JPG"
		 << "\n";

		 qDebug() << orgLandmarks.size() << ", " << mcresult.size();
		 for (size_t k = 0; k < mcresult.size(); k++) {
		 Point esPoint = mcresult.at(k);
		 of << esPoint.x << " " << esPoint.y << "\n";

		 Point orgPoint = orgLandmarks.at(k);
		 diffFile << orgPoint.x << " " << orgPoint.y << "\t" << esPoint.x
		 << " " << esPoint.y << "\t";
		 Line ldistance(esPoint, orgPoint);
		 diffFile << ldistance.length() << "\n";
		 }
		 diffFile.close();
		 of << "IMAGE = " << scenename.toStdString().c_str() << ".JPG";
		 of.close();*/
	}
	t2 = clock();
	of << "Total estimation time: "
			<< ((float) t2 - (float) t1) / CLOCKS_PER_SEC << " seconds";
	of.close();
}

/*
 * Compute the centroid of estimated landmarks
 * @parameter 1: refImage - the reference image
 * @parameter 2: sceneImage - the scene image
 * @parameter 3: lmPath - the reference landmarks file
 * @parameter 4: templSize - size of box around reference landmarks
 * @parameter 5: sceneSize - size of box around estimated landmarks
 * @parameter 6: ebary - the esimated center point
 * @return: the list of estimated landmarks and present it on the scene image
 */
double LandmarkDetection::centroidMatching(Image refImage, Image sceneImage,
		string lmPath, int templSize, int sceneSize,
		Image::SegmentMethod sgmethod, Point &ebary) {
	double angleDiff = 0, eCentroid = 0;
	vector<Point> mcResult;
	Point ePoint;
	estimateLandmarks(refImage, sceneImage, lmPath, templSize, sceneSize,
			angleDiff, sgmethod, mcResult, ePoint);
	eCentroid = measureDistance(mcResult, ebary);
	return eCentroid;
}
/*
 * Compute the centroid of estimated landmarks (on directory)
 * @parameter 1: refImage - the reference image
 * @parameter 2: lmPath - the reference landmarks file
 * @parameter 3: folderImages - the images folder
 * @parameter 4: folderlandmarks - the original landmarks folder
 * @parameter 5: templSize - size of box around reference landmarks
 * @parameter 6: sceneSize - size of box around estimated landmarks
 * @return: the list of estimated landmarks and present it on the scene image
 */
void LandmarkDetection::centroidMatchingDirectory(Image refImage, string lmPath,
		string folderImages, string folderlandmarks, int templSize,
		int sceneSize, string savePath, Image::SegmentMethod sgmethod) {

	QFileInfoList files = Image::readImagesFolder(folderImages);
	string centRoot = savePath;
	string saveFile = centRoot.append("/ECentroidMatching").append(
			Image::getTimeName().toStdString()).append(".PHG");
	ofstream of(saveFile.c_str());
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name.toStdString());

		QString scenename = sceneImage.getName();
		string sRoot = folderlandmarks;
		string spath = sRoot.append("/").append(scenename.toStdString()).append(
				".TPS");
		cout << spath << "\n";
		vector<Point> orgLandmarks = sceneImage.readLandmarksFile(
				spath.c_str());
		vector<Point> mcResult;
		double angle = 0;
		Point ePoint;
		estimateLandmarks(refImage, sceneImage, lmPath, templSize, sceneSize,
				angle, sgmethod, mcResult, ePoint);
		of << scenename.toStdString().c_str() << ".JPG" << "\t";
		qDebug() << scenename;
		Point mBary, eBary;
		double mCentroid, eCentroid;
		double distance = measureDistance(orgLandmarks, mcResult, mCentroid,
				eCentroid, mBary, eBary);

		of << "(" << mBary.x << "," << mBary.y << ")" << "\t" << "(" << eBary.x
				<< "," << eBary.y << ")" << "\t";

		of << mCentroid << "\t" << eCentroid << "\t" << distance << "\n";
		qDebug() << "value: (" << mBary.x << "," << mBary.y << ")" << "\t"
				<< "(" << eBary.x << "," << eBary.y << ")" << "\t" << mCentroid
				<< "\t" << eCentroid << "\t" << distance;
	}
	of.close();
}

/*
 * Compute the centroid of estimated landmarks (on directory)
 * @parameter 1: refImage - the reference image
 * @parameter 2: lmPath - the reference landmarks file
 * @parameter 3: folderImages - the images folder
 * @parameter 4: templSize - size of box around reference landmarks
 * @parameter 5: sceneSize - size of box around estimated landmarks
 * @return: the list of estimated landmarks and present it on the scene image
 */
void LandmarkDetection::centroidMatchingDirectory(Image refImage, string lmPath,
		string folderImages, int templSize, int sceneSize, string savePath,
		Image::SegmentMethod sgmethod) {
	QFileInfoList files = Image::readImagesFolder(folderImages);
	string saveRoot = savePath;
	string saveFile = saveRoot.append("/ECentroidMatching").append(
			Image::getTimeName().toStdString()).append(".PHG");
	ofstream of(saveFile.c_str());
	for (int i = 0; i < files.size(); i++) {
		QFileInfo file = files.at(i);
		QString _name = file.absoluteFilePath();
		Image sceneImage(_name.toStdString());

		QString scenename = sceneImage.getName();
		vector<Point> mcResult;
		double angle = 0;
		Point ePoint;
		estimateLandmarks(refImage, sceneImage, lmPath, templSize, sceneSize,
				angle, sgmethod, mcResult, ePoint);
		of << scenename.toStdString().c_str() << ".JPG" << "\t";
		qDebug() << scenename;
		Point eBary(0, 0);
		double eCentroid = 0;
		eCentroid = measureDistance(mcResult, eBary);

		of << "(" << eBary.x << "," << eBary.y << ")" << "\t";

		of << eCentroid << "\n";
		qDebug() << "value: (" << eBary.x << "," << eBary.y << ")" << "\t"
				<< eCentroid;
	}
	of.close();
}
Mat LandmarkDetection::loadOriginalLandmarks(Image image, string lmPath,
		vector<Point> &orgLandmarks) {
	return image.loadOriginalLandmarks(image.getMatrixImage(), lmPath,
			orgLandmarks);
}
} /* namespace impls_2015 */
