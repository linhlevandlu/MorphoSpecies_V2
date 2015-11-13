/*
 Image processing for morphometrics (IPM)
 Copyright (C) 2014  Nguyen Hoang Thao (hoangthao.ng@gmail.com)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <math.h>
#include <QtGui/qradiobutton.h>

#include <fstream>
#include <iostream>
#include <ostream>

#include "impls_2015/Line.h"
#include "impls_2015/Edge.h"
#include "impls_2015/Image.h"
#include "impls_2015/landmarks/LandmarkDetection.h"
#include "impls_2015/segmentation/EdgeSegmentation.h"
#include "impls_2015/Scenario.h"
#include "impls_2015/GFeatures.h"
#include "impls_2015/ShapeHistogram.h"
#include "impls_2015/IDrawOperation.h"
#include "impls_2015/LocalHistogram.h"
#include "impls_2015/pht/PHoughTransform.h"
#include "impls_2015/segmentation/EdgeSegmentationPanel.h"
#include "impls_2015/ReadResouces.h"

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
class QCloseEvent;
class QContextMenuEvent;
class QToolBar;
class QDialog;

class ImageViewer: public QMainWindow {
Q_OBJECT

public:
	ImageViewer();
	QString fileName;
	cv::Mat matImage;
	QImage qImage;
	void loadImage(QString fn);
	void loadImage(cv::Mat _matImage, QImage _qImage, QString tt);

	void addParameterPanel(QDialog *pnl, int x, int y);

	QAction *parameterAct;
	QDialog *parameterPanel;

	void activeFunction();
	void normalBlurAction(int kernelSize);
	void gaussianBlurAction(int kernelSize);
	void medianBlurAction(int kernelSize);
	void bilateralFilterAction(int sigmaColor);
	void convolutionFilterAction(int kernelSize);

	void erosionAction(int kernelSize, int typeEle);
	void dilationAction(int kernelSize, int typeEle);
	void morphoExtAction(int kernelSize, int type);

	void thresholdAction(int value, int type);
	void sobelAction(int kernelSize, int scale);
	void scharrAction(int scale);
	void laplaceAction(int kernelSize, int scale);
	void cannyAction(int kernelSize, int low, int ratio);

	void featureSurfAction(int minHessian, bool refresh = true);

	//add by LE Van Linh
	//void removeYLinesAction(int minBrightness, QString pathImage);

	void matchingDirectory(impls_2015::Image image, QString path, QString fileSave, impls_2015::Image::SegmentMethod sgmethod);
	void edgeSegmentation_Value_Changed(QString filePath, int thresholdValue);
	void edgeSegmentation_Method_Changed(QString filePath,
			impls_2015::Image::SegmentMethod sgmethod);

	impls_2015::Image::SegmentMethod chooseSegMethod();
	int saveOrNot();

	vector<Point> esLandmarks;
	vector<Point> orgLandmarks;
	vector<impls_2015::Line> lines;
	double angleDiff;
	Point ePoint;

	//end
protected:
	void closeEvent(QCloseEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);

private slots:
	void open();
	void print();
	bool save();
	bool saveAs();
	void zoomIn();
	void zoomOut();
	void normalSize();
	void fitToWindow();
	void about();

	void parameterAction();
	void normalBlurAction();
	void gaussianBlurAction();
	void medianBlurAction();
	void bilateralFilterAction();
	void convolutionFilterAction();

	void erosionAction();
	void dilationAction();
	void morphoExtAction();

	void thresholdAction();
	void sobelAction();
	void scharrAction();
	void laplaceAction();
	void cannyAction();

	void houghLineAction();
	void houghCircleAction();
	void findContourAction();
	void convexHullAction();
	void bdBoxCircleAction();
	void bdRotEclipseAction();
	void imgMomentsAction();

	void harrisAction();
	void shiTomasiAction();
	void cornerDetectAction();

	void featureSurfAction();
	void featureSiftAction();
	void matchingBFAction();
	void matchingFLANNAction();
	void matchingHomographyAction();
	void matchingTemplateAction();
	void correctMorphAction();

	void pyramidAction();
	void remapAction();
	void affineAction();
	void histogramAction();
	void equalizationAction();
	void backprojectionAction();

	void grayscaleMyImplAction();
	void binaryMyImplAction();
	void robertsMyImplAction();
	void sobelMyImplAction();
	void hitmissMyImplAction();
	void otsuMyImplAction();
	void growingMyImplAction();
	void antMyImplAction();

	// add by LE Van Linh
	void removeYLinesAction();
	void edgeSegmentation();
	void pairwiseHistogram();
	void pwBhattacharyyaMatching();
	void pwChiSquaredMatching();
	void pwIntersectionMatching();
	void getLandmarks();
	void pHoughTransform();
	void crossCorrelation();
	void crossCorrelationDistance();
	void tplMatchingDistance();
	void edgeSegmentDirectory();
	void bhattacharyyaDistanceDirectory();
	void phtOnDirectory();
	void estlmOnDirectory();
	void computeSizeOnDirectory();
	void loadOriginalLandmarks();

	// context menu
	void estContextMenu(const QPoint& pos);
	void putInOrgLandmarks();
	void putInLines();
	void putOutOrgLandmarks();
	void putOutLines();
private:
	void createActions();
	void createMenus();
	void updateActions();
	void scaleImage(double factor);
	void adjustScrollBar(QScrollBar *scrollBar, double factor);
	void createToolBars();
	void createStatusBar();
	void readSettings();
	void writeSettings();

	QLabel *imageLabel;
	QScrollArea *scrollArea;
	double scaleFactor;

	// Linh
	void optionDisplay(impls_2015::Image image, vector<Point> esLM,
			vector<Point> orgLM, vector<impls_2015::Line> lines, double angle, Point ePoint);
	bool checkPresent(impls_2015::Image mImage, impls_2015::Image sImage,impls_2015::Image::SegmentMethod sgmethod);

#ifndef QT_NO_PRINTER
	QPrinter printer;
#endif

	QAction *openAct;
	QAction *printAct;
	QAction *saveAct;
	QAction *saveAsAct;
	QAction *closeAct;
	QAction *exitAct;
	QAction *zoomInAct;
	QAction *zoomOutAct;
	QAction *normalSizeAct;
	QAction *fitToWindowAct;
	QAction *aboutAct;

	QAction *normalBlurAct;
	QAction *gaussianBlurAct;
	QAction *medianBlurAct;
	QAction *bilateralFilterAct;
	QAction *convolutionFilterAct;

	QAction *erosionAct;
	QAction *dilationAct;
	QAction *morphoExtAct;

	QAction *thresholdAct;
	QAction *sobelAct;
	QAction *scharrAct;
	QAction *laplaceAct;
	QAction *cannyAct;

	QAction *houghLineAct;
	QAction *houghCircleAct;
	QAction *imgMomentsAct;
	QAction *findContourAct;
	QAction *convexHullAct;
	QAction *bdBoxCircleAct;
	QAction *bdRotEclipseAct;

	QAction *harrisAct;
	QAction *shiTomasiAct;
	QAction *cornerDetectAct;

	QAction *featureSurfAct;
	QAction *featureSiftAct;
	QAction *matchingBFAct;
	QAction *matchingFLANNAct;
	QAction *matchingHomographyAct;
	QAction *matchingTemplateAct;

	QAction *correctMorphAct;

	QAction *pyramidAct;
	QAction *remapAct;
	QAction *affineAct;
	QAction *histogramAct;
	QAction *equalizationAct;
	QAction *backprojectionAct;

	QAction *grayscaleMyImplAct;
	QAction *binaryMyImplAct;
	QAction *robertsMyImplAct;
	QAction *sobelMyImplAct;
	QAction *hitmissMyImplAct;
	QAction *otsuMyImplAct;
	QAction *growingMyImplAct;
	QAction *antMyImplAct;

//add by LE Van Linh
	QAction *removeLinesAct; // remove yellow lines using the algorithm 1
	QAction *landmarksDetection; // detect landmarks
	QAction *edgeSegment; // edge segmentation (get edge and break into a set of lines)
	QAction *pwHistogram; // compute the PGHs of an image
	QAction *pwhMatching; // matching 2 PGH of 2 image using Bhattachyya metric
	QAction *pwhChisquared; // matching 2 PGH using chi-squared metric
	QAction *pwhIntersection; // matching 2 PGH using intersection
	QAction *phTransform;
	QAction *cCorrelation; // cross correlation
	QAction *ccMeasureDistance;
	QAction *tmMeasureDistance;
	QAction *loadOrgLandmarks;

	// working with directory
	QAction *segmentationDir;
	QAction *bhattcharyyaDir;
	QAction *proHoughTransformDir;
	QAction *lmExtractionDir;
	QAction *sizeLandmarksDir;

// end
	QMenu *fileMenu;
	QMenu *viewMenu;
	QMenu *opencvMenu;
	QMenu *myimplMenu;
	QMenu *pluginsMenu;
	QMenu *helpMenu;
//add by LE Van Linh
	QMenu *segment;
	QMenu *landmarks;
	QMenu *utilities;
	QMenu *other2015;

//end
	QToolBar *fileToolBar;
	QToolBar *viewToolBar;

};

#endif
