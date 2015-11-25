/*
 * Scenario.cpp
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

#include "Scenario.h"

namespace impls_2015 {
/**
 * Constructor
 */
Scenario::Scenario() {

}

Scenario::~Scenario() {

}
/*
 * Edge segmentation an image
 * @parameter 1: image - the input image
 * @parameter 2 - output: result - the matrix presented the segmentation result
 * @return: List of approximate line of edge segmentation
 */
vector<Line> Scenario::edgeSegmentation(Image image, cv::Mat &result,
		Image::SegmentMethod sgmethod) {
	vector<Line> appLines;
	EdgeSegmentation edgeSegment;
	appLines = edgeSegment.lineSegment(image, sgmethod);
	result = edgeSegment.rePresentation(result, appLines);
	return appLines;
}

/*
 * Edge segmentation on a directory
 * @parameter 1: inputFolder - the images folder path
 * @parameter 2: saveFolder - the saving folder path
 * @return: the segmentation result presented on Mat (convert into image)
 * 			and save in the saving folder
 */
void Scenario::edgeSegmentationDirectory(QString inputFolder,
		QString saveFolder, Image::SegmentMethod segmentMethod, int save) {
	EdgeSegmentation edgeSegment;
	edgeSegment.segmentDirectory(inputFolder.toStdString(),
			saveFolder.toStdString(), segmentMethod, save);
}

/*
 * Compute the pairwise geometric histogram of an image
 * @parameter 1: image - the input image
 * @parameter 2: angleAcc - the accuracy of angle axis (90, 180, 360, 720, 1080, 2160, 10800)
 * @parameter 3: columns - the accuracy of distance axis (250, 500, 1000)
 * @parameter 4: result - the matrix presented the result
 * @return: the list of local PGH
 */
vector<LocalHistogram> Scenario::pairwiseHistogram(Image image,
		LocalHistogram::AccuracyPGH angleAcc, int columns,
		Image::SegmentMethod sgmethod, cv::Mat &result,
		vector<vector<int> > &matrix) {
	GeometricHistogram geomHistogram;
	vector<LocalHistogram> pghHist = geomHistogram.shapeHistogram(image,
			angleAcc, columns, sgmethod, result, matrix);
	return pghHist;
}

/*
 * Compute the PGH of images in a directory with an accuracy
 * @parameter 1: folderPath - the image folder path
 * @parameter 2: angleAcc - the accuracy of angle axis (90, 180, 360, 720, 1080, 2160, 10800)
 * @parameter 3: columns - the accuracy of distance axis (250, 500, 1000)
 */
void Scenario::pairwiseHistogramDirectory(QString folderPath,
		LocalHistogram::AccuracyPGH angleAcc, int columns) {
	qDebug() << "Pairwise for directory: ";
	GeometricHistogram geomHistogram;
	geomHistogram.pairwiseHistogramDirectory(folderPath.toStdString(), angleAcc,
			columns);
}

/*
 * Compute the PGH measure distance between two images
 * @parameter 1: refImage - the reference image
 * @parameter 2: sceneImage - the scene image
 * @parameter 3: matching - the matching method (Bhattacharyya, Chi-squared, intersection)
 * @parameter 4: angleAcc - the accuracy of angle axis (90, 180, 360, 720, 1080, 2160, 10800)
 * @parameter 5: columns - the accuracy of distance axis (250, 500, 1000)
 * @return: the value presented for the agree between two images
 */
double Scenario::pghMatching(Image refImage, Image sceneImage,
		GeometricHistogram::MatchingMethod matching,
		LocalHistogram::AccuracyPGH angleAcc, int distanceAcc,
		Image::SegmentMethod sgmethod) {
	GeometricHistogram geomHistogram;
	return geomHistogram.pghHistogramMatching(refImage, sceneImage, matching,
			angleAcc, distanceAcc, sgmethod);
}

/*
 * Compute the PGH measure distance between a reference image and the images in a folder
 * @parameter 1: refImage - the reference image
 * @parameter 2: directoryPath - the images folder path
 * @parameter 3: matching - the matching method (Bhattacharyya, Chi-squared, intersection)
 * @parameter 4: angleAcc - the accuracy of angle axis (90, 180, 360, 720, 1080, 2160, 10800)
 * @parameter 5: columns - the accuracy of distance axis (250, 500, 1000)
 */
void Scenario::matchingDirectory(Image refImage, QString directoryPath,
		GeometricHistogram::MatchingMethod matching,
		LocalHistogram::AccuracyPGH angleAcc, int distanceAcc, QString fileSave,
		Image::SegmentMethod sgmethod) {
	qDebug() << "Matching directory";
	GeometricHistogram geomHistogram;
	geomHistogram.pghHistogramDirectoryMatching(refImage,
			directoryPath.toStdString(), matching, angleAcc, distanceAcc,
			fileSave.toStdString(), sgmethod);
}

/*
 * Compute the PGH measure distance between pair of images in folders
 * @parameter 1: directoryPath - the images folder path
 * @parameter 2: matching - the matching method (Bhattacharyya, Chi-squared, intersection)
 * @parameter 3: angleAcc - the accuracy of angle axis (90, 180, 360, 720, 1080, 2160, 10800)
 * @parameter 4: columns - the accuracy of distance axis (250, 500, 1000)
 */
void Scenario::matchingDirectory(QString directoryPath,
		GeometricHistogram::MatchingMethod matching,
		LocalHistogram::AccuracyPGH angleAcc, int distanceAcc) {
	qDebug() << "Matching directory";
	GeometricHistogram geomHistogram;
	geomHistogram.phgHistogramDirMatching(directoryPath.toStdString(), matching,
			angleAcc, distanceAcc);
}

/*
 * Estimated the landmarks of scene image based on a reference image and its landmarks
 * @parameter 1: refImage - the reference image
 * @parameter 2: sceneImage - the scene image
 * @parameter 3: reflmPath - the path of file contains the reference landmarks
 * @return: the matrix presented the estimated landmarks on the scene image
 */
Mat Scenario::probabilisticHoughTransform(Image refImage, Image sceneImage,
		QString reflmPath, Image::SegmentMethod sgmethod,
		vector<Point> &esLandmarks) {
	PHoughTransform pht;

	return pht.phtPresentation(refImage, sceneImage, reflmPath.toStdString(),
			esLandmarks, sgmethod);
}

/*
 * Estimated the landmarks of a reference image on the images in a folder
 * @parameter 1: refImage - the reference image
 * @parameter 2: reflmPath - the path of reference landmarks file
 * @parameter 3: sceneDir - the path of scene images folder
 * @parameter 4: scenelmDir - the path of scene landmarks folder
 * @parameter 5: saveDir - the path of saving directory
 */
void Scenario::phtDirectory(Image refImage, QString reflmPath, QString sceneDir,
		QString scenelmDir, QString saveDir, Image::SegmentMethod sgmethod,
		int save) {
	PHoughTransform pht;
	pht.phtDirectory(refImage, reflmPath.toStdString(), sceneDir.toStdString(),
			scenelmDir.toStdString(), saveDir.toStdString(), sgmethod, save);
}

/**
 * Estimated the landmarks based on the cross-correlation method
 * @parameter 1: image - the reference image
 * @parameter 2: lpath - the path of reference landmarks file
 * @parameter 3: sceneImage - the scene image
 * @return: the list of estimated landmarks of reference image on the scene image
 */
vector<Point> Scenario::landmarksByCrossCorelation(Image image, QString lpath,
		Image sceneImage, int tsize) {

	vector<Point> landmarks;
	LandmarkDetection lmdetection;
	landmarks = lmdetection.crossCorrelation(image, sceneImage,
			lpath.toStdString(), tsize);
	return landmarks;
}

/*
 * Estimated the landmarks of a reference image on the images in a folder
 * @parameter 1: refImage - the reference image
 * @parameter 2: path - the scene images folder path
 * @parameter 3: savePath - the path of folder save the result
 * @parameter 4: lmPath - the path of reference landmarks file
 */
void Scenario::cCorelationDirectory(Image refImage, QString path,
		QString savePath, QString lmPath) {
	LandmarkDetection lmdetection;
	lmdetection.cCorrelationByDirectory(refImage, path.toStdString(),
			savePath.toStdString(), lmPath.toStdString());
}

/*
 * Compute the centroid of estimated landmarks.
 * The estimated landmarks based on the cross-correlation method
 * @parameter 1: image - the reference image
 * @parameter 2: lmpath - the path of reference landmarks file
 * @parameter 3: sceneImage - the scene image
 * @parameter 4 - output: ebary - the bary point of the landmarks
 * @return: the centroid of estimated landmarks
 */
double Scenario::mDistanceByCrossCorrelation(Image image, QString lmpath,
		Image sceneImage, int size, Point &ebary) {
	LandmarkDetection lmdetection;
	double rs = lmdetection.centroidCCorrelation(image, sceneImage,
			lmpath.toStdString(), size, ebary);
	return rs;
}

/*
 * Compute the centroid measure distance of a reference image and the scene images in a folder.
 * The estimated landmarks based on the cross-correlation method
 * @parameter 1: refImage - the reference image
 * @parameter 2: lmpath - the path of reference landmarks file
 * @parameter 3: imgFolder - the scene images folder
 */
void Scenario::mDistanceByCrossCorrelationDir(Image refImage, QString lmpath,
		QString imgFolder) {
	// centroid point with folder
	//QString lmfolder = "/home/linh/Desktop/landmarks";
	QString lmfolder = "/home/linh/Desktop/mg/landmarks";
	LandmarkDetection lmdetection;
//	lmdetection.centroidCCorelations(refImage, lmpath, imgFolder);
	lmdetection.centroidCCorrelations(refImage, lmpath.toStdString(),
			imgFolder.toStdString(), lmfolder.toStdString());
}

/*
 * Estimate the reference landmarks on scene image based on the template matching (followed article)
 * @parameter 1: refImage - the reference image
 * @parameter 2: sceneImage - the scene image
 * @parameter 3: reflmPath - the path of reference landmarks file
 * @parameter 4: templSize - size of reference landmark bounding box
 * @parameter 5: scnSize - size of estimated landmark bounding box
 * @return: the matrix presented the estimated landmarks
 */
Mat Scenario::landmarksMatching(Image refImage, Image sceneImage,
		QString reflmPath, int templSize, int scnSize,
		Image::SegmentMethod sgmethod, vector<Point> &mcResult, double &angle,
		Point &ePoint) {
	LandmarkDetection lmdetection;
	return lmdetection.estimateLandmarks(refImage, sceneImage,
			reflmPath.toStdString(), templSize, scnSize, angle, sgmethod,
			mcResult, ePoint);
}

/*
 * Estimate the reference landmarks on each image in scene images folder
 * based on the template matching
 * @parameter 1: refImage - the reference image
 * @parameter 2: folderImage - the scene images folder
 * @parameter 3: lmPath - the reference landmarks path
 * @parameter 4: savePath - the saving folder path
 * @parameter 5: templSize - size of reference landmarks bounding box
 * @parameter 6: sceneSize - size of estimated landmark bounding box
 */
void Scenario::landmarksMatchingDirectory(Image refImage, QString folderImages,
		QString lmPath, QString savePath, int templSize, int sceneSize,
		Image::SegmentMethod sgmethod, int save) {

	LandmarkDetection lmdetection;
	double angle = 0;
	lmdetection.matchingDirectory(refImage, folderImages.toStdString(),
			lmPath.toStdString(), savePath.toStdString(), templSize, sceneSize,
			angle, sgmethod, save);

}

/*
 * Compute centroid of estimated landmarks. The estimated landmarks based on the template matching
 * @parameter 1: refImage - the reference image
 * @parameter 2: sceneImage - the reference image
 * @parameter 3: lmPath - the reference landmarks path
 * @parameter 4 - output: ebary - the bary point of the landmarks
 * @return: the centroid of estimated landmarks
 */
double Scenario::mDistanceByTemplateMatching(Image refImage, Image sceneImage,
		QString lmPath, int templSize, int sceneSize,
		Image::SegmentMethod sgmethod, Point &ebary) {
	LandmarkDetection lmdetection;
	return lmdetection.centroidMatching(refImage, sceneImage,
			lmPath.toStdString(), templSize, sceneSize, sgmethod, ebary);
}

/*
 * Compute the centroid of estimated landmarks of the images in a folder
 * @parameter 1: refImage - the reference image
 * @parameter 2: lmPath - the reference landmarks path
 * @parameter 3: folderImages - the scene images folder
 * @parameter 4: templSize - size of reference landmarks bounding box
 * @parameter 5: sceneSize - size of estimated landmark bounding box
 */
void Scenario::mDistanceByTemplateMatchingDirectory(Image refImage,
		QString lmPath, QString folderImages, QString lmFolder, int templSize,
		int sceneSize, QString savePath, Image::SegmentMethod sgmethod) {
	// compute the centroid
	LandmarkDetection lmdetection;
	//lmdetection.centroidMatchingDirectory(refImage, lmPath, folderImages,templSize,sceneSize);
	lmdetection.centroidMatchingDirectory(refImage, lmPath.toStdString(), folderImages.toStdString(),
			lmFolder.toStdString(), templSize, sceneSize, savePath.toStdString(), sgmethod);
}

Mat Scenario::loadOriginalLandmarks(Image image, QString lmPath,
		vector<Point> &orgLandmarks) {
	LandmarkDetection lmdetection;
	return lmdetection.loadOriginalLandmarks(image, lmPath.toStdString(), orgLandmarks);
}

} /* namespace impls_2015 */
