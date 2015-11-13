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

#include <QtGui/QApplication>
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#include <QtGui/QScrollBar>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QPrintDialog>
#include <QtGui/QAction>
#include <QtGui/QMenuBar>
#include <QtGui/QPainter>
#include <QtGui/QCloseEvent>
#include <QtCore/QSettings>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QMessageBox>
#include <QtCore/QDebug>
#include <QtGui/QIcon>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"

#include "ImageViewer.h"
#include "ImageConvert.h"
#include "ui/NormalBlurPanel.h"
#include "ui/GaussianBlurPanel.h"
#include "ui/MedianBlurPanel.h"
#include "ui/BilateralFilterPanel.h"
#include "ui/ErosionPanel.h"
#include "ui/DilationPanel.h"
#include "ui/MorphoExtPanel.h"
#include "ui/ConvolutionFilterPanel.h"
#include "ui/ThresholdingPanel.h"
#include "ui/SobelPanel.h"
#include "ui/ScharrPanel.h"
#include "ui/LaplacePanel.h"
#include "ui/CannyPanel.h"
#include "ui/SurfPanel.h"

#include "impls/algorithms/MyImpl.h"
#include "impls_2015/Image.h"

using namespace std;
using namespace algorithms;
using namespace impls_2015;

ImageViewer::ImageViewer() {
	imageLabel = new QLabel;
	imageLabel->setBackgroundRole(QPalette::Base);
	imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	imageLabel->setScaledContents(true);

	scrollArea = new QScrollArea;
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidget(imageLabel);
	setCentralWidget(scrollArea);

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();

	setWindowTitle(tr("Image Viewer"));
	resize(800, 600);

	setWindowIcon(QIcon(":/images/ip.ico"));
	parameterPanel = NULL;
}

void ImageViewer::activeFunction() {
	printAct->setEnabled(true);
	fitToWindowAct->setEnabled(true);

	normalBlurAct->setEnabled(true);
	gaussianBlurAct->setEnabled(true);
	medianBlurAct->setEnabled(true);
	bilateralFilterAct->setEnabled(true);
	convolutionFilterAct->setEnabled(true);

	erosionAct->setEnabled(true);
	dilationAct->setEnabled(true);
	morphoExtAct->setEnabled(true);

	thresholdAct->setEnabled(true);
	sobelAct->setEnabled(true);
	scharrAct->setEnabled(true);
	laplaceAct->setEnabled(true);
	cannyAct->setEnabled(true);

	houghLineAct->setEnabled(true);
	houghCircleAct->setEnabled(true);
	findContourAct->setEnabled(true);
	convexHullAct->setEnabled(true);
	bdBoxCircleAct->setEnabled(true);
	bdRotEclipseAct->setEnabled(true);
	imgMomentsAct->setEnabled(true);

	harrisAct->setEnabled(true);
	shiTomasiAct->setEnabled(true);
	cornerDetectAct->setEnabled(true);

	featureSurfAct->setEnabled(true);
	featureSiftAct->setEnabled(true);
	matchingBFAct->setEnabled(true);
	matchingFLANNAct->setEnabled(true);
	matchingHomographyAct->setEnabled(true);
	matchingTemplateAct->setEnabled(true);
	correctMorphAct->setEnabled(true);

	pyramidAct->setEnabled(true);
	remapAct->setEnabled(true);
	affineAct->setEnabled(true);
	histogramAct->setEnabled(true);
	equalizationAct->setEnabled(true);
	backprojectionAct->setEnabled(true);

	grayscaleMyImplAct->setEnabled(true);
	binaryMyImplAct->setEnabled(true);
	robertsMyImplAct->setEnabled(true);
	sobelMyImplAct->setEnabled(true);
	hitmissMyImplAct->setEnabled(true);
	otsuMyImplAct->setEnabled(true);
	growingMyImplAct->setEnabled(true);
	antMyImplAct->setEnabled(true);

	//add by LE Van Linh
	removeLinesAct->setEnabled(true);
	landmarksDetection->setEnabled(true);
	edgeSegment->setEnabled(true);
	pwHistogram->setEnabled(true);
	pwhMatching->setEnabled(true);
	pwhChisquared->setEnabled(true);
	pwhIntersection->setEnabled(true);
	phTransform->setEnabled(true);
	cCorrelation->setEnabled(true);
	ccMeasureDistance->setEnabled(true);
	tmMeasureDistance->setEnabled(true);
	segmentationDir->setEnabled(true);
	bhattcharyyaDir->setEnabled(true);
	proHoughTransformDir->setEnabled(true);
	lmExtractionDir->setEnabled(true);
	sizeLandmarksDir->setEnabled(true);
	loadOrgLandmarks->setEnabled(true);
	//end
	updateActions();

	if (!fitToWindowAct->isChecked())
		imageLabel->adjustSize();
}

void ImageViewer::loadImage(cv::Mat _matImage, QImage _qImage, QString tt) {

	matImage = _matImage;
	qImage = _qImage;
	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	scaleFactor = 1.0;

	saveAct->setEnabled(true);
	activeFunction();

	setWindowTitle(tr("Image Viewer - ") + tt);
	statusBar()->showMessage(tr("Finished"), 2000);
}

void ImageViewer::loadImage(QString fn) {

	matImage = cv::imread(fn.toStdString());
	qImage.load(fn);

	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	scaleFactor = 1.0;

	saveAsAct->setEnabled(true);
	activeFunction();

	this->fileName = fn;
	setWindowTitle(tr("Image Viewer - ") + fileName);
	statusBar()->showMessage(tr("File loaded"), 2000);
}

void ImageViewer::open() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
			QDir::currentPath());
	if (!fileName.isEmpty()) {
		QImage image(fileName);
		if (image.isNull()) {
			QMessageBox::information(this, tr("Image Viewer"),
					tr("Cannot load %1.").arg(fileName));
			return;
		}
		if (!this->fileName.isEmpty()) {
			ImageViewer* other = new ImageViewer;
			other->loadImage(fileName);
			other->move(x() + 40, y() + 40);
			other->show();
		} else {
			this->loadImage(fileName);
		}
	}
}

void ImageViewer::print() {
	Q_ASSERT(imageLabel->pixmap());
#ifndef QT_NO_PRINTER
	QPrintDialog dialog(&printer, this);
	if (dialog.exec()) {
		QPainter painter(&printer);
		QRect rect = painter.viewport();
		QSize size = imageLabel->pixmap()->size();
		size.scale(rect.size(), Qt::KeepAspectRatio);
		painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
		painter.setWindow(imageLabel->pixmap()->rect());
		painter.drawPixmap(0, 0, *imageLabel->pixmap());
	}
#endif
}

void ImageViewer::zoomIn() {
	scaleImage(1.25);
}

void ImageViewer::zoomOut() {
	scaleImage(0.8);
}

void ImageViewer::normalSize() {
	imageLabel->adjustSize();
	scaleFactor = 1.0;
}

void ImageViewer::fitToWindow() {
	bool fitToWindow = fitToWindowAct->isChecked();
	scrollArea->setWidgetResizable(fitToWindow);
	if (!fitToWindow) {
		normalSize();
	}
	updateActions();
}

void ImageViewer::about() {
	QMessageBox::about(this, tr("About Image Processing"),
			tr(
					"<p>The <b>Image Processing</b> shows how to process digital images.</p>"));
}

void ImageViewer::createActions() {
	printAct = new QAction(tr("&Print..."), this);
	printAct->setShortcut(tr("Ctrl+P"));
	printAct->setEnabled(false);
	connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

	openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(tr("Open an existing file"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save the document to disk"));
	saveAct->setEnabled(false);
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

	saveAsAct = new QAction(tr("Save &As..."), this);
	saveAsAct->setShortcuts(QKeySequence::SaveAs);
	saveAsAct->setStatusTip(tr("Save the document under a new name"));
	saveAsAct->setEnabled(false);
	connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

	closeAct = new QAction(tr("&Close"), this);
	closeAct->setShortcut(tr("Ctrl+W"));
	closeAct->setStatusTip(tr("Close this window"));
	connect(closeAct, SIGNAL(triggered()), this, SLOT(close()));

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcuts(QKeySequence::Quit);
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

	zoomInAct = new QAction(QIcon(":/images/1uparrow.png"),
			tr("Zoom &In (25%)"), this);
	zoomInAct->setShortcut(tr("Ctrl++"));
	zoomInAct->setEnabled(false);
	connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

	zoomOutAct = new QAction(QIcon(":/images/1downarrow.png"),
			tr("Zoom &Out (25%)"), this);
	zoomOutAct->setShortcut(tr("Ctrl+-"));
	zoomOutAct->setEnabled(false);
	connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

	normalSizeAct = new QAction(tr("&Normal Size"), this);
	normalSizeAct->setShortcut(tr("Ctrl+N"));
	normalSizeAct->setEnabled(false);
	connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

	fitToWindowAct = new QAction(tr("&Fit to Window"), this);
	fitToWindowAct->setEnabled(false);
	fitToWindowAct->setCheckable(true);
	fitToWindowAct->setShortcut(tr("Ctrl+F"));
	connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

	aboutAct = new QAction(tr("&About"), this);
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	parameterAct = new QAction(tr("Parameters"), this);
	connect(parameterAct, SIGNAL(triggered()), this, SLOT(parameterAction()));

	normalBlurAct = new QAction(tr("Normal blur"), this);
	normalBlurAct->setEnabled(false);
	connect(normalBlurAct, SIGNAL(triggered()), this, SLOT(normalBlurAction()));

	gaussianBlurAct = new QAction(tr("Gaussian blur "), this);
	gaussianBlurAct->setEnabled(false);
	connect(gaussianBlurAct, SIGNAL(triggered()), this,
			SLOT(gaussianBlurAction()));

	medianBlurAct = new QAction(tr("Median blur "), this);
	medianBlurAct->setEnabled(false);
	connect(medianBlurAct, SIGNAL(triggered()), this, SLOT(medianBlurAction()));

	bilateralFilterAct = new QAction(tr("Bilateral filter"), this);
	bilateralFilterAct->setEnabled(false);
	connect(bilateralFilterAct, SIGNAL(triggered()), this,
			SLOT(bilateralFilterAction()));

	convolutionFilterAct = new QAction(tr("Convolution filter"), this);
	convolutionFilterAct->setEnabled(false);
	connect(convolutionFilterAct, SIGNAL(triggered()), this,
			SLOT(convolutionFilterAction()));

	erosionAct = new QAction(tr("Erosion"), this);
	erosionAct->setEnabled(false);
	connect(erosionAct, SIGNAL(triggered()), this, SLOT(erosionAction()));

	dilationAct = new QAction(tr("Dilation"), this);
	dilationAct->setEnabled(false);
	connect(dilationAct, SIGNAL(triggered()), this, SLOT(dilationAction()));

	morphoExtAct = new QAction(tr("Morphology transform"), this);
	morphoExtAct->setEnabled(false);
	connect(morphoExtAct, SIGNAL(triggered()), this, SLOT(morphoExtAction()));

	thresholdAct = new QAction(tr("Thresholding"), this);
	thresholdAct->setEnabled(false);
	connect(thresholdAct, SIGNAL(triggered()), this, SLOT(thresholdAction()));

	sobelAct = new QAction(tr("Sobel method"), this);
	sobelAct->setEnabled(false);
	connect(sobelAct, SIGNAL(triggered()), this, SLOT(sobelAction()));

	scharrAct = new QAction(tr("Scharr method"), this);
	scharrAct->setEnabled(false);
	connect(scharrAct, SIGNAL(triggered()), this, SLOT(scharrAction()));

	laplaceAct = new QAction(tr("Laplace method"), this);
	laplaceAct->setEnabled(false);
	connect(laplaceAct, SIGNAL(triggered()), this, SLOT(laplaceAction()));

	cannyAct = new QAction(tr("Canny method"), this);
	cannyAct->setEnabled(false);
	connect(cannyAct, SIGNAL(triggered()), this, SLOT(cannyAction()));

	findContourAct = new QAction(tr("Finding contour"), this);
	findContourAct->setEnabled(false);
	connect(findContourAct, SIGNAL(triggered()), this,
			SLOT(findContourAction()));

	imgMomentsAct = new QAction(tr("Image moments"), this);
	imgMomentsAct->setEnabled(false);
	connect(imgMomentsAct, SIGNAL(triggered()), this, SLOT(imgMomentsAction()));

	houghLineAct = new QAction(tr("Hough line"), this);
	houghLineAct->setEnabled(false);
	connect(houghLineAct, SIGNAL(triggered()), this, SLOT(houghLineAction()));

	houghCircleAct = new QAction(tr("Hough circle"), this);
	houghCircleAct->setEnabled(false);
	connect(houghCircleAct, SIGNAL(triggered()), this,
			SLOT(houghCircleAction()));

	convexHullAct = new QAction(tr("Convex Hull"), this);
	convexHullAct->setEnabled(false);
	connect(convexHullAct, SIGNAL(triggered()), this, SLOT(convexHullAction()));

	bdBoxCircleAct = new QAction(tr("Bounding and Circle"), this);
	bdBoxCircleAct->setEnabled(false);
	connect(bdBoxCircleAct, SIGNAL(triggered()), this,
			SLOT(bdBoxCircleAction()));

	bdRotEclipseAct = new QAction(tr("Bounding Rotate and Eclipse"), this);
	bdRotEclipseAct->setEnabled(false);
	connect(bdRotEclipseAct, SIGNAL(triggered()), this,
			SLOT(bdRotEclipseAction()));

	harrisAct = new QAction(tr("Harris corner detector"), this);
	harrisAct->setEnabled(false);
	connect(harrisAct, SIGNAL(triggered()), this, SLOT(harrisAction()));

	shiTomasiAct = new QAction(tr("Shi-Tomasi corner detector"), this);
	shiTomasiAct->setEnabled(false);
	connect(shiTomasiAct, SIGNAL(triggered()), this, SLOT(shiTomasiAction()));

	cornerDetectAct = new QAction(tr("Corner detector"), this);
	cornerDetectAct->setEnabled(false);
	connect(cornerDetectAct, SIGNAL(triggered()), this,
			SLOT(cornerDetectAction()));

	featureSurfAct = new QAction(tr("SUFT feature detector"), this);
	featureSurfAct->setEnabled(false);
	connect(featureSurfAct, SIGNAL(triggered()), this,
			SLOT(featureSurfAction()));

	featureSiftAct = new QAction(tr("SIFT feature detector"), this);
	featureSiftAct->setEnabled(false);
	connect(featureSiftAct, SIGNAL(triggered()), this,
			SLOT(featureSiftAction()));

	matchingBFAct = new QAction(tr("Description matching with Brute Force"),
			this);
	matchingBFAct->setEnabled(false);
	connect(matchingBFAct, SIGNAL(triggered()), this, SLOT(matchingBFAction()));

	matchingFLANNAct = new QAction(tr("Description matching with FLANN"), this);
	matchingFLANNAct->setEnabled(false);
	connect(matchingFLANNAct, SIGNAL(triggered()), this,
			SLOT(matchingFLANNAction()));

	matchingHomographyAct = new QAction(tr("Description matching Homography"),
			this);
	matchingHomographyAct->setEnabled(false);
	connect(matchingHomographyAct, SIGNAL(triggered()), this,
			SLOT(matchingHomographyAction()));

	matchingTemplateAct = new QAction(tr("Matching template"), this);
	matchingTemplateAct->setEnabled(false);
	connect(matchingTemplateAct, SIGNAL(triggered()), this,
			SLOT(matchingTemplateAction()));

	correctMorphAct = new QAction(tr("Correct now"), this);
	correctMorphAct->setEnabled(false);
	connect(correctMorphAct, SIGNAL(triggered()), this,
			SLOT(correctMorphAction()));

	pyramidAct = new QAction(tr("Pyramid"), this);
	pyramidAct->setEnabled(false);
	connect(pyramidAct, SIGNAL(triggered()), this, SLOT(pyramidAction()));

	remapAct = new QAction(tr("Remapping"), this);
	remapAct->setEnabled(false);
	connect(remapAct, SIGNAL(triggered()), this, SLOT(remapAction()));

	affineAct = new QAction(tr("Affine"), this);
	affineAct->setEnabled(false);
	connect(affineAct, SIGNAL(triggered()), this, SLOT(affineAction()));

	histogramAct = new QAction(tr("Histogram"), this);
	histogramAct->setEnabled(false);
	connect(histogramAct, SIGNAL(triggered()), this, SLOT(histogramAction()));

	equalizationAct = new QAction(tr("Equalization"), this);
	equalizationAct->setEnabled(false);
	connect(equalizationAct, SIGNAL(triggered()), this,
			SLOT(equalizationAction()));

	backprojectionAct = new QAction(tr("Back projection"), this);
	backprojectionAct->setEnabled(false);
	connect(backprojectionAct, SIGNAL(triggered()), this,
			SLOT(backprojectionAction()));

	grayscaleMyImplAct = new QAction(tr("Color 2 grayscale"), this);
	grayscaleMyImplAct->setEnabled(false);
	connect(grayscaleMyImplAct, SIGNAL(triggered()), this,
			SLOT(grayscaleMyImplAction()));

	binaryMyImplAct = new QAction(tr("Binary threshold"), this);
	binaryMyImplAct->setEnabled(false);
	connect(binaryMyImplAct, SIGNAL(triggered()), this,
			SLOT(binaryMyImplAction()));

	robertsMyImplAct = new QAction(tr("Roberts"), this);
	robertsMyImplAct->setEnabled(false);
	connect(robertsMyImplAct, SIGNAL(triggered()), this,
			SLOT(robertsMyImplAction()));

	sobelMyImplAct = new QAction(tr("Sobel"), this);
	sobelMyImplAct->setEnabled(false);
	connect(sobelMyImplAct, SIGNAL(triggered()), this,
			SLOT(sobelMyImplAction()));

	hitmissMyImplAct = new QAction(tr("Hit and Miss transfrom"), this);
	hitmissMyImplAct->setEnabled(false);
	connect(hitmissMyImplAct, SIGNAL(triggered()), this,
			SLOT(hitmissMyImplAction()));

	otsuMyImplAct = new QAction(tr("Otsu-multilevel"), this);
	otsuMyImplAct->setEnabled(false);
	connect(otsuMyImplAct, SIGNAL(triggered()), this, SLOT(otsuMyImplAction()));

	growingMyImplAct = new QAction(tr("Region growing"), this);
	growingMyImplAct->setEnabled(false);
	connect(growingMyImplAct, SIGNAL(triggered()), this,
			SLOT(growingMyImplAction()));

	antMyImplAct = new QAction(tr("Social ant method"), this);
	antMyImplAct->setEnabled(false);
	connect(antMyImplAct, SIGNAL(triggered()), this, SLOT(antMyImplAction()));

	//add by LE Van Linh
	removeLinesAct = new QAction(tr("Remove Yellow grid"), this);
	removeLinesAct->setEnabled(false);
	removeLinesAct->setShortcut(tr("Ctrl+R"));
	connect(removeLinesAct, SIGNAL(triggered()), this,
			SLOT(removeYLinesAction()));

	landmarksDetection = new QAction(tr("Landmarks identifying"), this);
	landmarksDetection->setEnabled(false);
	landmarksDetection->setShortcut(tr("Ctrl+L"));
	connect(landmarksDetection, SIGNAL(triggered()), this,
			SLOT(getLandmarks()));

	edgeSegment = new QAction(tr("Edge segmentation"), this);
	edgeSegment->setEnabled(false);
	edgeSegment->setShortcut(tr("Ctrl+E"));
	connect(edgeSegment, SIGNAL(triggered()), this, SLOT(edgeSegmentation()));

	pwHistogram = new QAction(tr("Pairwise histogram"), this);
	pwHistogram->setEnabled(false);
	pwHistogram->setShortcut(tr("Ctrl+H"));
	connect(pwHistogram, SIGNAL(triggered()), this, SLOT(pairwiseHistogram()));

	pwhMatching = new QAction(tr("PGH matching - Bhattacharyya metric"), this);
	pwhMatching->setEnabled(false);
	pwhMatching->setShortcut(tr("Ctrl+B"));
	connect(pwhMatching, SIGNAL(triggered()), this,
			SLOT(pwBhattacharyyaMatching()));

	pwhChisquared = new QAction(tr("PGH matching - Chi-squared metric"), this);
	pwhChisquared->setEnabled(false);
	pwhChisquared->setShortcut(tr("Ctrl+C"));
	connect(pwhChisquared, SIGNAL(triggered()), this,
			SLOT(pwChiSquaredMatching()));

	pwhIntersection = new QAction(tr("PGH matching - Intersection metric"),
			this);
	pwhIntersection->setEnabled(false);
	pwhIntersection->setShortcut(tr("Ctrl+I"));
	connect(pwhIntersection, SIGNAL(triggered()), this,
			SLOT(pwIntersectionMatching()));

	phTransform = new QAction(tr("Probabilistic Hough Transform"), this);
	phTransform->setEnabled(false);
	phTransform->setShortcut(tr("Ctrl+T"));
	connect(phTransform, SIGNAL(triggered()), this, SLOT(pHoughTransform()));

	cCorrelation = new QAction(tr("Cross Correlation"), this);
	cCorrelation->setEnabled(false);
	connect(cCorrelation, SIGNAL(triggered()), this, SLOT(crossCorrelation()));

	ccMeasureDistance = new QAction(tr("By Cross-correlation"), this);
	ccMeasureDistance->setEnabled(false);
	connect(ccMeasureDistance, SIGNAL(triggered()), this,
			SLOT(crossCorrelationDistance()));

	tmMeasureDistance = new QAction(tr("By Edge extraction (1,1)"), this);
	tmMeasureDistance->setEnabled(false);
	connect(tmMeasureDistance, SIGNAL(triggered()), this,
			SLOT(tplMatchingDistance()));

	segmentationDir = new QAction(tr("Edge segmentation"), this);
	segmentationDir->setEnabled(false);
	connect(segmentationDir, SIGNAL(triggered()), this,
			SLOT(edgeSegmentDirectory()));

	bhattcharyyaDir = new QAction(tr("Compute Bhattacharyya distance"), this);
	bhattcharyyaDir->setEnabled(false);
	connect(bhattcharyyaDir, SIGNAL(triggered()), this,
			SLOT(bhattacharyyaDistanceDirectory()));

	proHoughTransformDir = new QAction(tr("Probabilistic Hough Transform"),
			this);
	proHoughTransformDir->setEnabled(false);
	connect(proHoughTransformDir, SIGNAL(triggered()), this,
			SLOT(phtOnDirectory()));

	lmExtractionDir = new QAction(tr("Landmarks identifying"), this);
	lmExtractionDir->setEnabled(false);
	connect(lmExtractionDir, SIGNAL(triggered()), this,
			SLOT(estlmOnDirectory()));

	sizeLandmarksDir = new QAction(tr("By Edge extraction (1,n)"), this);
	sizeLandmarksDir->setEnabled(false);
	connect(sizeLandmarksDir, SIGNAL(triggered()), this,
			SLOT(computeSizeOnDirectory()));
	loadOrgLandmarks = new QAction(tr("Load original landmarks"), this);
	loadOrgLandmarks->setEnabled(false);
	connect(loadOrgLandmarks, SIGNAL(triggered()), this,
			SLOT(loadOriginalLandmarks()));
	//end
}

void ImageViewer::createMenus() {
	fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAct);
	fileMenu->addAction(saveAsAct);
	fileMenu->addAction(printAct);
	fileMenu->addSeparator();
	fileMenu->addAction(closeAct);
	fileMenu->addAction(exitAct);

	viewMenu = new QMenu(tr("&View"), this);
	viewMenu->addAction(zoomInAct);
	viewMenu->addAction(zoomOutAct);
	viewMenu->addAction(normalSizeAct);
	viewMenu->addSeparator();
	viewMenu->addAction(fitToWindowAct);

	helpMenu = new QMenu(tr("&Help"), this);
	helpMenu->addAction(aboutAct);

	//pluginsMenu = new QMenu(tr("&Algorithms"), this);
	//opencvMenu = pluginsMenu->addMenu(tr("OpenCV"));

	segment = new QMenu(tr("&Segmentation"), this);
	segment->addAction(edgeSegment);
	opencvMenu = segment->addMenu(tr("OpenCV"));

	QMenu* mnuSmooth = opencvMenu->addMenu(tr("Smooth"));
	mnuSmooth->addAction(normalBlurAct);
	mnuSmooth->addAction(gaussianBlurAct);
	mnuSmooth->addAction(medianBlurAct);
	mnuSmooth->addAction(bilateralFilterAct);
	mnuSmooth->addAction(convolutionFilterAct);

	QMenu* mnuMorph = opencvMenu->addMenu(tr("Morphology"));
	mnuMorph->addAction(erosionAct);
	mnuMorph->addAction(dilationAct);
	mnuMorph->addAction(morphoExtAct);

	QMenu* mnuSegm = opencvMenu->addMenu(tr("Segmentation"));
	mnuSegm->addAction(thresholdAct);
	mnuSegm->addAction(sobelAct);
	mnuSegm->addAction(scharrAct);
	mnuSegm->addAction(laplaceAct);
	mnuSegm->addAction(cannyAct);
	mnuSegm->addSeparator();
	mnuSegm->addAction(houghLineAct);
	mnuSegm->addAction(houghCircleAct);
	mnuSegm->addAction(findContourAct);
	mnuSegm->addAction(convexHullAct);
	mnuSegm->addAction(imgMomentsAct);
	mnuSegm->addAction(bdBoxCircleAct);
	mnuSegm->addAction(bdRotEclipseAct);

	QMenu* mnuFeaDetector = opencvMenu->addMenu(tr("Point, feature detector"));

	mnuFeaDetector->addAction(harrisAct);
	mnuFeaDetector->addAction(shiTomasiAct);
	mnuFeaDetector->addAction(cornerDetectAct);
	mnuFeaDetector->addSeparator();
	mnuFeaDetector->addAction(featureSurfAct);
	mnuFeaDetector->addAction(featureSiftAct);
	mnuFeaDetector->addSeparator();
	mnuFeaDetector->addAction(matchingBFAct);
	mnuFeaDetector->addAction(matchingFLANNAct);
	mnuFeaDetector->addAction(matchingHomographyAct);
	mnuFeaDetector->addAction(matchingTemplateAct);
	mnuFeaDetector->addAction(correctMorphAct);

	QMenu* mnuTrans = opencvMenu->addMenu(tr("Transform and histogram"));
	mnuTrans->addAction(pyramidAct);
	mnuTrans->addAction(remapAct);
	mnuTrans->addAction(affineAct);
	mnuTrans->addSeparator();
	mnuTrans->addAction(histogramAct);
	mnuTrans->addAction(equalizationAct);
	mnuTrans->addAction(backprojectionAct);

	/*myimplMenu = pluginsMenu->addMenu(tr("Others"));
	 myimplMenu->addAction(grayscaleMyImplAct);
	 myimplMenu->addAction(binaryMyImplAct);
	 myimplMenu->addSeparator();
	 myimplMenu->addAction(robertsMyImplAct);
	 myimplMenu->addAction(sobelMyImplAct);
	 myimplMenu->addSeparator();
	 myimplMenu->addAction(otsuMyImplAct);
	 myimplMenu->addAction(growingMyImplAct);
	 myimplMenu->addSeparator();
	 myimplMenu->addAction(hitmissMyImplAct);
	 myimplMenu->addAction(antMyImplAct);*/

	//add by Linh
	landmarks = new QMenu(tr("Landmarks"), this);

	QMenu *estimation = landmarks->addMenu(tr("Estimation"));

	QMenu* mnuArticle = estimation->addMenu(tr("By Edge extraction (1,1)"));
	mnuArticle->addAction(phTransform);
	mnuArticle->addAction(landmarksDetection);

	QMenu* mnuOnDirectory = estimation->addMenu(tr("By Edge extraction (1,n)"));
	//mnuOnDirectory->addAction(segmentationDir);
	//mnuOnDirectory->addAction(bhattcharyyaDir);
	mnuOnDirectory->addAction(proHoughTransformDir);
	mnuOnDirectory->addAction(lmExtractionDir);

	estimation->addAction(cCorrelation);

	QMenu *analysis = landmarks->addMenu(tr("Analysis"));
	analysis->addAction(tmMeasureDistance);
	analysis->addAction(sizeLandmarksDir);
	analysis->addAction(ccMeasureDistance);

	utilities = new QMenu(tr("Utilities"), this);
	utilities->addAction(grayscaleMyImplAct);
	utilities->addAction(binaryMyImplAct);
	utilities->addSeparator();
	utilities->addAction(removeLinesAct);
	utilities->addAction(pwHistogram);
	utilities->addAction(loadOrgLandmarks);

	QMenu* mnuHistMatch = utilities->addMenu(tr("PGH matching"));
	mnuHistMatch->addAction(pwhMatching);
	mnuHistMatch->addAction(pwhChisquared);
	mnuHistMatch->addAction(pwhIntersection);

	//end

	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(viewMenu);
	menuBar()->addMenu(segment);
	menuBar()->addMenu(landmarks);
	menuBar()->addMenu(utilities);
	//menuBar()->addMenu(pluginsMenu);
	menuBar()->addMenu(helpMenu);
}

void ImageViewer::createToolBars() {
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(openAct);
	fileToolBar->addAction(saveAct);

	viewToolBar = addToolBar(tr("View"));
	viewToolBar->addAction(zoomInAct);
	viewToolBar->addAction(zoomOutAct);
}

void ImageViewer::updateActions() {
	zoomInAct->setEnabled(!fitToWindowAct->isChecked());
	zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
	normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void ImageViewer::scaleImage(double factor) {
	Q_ASSERT(imageLabel->pixmap());
	scaleFactor *= factor;
	imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

	adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
	adjustScrollBar(scrollArea->verticalScrollBar(), factor);

	zoomInAct->setEnabled(scaleFactor < 3.0);
	zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor) {
	scrollBar->setValue(
			int(
					factor * scrollBar->value()
							+ ((factor - 1) * scrollBar->pageStep() / 2)));
}

void ImageViewer::closeEvent(QCloseEvent *event) {
	if (!fileName.isEmpty())
		return;
	QMessageBox::StandardButton ret;
	ret = QMessageBox::warning(this, tr("Image Viewer"),
			tr("Image is not saved. \nDo you want to close this form?"),
			QMessageBox::Ok | QMessageBox::Cancel);
	if (ret == QMessageBox::Ok) {
		writeSettings();
		event->accept();
	} else if (ret == QMessageBox::Cancel) {
		event->ignore();
	}
}

void ImageViewer::contextMenuEvent(QContextMenuEvent *event) // this is a slot
		{
	QMenu menu(this);
	//menu.addAction(histogramAct);
	if (parameterPanel != NULL)
		menu.addAction(parameterAct);
	menu.exec(event->globalPos());
}

void ImageViewer::createStatusBar() {
	statusBar()->showMessage(tr("Ready"));
}

void ImageViewer::readSettings() {
	QSettings settings;
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	//QSize size = settings.value("size", QSize(400, 400)).toSize();
	move(pos);
	//resize(size);
}

void ImageViewer::writeSettings() {
	QSettings settings;
	settings.setValue("pos", pos());
	//settings.setValue("size", size());
}

bool ImageViewer::save() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save image"), ".",
			tr("Image Files (*.jpg)"));
	if (fileName.isEmpty()) {
		return false;
	}
	QApplication::setOverrideCursor(Qt::WaitCursor);
	qImage.save(fileName, "JPEG");

	QApplication::restoreOverrideCursor();

	this->fileName = fileName;
	setWindowTitle(tr("Image Viewer - ") + fileName);

	saveAct->setEnabled(false);
	saveAsAct->setEnabled(true);

	statusBar()->showMessage(tr("File saved"), 2000);
	return true;
}

bool ImageViewer::saveAs() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save image"), ".",
			tr("Image Files (*.jpg)"));
	if (fileName.isEmpty()) {
		return false;
	}
	QApplication::setOverrideCursor(Qt::WaitCursor);
	cv::imwrite(fileName.toStdString(), ImageConvert::QImageToCvMat(qImage));
	QApplication::restoreOverrideCursor();

	statusBar()->showMessage(tr("File saved"), 2000);
	return true;
}

void ImageViewer::parameterAction() {
	parameterPanel->show();
}

void ImageViewer::addParameterPanel(QDialog *pnl, int x, int y) {
	parameterPanel = pnl;
	move(x, y);
	show();
}

void ImageViewer::normalBlurAction() {
	qDebug() << "ImageViewer::normalBlurAction ";
	cv::Mat dstMat;
	int size = ui::NormalBlurPanel::DF_KERNEL_SIZE * 2 + 1;
	cv::blur(matImage, dstMat, cv::Size(size, size), cv::Point(-1, -1));

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(dstMat),
			"normal blur result");
	other->addParameterPanel(new ui::NormalBlurPanel(other), x() + 40,
			y() + 40);
}

void ImageViewer::normalBlurAction(int kernelSize) {
	cv::Mat dstMat;
	cv::blur(matImage, dstMat, cv::Size(kernelSize, kernelSize),
			cv::Point(-1, -1));

	qImage = ImageConvert::cvMatToQImage(dstMat);
	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	statusBar()->showMessage(tr("Kernel size: ") + QString::number(kernelSize));
}

void ImageViewer::gaussianBlurAction() {
	qDebug() << "ImageViewer::gaussianBlurAction ";
	cv::Mat dstMat;
	int size = ui::GaussianBlurPanel::DF_KERNEL_SIZE * 2 + 1;
	cv::GaussianBlur(matImage, dstMat, cv::Size(size, size), 0, 0);

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(dstMat),
			"gaussian blur result");
	other->addParameterPanel(new ui::GaussianBlurPanel(other), x() + 40,
			y() + 40);
}
void ImageViewer::gaussianBlurAction(int kernelSize) {
	cv::Mat dstMat;
	cv::GaussianBlur(matImage, dstMat, cv::Size(kernelSize, kernelSize), 0, 0);

	qImage = ImageConvert::cvMatToQImage(dstMat);
	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	statusBar()->showMessage(tr("Kernel size: ") + QString::number(kernelSize));
}

void ImageViewer::medianBlurAction() {
	qDebug() << "ImageViewer::medianBlurAction ";
	cv::Mat dstMat;
	int size = ui::MedianBlurPanel::DF_KERNEL_SIZE * 2 + 1;
	cv::medianBlur(matImage, dstMat, size);

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(dstMat),
			"median blur result");
	other->addParameterPanel(new ui::MedianBlurPanel(other), x() + 40,
			y() + 40);

}
void ImageViewer::medianBlurAction(int kernelSize) {
	cv::Mat dstMat;
	cv::medianBlur(matImage, dstMat, kernelSize);

	qImage = ImageConvert::cvMatToQImage(dstMat);
	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	statusBar()->showMessage(tr("Kernel size: ") + QString::number(kernelSize));
}

void ImageViewer::bilateralFilterAction() {
	qDebug() << "ImageViewer::medianBlurAction ";
	cv::Mat dstMat;
	int sigma = ui::BilateralFilterPanel::DF_SIGMA_COLOR * 2 + 1;
	cv::bilateralFilter(matImage, dstMat, sigma, sigma * 2, sigma / 2);

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(dstMat),
			"bilateral filter result");
	other->addParameterPanel(new ui::BilateralFilterPanel(other), x() + 40,
			y() + 40);
}

void ImageViewer::bilateralFilterAction(int sigmaColor) {
	cv::Mat dstMat;
	cv::bilateralFilter(matImage, dstMat, sigmaColor, sigmaColor * 2,
			sigmaColor / 2);

	qImage = ImageConvert::cvMatToQImage(dstMat);
	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	statusBar()->showMessage(tr("Sigma color: ") + QString::number(sigmaColor));
}

void ImageViewer::convolutionFilterAction() {
	qDebug() << "ImageViewer::convolutionFilterAction ";

	cv::Point anchor(-1, -1);
	double delta = 0;
	int ddepth = -1;
	int kernel_size = ui::ConvolutionFilterPanel::DF_KERNEL_SIZE * 2 + 1;

	cv::Mat kernel, dst;
	kernel = cv::Mat::ones(kernel_size, kernel_size, CV_32F)
			/ (float) (kernel_size * kernel_size);

	/// Apply filter
	cv::filter2D(matImage, dst, ddepth, kernel, anchor, delta,
			cv::BORDER_DEFAULT);

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(dst),
			"convolution filter result");
	other->addParameterPanel(new ui::ConvolutionFilterPanel(other), x() + 40,
			y() + 40);

}

void ImageViewer::convolutionFilterAction(int kernelSize) {
	cv::Point anchor(-1, -1);
	double delta = 0;
	int ddepth = -1;

	cv::Mat kernel, dst;
	kernel = cv::Mat::ones(kernelSize, kernelSize, CV_32F)
			/ (float) (kernelSize * kernelSize);

	/// Apply filter
	cv::filter2D(matImage, dst, ddepth, kernel, anchor, delta,
			cv::BORDER_DEFAULT);

	qImage = ImageConvert::cvMatToQImage(dst);
	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	statusBar()->showMessage(tr("Sigma color: ") + QString::number(kernelSize));
}

void ImageViewer::erosionAction() {
	qDebug() << "ImageViewer::erosionAction ";
	cv::Mat dstMat;
	int erosion_size = ui::ErosionPanel::DF_KERNEL_SIZE * 2 + 1;
	int erosion_type = ui::ErosionPanel::DF_TYPE;

	cv::Mat element = cv::getStructuringElement(erosion_type,
			cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
			cv::Point(erosion_size, erosion_size));

	/// Apply the erosion operation
	cv::erode(matImage, dstMat, element);

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(dstMat),
			"erosion filter result");
	other->addParameterPanel(new ui::ErosionPanel(other), x() + 40, y() + 40);

}
void ImageViewer::erosionAction(int kernelSize, int typeEle) {
	cv::Mat dstMat;
	cv::Mat element = cv::getStructuringElement(typeEle,
			cv::Size(2 * kernelSize + 1, 2 * kernelSize + 1),
			cv::Point(kernelSize, kernelSize));

	/// Apply the erosion operation
	cv::erode(matImage, dstMat, element);

	qImage = ImageConvert::cvMatToQImage(dstMat);
	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	statusBar()->showMessage(
			tr("Kernel size: ") + QString::number(kernelSize)
					+ tr("| Element type: ") + QString::number(typeEle));
}

void ImageViewer::dilationAction() {
	qDebug() << "ImageViewer::dilationAction ";
	cv::Mat dstMat;
	int dilation_size = ui::DilationPanel::DF_KERNEL_SIZE * 2 + 1;
	int dilation_type = ui::DilationPanel::DF_TYPE;

	cv::Mat element = cv::getStructuringElement(dilation_type,
			cv::Size(2 * dilation_size + 1, 2 * dilation_size + 1),
			cv::Point(dilation_size, dilation_size));

	/// Apply the erosion operation
	cv::dilate(matImage, dstMat, element);

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(dstMat),
			"dilation filter result");
	other->addParameterPanel(new ui::DilationPanel(other), x() + 40, y() + 40);
}
void ImageViewer::dilationAction(int kernelSize, int typeEle) {
	cv::Mat dstMat;
	cv::Mat element = cv::getStructuringElement(typeEle,
			cv::Size(2 * kernelSize + 1, 2 * kernelSize + 1),
			cv::Point(kernelSize, kernelSize));

	/// Apply the erosion operation
	cv::dilate(matImage, dstMat, element);

	qImage = ImageConvert::cvMatToQImage(dstMat);
	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	statusBar()->showMessage(
			tr("Kernel size: ") + QString::number(kernelSize)
					+ tr("| Element type: ") + QString::number(typeEle));
}

void ImageViewer::morphoExtAction() {
	qDebug() << "ImageViewer::morphoExtAction ";
	cv::Mat dstMat;
	int morph_size = ui::MorphoExtPanel::DF_KERNEL_SIZE * 2 + 1;
	int morph_operator = ui::MorphoExtPanel::DF_TYPE;

	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
			cv::Size(2 * morph_size + 1, 2 * morph_size + 1),
			cv::Point(morph_size, morph_size));

	/// Apply the specified morphology operation
	cv::morphologyEx(matImage, dstMat, morph_operator, element);

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(dstMat),
			"morphology ext. result");
	other->addParameterPanel(new ui::MorphoExtPanel(other), x() + 40, y() + 40);
}
void ImageViewer::morphoExtAction(int kernelSize, int type) {
	cv::Mat dstMat;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
			cv::Size(2 * kernelSize + 1, 2 * kernelSize + 1),
			cv::Point(kernelSize, kernelSize));

	/// Apply the specified morphology operation
	cv::morphologyEx(matImage, dstMat, type, element);

	qImage = ImageConvert::cvMatToQImage(dstMat);
	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	statusBar()->showMessage(
			tr("Kernel size: ") + QString::number(kernelSize)
					+ tr("| Operator type: ") + QString::number(type));
}

void ImageViewer::thresholdAction() {
	qDebug() << "ImageViewer::thresholdAction ";

	int value = ui::ThresholdingPanel::DF_VALUE;
	int type = ui::ThresholdingPanel::DF_TYPE;

	cv::Mat src_gray, dst;
	cv::cvtColor(matImage, src_gray, CV_RGB2GRAY);
	cv::threshold(src_gray, dst, value, 255, type);

	ImageViewer* other = new ImageViewer;
	other->loadImage(src_gray, ImageConvert::cvMatToQImage(dst),
			"thresholding result");
	other->addParameterPanel(new ui::ThresholdingPanel(other), x() + 40,
			y() + 40);
}

void ImageViewer::thresholdAction(int value, int type) {

	cv::Mat dstMat;
	cv::threshold(matImage, dstMat, value, 255, type);

	qImage = ImageConvert::cvMatToQImage(dstMat);
	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	statusBar()->showMessage(
			tr("Value: ") + QString::number(value) + tr("| Type: ")
					+ QString::number(type));
}

void ImageViewer::sobelAction() {
	qDebug() << "ImageViewer::sobelAction ";

	int kernelSize = ui::SobelPanel::DF_KERNEL_SIZE * 2 + 1;
	int scale = ui::SobelPanel::DF_SCALE;
	int delta = 0;
	int ddepth = CV_16S;

	cv::GaussianBlur(matImage, matImage, cv::Size(3, 3), 0, 0,
			cv::BORDER_DEFAULT);

	cv::Mat src_gray;
	cv::cvtColor(matImage, src_gray, CV_RGB2GRAY);

	cv::Mat grad_x, grad_y, grad;
	cv::Mat abs_grad_x, abs_grad_y;

	/// Gradient X
	cv::Sobel(src_gray, grad_x, ddepth, 1, 0, kernelSize, scale, delta,
			cv::BORDER_DEFAULT);
	cv::convertScaleAbs(grad_x, abs_grad_x);

	/// Gradient Y
	cv::Sobel(src_gray, grad_y, ddepth, 0, 1, kernelSize, scale, delta,
			cv::BORDER_DEFAULT);
	cv::convertScaleAbs(grad_y, abs_grad_y);

	/// Total Gradient (approximate)
	cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	ImageViewer* other = new ImageViewer;
	other->loadImage(src_gray, ImageConvert::cvMatToQImage(grad),
			"sobel result");
	other->addParameterPanel(new ui::SobelPanel(other), x() + 40, y() + 40);
}
void ImageViewer::sobelAction(int kernelSize, int scale) {
	int delta = 0;
	int ddepth = CV_16S;

	cv::Mat grad_x, grad_y, grad;
	cv::Mat abs_grad_x, abs_grad_y;

	/// Gradient X
	cv::Sobel(matImage, grad_x, ddepth, 1, 0, kernelSize, scale, delta,
			cv::BORDER_DEFAULT);
	cv::convertScaleAbs(grad_x, abs_grad_x);

	/// Gradient Y
	cv::Sobel(matImage, grad_y, ddepth, 0, 1, kernelSize, scale, delta,
			cv::BORDER_DEFAULT);
	cv::convertScaleAbs(grad_y, abs_grad_y);

	/// Total Gradient (approximate)
	cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	qImage = ImageConvert::cvMatToQImage(grad);
	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	statusBar()->showMessage(
			tr("Kernel size: ") + QString::number(kernelSize) + tr("| Scale: ")
					+ QString::number(scale));

}
void ImageViewer::scharrAction() {
	qDebug() << "ImageViewer::scharrAction ";

	cv::GaussianBlur(matImage, matImage, cv::Size(3, 3), 0, 0,
			cv::BORDER_DEFAULT);

	cv::Mat src_gray;
	cv::cvtColor(matImage, src_gray, CV_RGB2GRAY);

	int scale = ui::ScharrPanel::DF_SCALE;
	int delta = 0;
	int ddepth = CV_16S;

	cv::Mat grad_x, grad_y, grad;
	cv::Mat abs_grad_x, abs_grad_y;

	/// Gradient X
	cv::Scharr(src_gray, grad_x, ddepth, 1, 0, scale, delta,
			cv::BORDER_DEFAULT);
	cv::convertScaleAbs(grad_x, abs_grad_x);

	/// Gradient Y
	cv::Scharr(src_gray, grad_y, ddepth, 0, 1, scale, delta,
			cv::BORDER_DEFAULT);
	cv::convertScaleAbs(grad_y, abs_grad_y);

	/// Total Gradient (approximate)
	cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	/// Total Gradient (approximate)
	cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	ImageViewer* other = new ImageViewer;
	other->loadImage(src_gray, ImageConvert::cvMatToQImage(grad),
			"scharr result");
	other->addParameterPanel(new ui::ScharrPanel(other), x() + 40, y() + 40);
}
void ImageViewer::scharrAction(int scale) {
	int delta = 0;
	int ddepth = CV_16S;

	cv::Mat grad_x, grad_y, grad;
	cv::Mat abs_grad_x, abs_grad_y;

	/// Gradient X
	cv::Scharr(matImage, grad_x, ddepth, 1, 0, scale, delta,
			cv::BORDER_DEFAULT);
	cv::convertScaleAbs(grad_x, abs_grad_x);

	/// Gradient Y
	cv::Scharr(matImage, grad_y, ddepth, 0, 1, scale, delta,
			cv::BORDER_DEFAULT);
	cv::convertScaleAbs(grad_y, abs_grad_y);

	/// Total Gradient (approximate)
	cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	/// Total Gradient (approximate)
	cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	qImage = ImageConvert::cvMatToQImage(grad);
	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	statusBar()->showMessage(tr("| Scale: ") + QString::number(scale));
}

void ImageViewer::laplaceAction() {

	qDebug() << "ImageViewer::laplaceAction ";

	cv::GaussianBlur(matImage, matImage, cv::Size(3, 3), 0, 0,
			cv::BORDER_DEFAULT);

	cv::Mat src_gray;
	cv::cvtColor(matImage, src_gray, CV_RGB2GRAY);

	cv::Mat abs_dst, dst;

	int kernel_size = ui::LaplacePanel::DF_KERNEL_SIZE * 2 + 1;
	int scale = ui::LaplacePanel::DF_SCALE;
	;

	int delta = 0;
	int ddepth = CV_16S;

	cv::Laplacian(src_gray, dst, ddepth, kernel_size, scale, delta,
			cv::BORDER_DEFAULT);
	cv::convertScaleAbs(dst, abs_dst);

	ImageViewer* other = new ImageViewer;
	other->loadImage(src_gray, ImageConvert::cvMatToQImage(abs_dst),
			"laplace result");
	other->addParameterPanel(new ui::LaplacePanel(other), x() + 40, y() + 40);

}
void ImageViewer::laplaceAction(int kernelSize, int scale) {

	cv::Mat abs_dst, dst;

	int delta = 0;
	int ddepth = CV_16S;

	cv::Laplacian(matImage, dst, ddepth, kernelSize, scale, delta,
			cv::BORDER_DEFAULT);
	cv::convertScaleAbs(dst, abs_dst);

	qImage = ImageConvert::cvMatToQImage(abs_dst);
	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	statusBar()->showMessage(
			tr("Kernel size: ") + QString::number(kernelSize) + tr("| Scale: ")
					+ QString::number(scale));

}
void ImageViewer::cannyAction() {
	qDebug() << "ImageViewer::cannyAction ";

	cv::Mat src_gray;
	cv::cvtColor(matImage, src_gray, CV_RGB2GRAY);

	cv::Mat dst, detected_edges;

	cv::blur(src_gray, detected_edges, cv::Size(3, 3));

	int lowThreshold = ui::CannyPanel::DF_LOW;
	int ratio = ui::CannyPanel::DF_RATIO;
	int kernel_size = ui::CannyPanel::DF_KERNEL_SIZE * 2 + 1;

	/// Canny detector
	cv::Canny(detected_edges, detected_edges, lowThreshold,
			lowThreshold * ratio, kernel_size);

	/// Using Canny's output as a mask, we display our result
	dst = cv::Scalar::all(0);

	matImage.copyTo(dst, detected_edges);

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(dst),
			"canny result");
	other->addParameterPanel(new ui::CannyPanel(other), x() + 40, y() + 40);
}
void ImageViewer::cannyAction(int kernelSize, int low, int ratio) {

	cv::Mat src_gray;
	cv::cvtColor(matImage, src_gray, CV_RGB2GRAY);

	cv::Mat dst, detected_edges;

	cv::blur(src_gray, detected_edges, cv::Size(3, 3));

	/// Canny detector
	cv::Canny(detected_edges, detected_edges, low, low * ratio, kernelSize);

	/// Using Canny's output as a mask, we display our result
	dst = cv::Scalar::all(0);

	matImage.copyTo(dst, detected_edges);

	qImage = ImageConvert::cvMatToQImage(dst);
	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	statusBar()->showMessage(
			tr("Kernel size: ") + QString::number(kernelSize)
					+ tr("| Low threshold: ") + QString::number(low)
					+ tr("| Ratio: ") + QString::number(ratio));

}

void ImageViewer::houghLineAction() {
	qDebug() << "ImageViewer::houghLineAction ";

	cv::Mat dst, cdst;

	dst.create(matImage.size(), matImage.type());

	cv::Canny(matImage, dst, 50, 200, 3);
	cv::cvtColor(dst, cdst, CV_GRAY2BGR);

	vector < cv::Vec2f > lines;
	cv::HoughLines(dst, lines, 1, CV_PI / 180, 100, 0, 0);

	for (size_t i = 0; i < lines.size(); i++) {
		float rho = lines[i][0], theta = lines[i][1];
		cv::Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		cv::line(cdst, pt1, pt2, cv::Scalar(0, 0, 255), 3, CV_AA);
	}

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(cdst),
			"hough line result");
	other->move(x() + 40, y() + 40);
	other->show();

}
void ImageViewer::houghCircleAction() {
	qDebug() << "ImageViewer::houghCircleAction ";

	cv::Mat src_gray;

	cv::cvtColor(matImage, src_gray, CV_BGR2GRAY);

	/// Reduce the noise so we avoid false circle detection
	cv::GaussianBlur(src_gray, src_gray, cv::Size(9, 9), 2, 2);

	vector < cv::Vec3f > circles;

	/// Apply the Hough Transform to find the circles
	cv::HoughCircles(src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows / 8,
			200, 100, 0, 0);

	qDebug() << "number circle: " << circles.size();

	/// Draw the circles detected
	for (size_t i = 0; i < circles.size(); i++) {
		cv::Point center(cvRound (circles[i][0]), cvRound (circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// circle center
		cv::circle(matImage, center, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
		// circle outline
		cv::circle(matImage, center, radius, cv::Scalar(0, 0, 255), 3, 8, 0);
	}

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(matImage),
			"hough circle result");
	other->move(x() + 40, y() + 40);
	other->show();
}

void ImageViewer::findContourAction() {
	qDebug() << "ImageViewer::findContourAction ";

	cv::Mat dst, src_gray;
	int thresh = 100;
	cv::RNG rng(12345);

	cv::cvtColor(matImage, src_gray, CV_BGR2GRAY);
	cv::blur(src_gray, src_gray, cv::Size(3, 3));

	cv::Mat canny_output;
	vector < vector<cv::Point> > contours;
	vector < cv::Vec4i > hierarchy;

	/// Detect edges using canny
	cv::Canny(src_gray, canny_output, thresh, thresh * 2, 3);
	/// Find contours
	cv::findContours(canny_output, contours, hierarchy, CV_RETR_TREE,
			CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	/// Draw contours
	cv::Mat drawing = cv::Mat::zeros(canny_output.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++) {
		cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));
		cv::drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0,
				cv::Point());
	}

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(drawing),
			"finding contour result");
	other->move(x() + 40, y() + 40);
	other->show();

}

void ImageViewer::convexHullAction() {
	qDebug() << "ImageViewer::convexHullAct ";

	cv::Mat src_gray;
	int thresh = 100;
	cv::RNG rng(12345);

	cv::cvtColor(matImage, src_gray, CV_BGR2GRAY);
	cv::blur(src_gray, src_gray, cv::Size(3, 3));

	cv::Mat src_copy = matImage.clone();
	cv::Mat threshold_output;
	vector < vector<cv::Point> > contours;
	vector < cv::Vec4i > hierarchy;

	/// Detect edges using Threshold
	cv::threshold(src_gray, threshold_output, thresh, 255, cv::THRESH_BINARY);

	/// Find contours
	cv::findContours(threshold_output, contours, hierarchy, CV_RETR_TREE,
			CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	/// Find the convex hull object for each contour
	vector < vector<cv::Point> > hull(contours.size());
	for (size_t i = 0; i < contours.size(); i++) {
		cv::convexHull(cv::Mat(contours[i]), hull[i], false);
	}

	/// Draw contours + hull results
	cv::Mat drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++) {
		cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));
		cv::drawContours(drawing, contours, i, color, 1, 8, vector<cv::Vec4i>(),
				0, cv::Point());
		cv::drawContours(drawing, hull, i, color, 1, 8, vector<cv::Vec4i>(), 0,
				cv::Point());
	}

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(drawing),
			"convex hull result");
	other->move(x() + 40, y() + 40);
	other->show();

}

void ImageViewer::bdBoxCircleAction() {
	qDebug() << "ImageViewer::bdBoxCircleAction ";

	cv::Mat src_gray;
	int thresh = 100;
	cv::RNG rng(12345);

	cv::cvtColor(matImage, src_gray, CV_BGR2GRAY);
	cv::blur(src_gray, src_gray, cv::Size(3, 3));

	cv::Mat threshold_output;
	vector < vector<cv::Point> > contours;
	vector < cv::Vec4i > hierarchy;

	/// Detect edges using Threshold
	cv::threshold(src_gray, threshold_output, thresh, 255, cv::THRESH_BINARY);
	/// Find contours
	cv::findContours(threshold_output, contours, hierarchy, CV_RETR_TREE,
			CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	/// Approximate contours to polygons + get bounding rects and circles
	vector < vector<cv::Point> > contours_poly(contours.size());
	vector < cv::Rect > boundRect(contours.size());
	vector < cv::Point2f > center(contours.size());
	vector<float> radius(contours.size());

	for (size_t i = 0; i < contours.size(); i++) {
		cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = cv::boundingRect(cv::Mat(contours_poly[i]));
		cv::minEnclosingCircle((cv::Mat) contours_poly[i], center[i],
				radius[i]);
	}

	/// Draw polygonal contour + bonding rects + circles
	cv::Mat drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++) {
		cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));
		cv::drawContours(drawing, contours_poly, i, color, 1, 8,
				vector<cv::Vec4i>(), 0, cv::Point());
		cv::rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2,
				8, 0);
		cv::circle(drawing, center[i], (int) radius[i], color, 2, 8, 0);
	}

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(drawing),
			"bounding and circle result");
	other->move(x() + 40, y() + 40);
	other->show();

}

void ImageViewer::bdRotEclipseAction() {
	qDebug() << "ImageViewer::bdRotEclipseAction ";

	cv::Mat src_gray;
	int thresh = 100;
	cv::RNG rng(12345);

	cv::cvtColor(matImage, src_gray, CV_BGR2GRAY);
	cv::blur(src_gray, src_gray, cv::Size(3, 3));

	cv::Mat threshold_output;
	vector < vector<cv::Point> > contours;
	vector < cv::Vec4i > hierarchy;

	/// Detect edges using Threshold
	cv::threshold(src_gray, threshold_output, thresh, 255, cv::THRESH_BINARY);
	/// Find contours
	cv::findContours(threshold_output, contours, hierarchy, CV_RETR_TREE,
			CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	/// Find the rotated rectangles and ellipses for each contour
	vector < cv::RotatedRect > minRect(contours.size());
	vector < cv::RotatedRect > minEllipse(contours.size());

	for (size_t i = 0; i < contours.size(); i++) {
		minRect[i] = cv::minAreaRect(cv::Mat(contours[i]));
//		error
//		if (contours[i].size() > 5)
//			minEllipse[i] = cv::fitEllipse(cv::Mat(contours[i]));
	}

	/// Draw contours + rotated rects + ellipses
	cv::Mat drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++) {
		cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));
		// contour
		cv::drawContours(drawing, contours, i, color, 1, 8, vector<cv::Vec4i>(),
				0, cv::Point());
		// ellipse
		cv::ellipse(drawing, minEllipse[i], color, 2, 8);
		// rotated rectangle
		cv::Point2f rect_points[4];
		minRect[i].points(rect_points);
		for (int j = 0; j < 4; j++)
			cv::line(drawing, rect_points[j], rect_points[(j + 1) % 4], color,
					1, 8);
	}

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(drawing),
			"bounding rotate and eclipse result");
	other->move(x() + 40, y() + 40);
	other->show();

}

void ImageViewer::imgMomentsAction() {
	qDebug() << "ImageViewer::imgMomentsAction ";

	cv::Mat src_gray;
	int thresh = 100;
	cv::RNG rng(12345);

	cv::cvtColor(matImage, src_gray, CV_BGR2GRAY);
	cv::blur(src_gray, src_gray, cv::Size(3, 3));

	cv::Mat canny_output;
	vector < vector<cv::Point> > contours;
	vector < cv::Vec4i > hierarchy;

	/// Detect edges using canny
	cv::Canny(src_gray, canny_output, thresh, thresh * 2, 3);
	/// Find contours
	cv::findContours(canny_output, contours, hierarchy, CV_RETR_TREE,
			CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	/// Get the moments
	vector < cv::Moments > mu(contours.size());
	for (size_t i = 0; i < contours.size(); i++) {
		mu[i] = cv::moments(contours[i], false);
	}

	///  Get the mass centers:
	vector < cv::Point2f > mc(contours.size());
	for (size_t i = 0; i < contours.size(); i++) {
		mc[i] = cv::Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}

	/// Draw contours
	cv::Mat drawing = cv::Mat::zeros(canny_output.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++) {
		cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));
		cv::drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0,
				cv::Point());
		cv::circle(drawing, mc[i], 4, color, -1, 8, 0);
	}

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(drawing),
			"image moments result");
	other->move(x() + 40, y() + 40);
	other->show();
}

void ImageViewer::harrisAction() {
	qDebug() << "ImageViewer::harrisAction ";

	cv::Mat src_gray;
	int thresh = 200;

	cv::cvtColor(matImage, src_gray, CV_BGR2GRAY);

	cv::Mat dst, dst_norm, dst_norm_scaled;
	dst = cv::Mat::zeros(matImage.size(), CV_32FC1);

	/// Detector parameters
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;

	/// Detecting corners
	cv::cornerHarris(src_gray, dst, blockSize, apertureSize, k,
			cv::BORDER_DEFAULT);

	/// Normalizing
	cv::normalize(dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());
	cv::convertScaleAbs(dst_norm, dst_norm_scaled);

	/// Drawing a circle around corners
	for (int j = 0; j < dst_norm.rows; j++) {
		for (int i = 0; i < dst_norm.cols; i++) {
			if ((int) dst_norm.at<float>(j, i) > thresh) {
				cv::circle(dst_norm_scaled, cv::Point(i, j), 5, cv::Scalar(0),
						2, 8, 0);
			}
		}
	}

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(dst_norm_scaled),
			"corner harris result");
	other->move(x() + 40, y() + 40);
	other->show();

}
void ImageViewer::shiTomasiAction() {
	qDebug() << "ImageViewer::shiTomasiAction ";

	cv::Mat src_gray;

	int maxCorners = 23;

	cv::RNG rng(12345);
	cv::cvtColor(matImage, src_gray, CV_BGR2GRAY);

	if (maxCorners < 1) {
		maxCorners = 1;
	}

	/// Parameters for Shi-Tomasi algorithm
	vector < cv::Point2f > corners;
	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;

	/// Copy the source image
	cv::Mat copy;
	copy = matImage.clone();

	/// Apply corner detection
	cv::goodFeaturesToTrack(src_gray, corners, maxCorners, qualityLevel,
			minDistance, cv::Mat(), blockSize, useHarrisDetector, k);

	/// Draw corners detected
	qDebug() << "** Number of corners detected: " << corners.size();
	int r = 4;
	for (size_t i = 0; i < corners.size(); i++) {
		cv::circle(copy, corners[i], r,
				cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
						rng.uniform(0, 255)), -1, 8, 0);
	}

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(copy),
			"corner Shi-Tomasi result");
	other->move(x() + 40, y() + 40);
	other->show();

}
void ImageViewer::cornerDetectAction() {
	qDebug() << "ImageViewer::custCornerDetectAction ";

	cv::Mat src_gray;
	cv::Mat myHarris_dst, myHarris_copy, Mc;
	cv::Mat myShiTomasi_dst, myShiTomasi_copy;

	int myShiTomasi_qualityLevel = 50;
	int myHarris_qualityLevel = 50;
	int max_qualityLevel = 100;

	double myHarris_minVal;
	double myHarris_maxVal;
	double myShiTomasi_minVal;
	double myShiTomasi_maxVal;

	cv::RNG rng(12345);

	cv::cvtColor(matImage, src_gray, cv::COLOR_BGR2GRAY);

	/// Set some parameters
	int blockSize = 3;
	int apertureSize = 3;

	/// My Harris matrix -- Using cornerEigenValsAndVecs
	myHarris_dst = cv::Mat::zeros(src_gray.size(), CV_32FC(6));
	Mc = cv::Mat::zeros(src_gray.size(), CV_32FC1);

	cv::cornerEigenValsAndVecs(src_gray, myHarris_dst, blockSize, apertureSize,
			cv::BORDER_DEFAULT);

	/* calculate Mc */
	for (int j = 0; j < src_gray.rows; j++) {
		for (int i = 0; i < src_gray.cols; i++) {
			float lambda_1 = myHarris_dst.at < cv::Vec6f > (j, i)[0];
			float lambda_2 = myHarris_dst.at < cv::Vec6f > (j, i)[1];
			Mc.at<float>(j, i) = lambda_1 * lambda_2
					- 0.04f * pow((lambda_1 + lambda_2), 2);
		}
	}

	cv::minMaxLoc(Mc, &myHarris_minVal, &myHarris_maxVal, 0, 0, cv::Mat());

	/* Create Window and Trackbar */
	myHarris_copy = matImage.clone();

	if (myHarris_qualityLevel < 1) {
		myHarris_qualityLevel = 1;
	}

//	  for( int j = 0; j < src_gray.rows; j++ )
//	     { for( int i = 0; i < src_gray.cols; i++ )
//	          {
//	            if( Mc.at<float>(j,i) > myHarris_minVal + ( myHarris_maxVal - myHarris_minVal )*myHarris_qualityLevel/max_qualityLevel )
//	              { cv::circle( myHarris_copy, cv::Point(i,j), 4, cv::Scalar( rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255) ), -1, 8, 0 ); }
//	          }
//	     }

//	cv::namedWindow("Customize Harris Demo", CV_WINDOW_AUTOSIZE);
//	cv::imshow("Customize Harris Demo", myHarris_copy  );

	/// My Shi-Tomasi -- Using cornerMinEigenVal
	myShiTomasi_dst = cv::Mat::zeros(src_gray.size(), CV_32FC1);
	cv::cornerMinEigenVal(src_gray, myShiTomasi_dst, blockSize, apertureSize,
			cv::BORDER_DEFAULT);

	cv::minMaxLoc(myShiTomasi_dst, &myShiTomasi_minVal, &myShiTomasi_maxVal, 0,
			0, cv::Mat());

	/* Create Window and Trackbar */
	myShiTomasi_copy = matImage.clone();

	if (myShiTomasi_qualityLevel < 1) {
		myShiTomasi_qualityLevel = 1;
	}

	for (int j = 0; j < src_gray.rows; j++) {
		for (int i = 0; i < src_gray.cols; i++) {
			if (Mc.at<float>(j, i)
					> myHarris_minVal
							+ (myHarris_maxVal - myHarris_minVal)
									* myHarris_qualityLevel
									/ max_qualityLevel) {
				//note: be careful _ thaonh
				cv::circle(myShiTomasi_copy, cv::Point(i, j), 4,
						cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
								rng.uniform(0, 255)), -1, 8, 0);
			}
			if (myShiTomasi_dst.at<float>(j, i)
					> myShiTomasi_minVal
							+ (myShiTomasi_maxVal - myShiTomasi_minVal)
									* myShiTomasi_qualityLevel
									/ max_qualityLevel) {
				cv::circle(myShiTomasi_copy, cv::Point(i, j), 4,
						cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
								rng.uniform(0, 255)), -1, 8, 0);
			}
		}
	}

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(myShiTomasi_copy),
			"corner detector result");
	other->move(x() + 40, y() + 40);
	other->show();
}

void ImageViewer::featureSurfAction() {
	qDebug() << "ImageViewer::featureSurfAction";

	/*
	 cv::Mat src_gray;// = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
	 cv::cvtColor( matImage, src_gray, CV_RGB2GRAY );

	 int minHessian = 400;

	 cv::SurfFeatureDetector detector( minHessian );

	 std::vector<cv::KeyPoint> keypoints;

	 detector.detect( src_gray, keypoints );

	 //-- Draw keypoints
	 cv::Mat img_keypoints;

	 cv::drawKeypoints( src_gray, keypoints, img_keypoints, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT );
	 */
	featureSurfAction(ui::SurfPanel::DF_MIN_HESSIAN, false);

}

void ImageViewer::featureSurfAction(int minHessian, bool refresh) {
	cv::SurfFeatureDetector detector(minHessian);
	std::vector < cv::KeyPoint > keypoints;
	detector.detect(matImage, keypoints);
	//-- Draw keypoints
	cv::Mat img_keypoints;
	cv::drawKeypoints(matImage, keypoints, img_keypoints, cv::Scalar::all(-1),
			cv::DrawMatchesFlags::DEFAULT);

	if (refresh) {
		qImage = ImageConvert::cvMatToQImage(img_keypoints);
		imageLabel->setPixmap(QPixmap::fromImage(qImage));
		statusBar()->showMessage(
				tr("Min hessian: ") + QString::number(minHessian));
	} else {
		ImageViewer* other = new ImageViewer;
		other->loadImage(matImage, ImageConvert::cvMatToQImage(img_keypoints),
				"SUFT feature detector result");
		other->parameterPanel = new ui::SurfPanel(other);
		other->move(x() + 40, y() + 40);
		other->statusBar()->showMessage(
				tr("Min hessian: ") + QString::number(minHessian));
		other->show();

	}
}

void ImageViewer::featureSiftAction() {
	qDebug() << "ImageViewer::featureSiftAction ";

	cv::Mat src_gray; // = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
	cv::cvtColor(matImage, src_gray, CV_RGB2GRAY);

	cv::SiftFeatureDetector detector;

	std::vector < cv::KeyPoint > keypoints;

	detector.detect(src_gray, keypoints);

	//-- Draw keypoints
	cv::Mat img_keypoints;

	cv::drawKeypoints(src_gray, keypoints, img_keypoints, cv::Scalar::all(-1),
			cv::DrawMatchesFlags::DEFAULT);

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(img_keypoints),
			"SIFT feature detector result");
	other->move(x() + 40, y() + 40);
	other->show();
}
void ImageViewer::matchingBFAction() {
	qDebug() << "ImageViewer::matchingBFAction";

	cv::Mat img_2; // = cv::imread(child->m_filename.toStdString(),CV_LOAD_IMAGE_GRAYSCALE);
	cv::cvtColor(matImage, img_2, CV_RGB2GRAY);

	QString fileName = QFileDialog::getOpenFileName(this);
	if (fileName.isEmpty())
		return;

	cv::Mat img_1 = cv::imread(fileName.toStdString(), CV_LOAD_IMAGE_GRAYSCALE);

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;

	cv::SurfFeatureDetector detector(minHessian);

	std::vector<cv::KeyPoint> keypoints_1, keypoints_2;

	detector.detect(img_1, keypoints_1);
	detector.detect(img_2, keypoints_2);

	//-- Step 2: Calculate descriptors (feature vectors)
	cv::SurfDescriptorExtractor extractor;

	cv::Mat descriptors_1, descriptors_2;

	extractor.compute(img_1, keypoints_1, descriptors_1);
	extractor.compute(img_2, keypoints_2, descriptors_2);

//	cv::namedWindow("Template matching Demo", CV_WINDOW_AUTOSIZE);
//	cv::imshow("Template matching Demo", descriptors_2  );

	//-- Step 3: Matching descriptor vectors with a brute force matcher
	cv::BFMatcher matcher(cv::NORM_L2);
	std::vector < cv::DMatch > matches;
	matcher.match(descriptors_1, descriptors_2, matches);

	//-- Draw matches
	cv::Mat img_matches;
	cv::drawMatches(img_1, keypoints_1, img_2, keypoints_2, matches,
			img_matches);

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(img_matches),
			"description with brute force matcher");
	other->move(x() + 40, y() + 40);
	other->show();
}
void ImageViewer::matchingFLANNAction() {
	qDebug() << "ImageViewer::featureFLANNAction ";

	cv::Mat img_2; // = cv::imread(child->m_filename.toStdString(),CV_LOAD_IMAGE_GRAYSCALE);
	cv::cvtColor(matImage, img_2, CV_RGB2GRAY);

	QString fileName = QFileDialog::getOpenFileName(this);
	if (fileName.isEmpty())
		return;

	cv::Mat img_1 = cv::imread(fileName.toStdString(), CV_LOAD_IMAGE_GRAYSCALE);

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;

	cv::SurfFeatureDetector detector(minHessian);

	std::vector<cv::KeyPoint> keypoints_1, keypoints_2;

	detector.detect(img_1, keypoints_1);
	detector.detect(img_2, keypoints_2);

	//-- Step 2: Calculate descriptors (feature vectors)
	cv::SurfDescriptorExtractor extractor;

	cv::Mat descriptors_1, descriptors_2;

	extractor.compute(img_1, keypoints_1, descriptors_1);
	extractor.compute(img_2, keypoints_2, descriptors_2);

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	cv::FlannBasedMatcher matcher;
	std::vector < cv::DMatch > matches;
	matcher.match(descriptors_1, descriptors_2, matches);

	double max_dist = 0;
	double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_1.rows; i++) {
		double dist = matches[i].distance;
		if (dist < min_dist)
			min_dist = dist;
		if (dist > max_dist)
			max_dist = dist;
	}

	qDebug("-- Max dist : %f \n", max_dist);
	qDebug("-- Min dist : %f \n", min_dist);

	//-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
	//-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
	//-- small)
	//-- PS.- radiusMatch can also be used here.
	std::vector < cv::DMatch > good_matches;

	for (int i = 0; i < descriptors_1.rows; i++) {
		if (matches[i].distance <= max(2 * min_dist, 0.02)) {
			good_matches.push_back(matches[i]);
		}
	}

	//-- Draw only "good" matches
	cv::Mat img_matches;
	cv::drawMatches(img_1, keypoints_1, img_2, keypoints_2, good_matches,
			img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
			vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(img_matches),
			"description with FLANN matcher");
	other->move(x() + 40, y() + 40);
	other->show();
}
void ImageViewer::matchingHomographyAction() {

	qDebug() << "ImageViewer::matchingHomographyAction";

	cv::Mat img_scene; //= cv::imread(child->m_filename.toStdString(),CV_LOAD_IMAGE_GRAYSCALE);
	cv::cvtColor(matImage, img_scene, CV_RGB2GRAY);

	QString fileName = QFileDialog::getOpenFileName(this);
	if (fileName.isEmpty())
		return;

	cv::Mat img_object = cv::imread(fileName.toStdString(),
			CV_LOAD_IMAGE_GRAYSCALE);

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;

	cv::SurfFeatureDetector detector(minHessian);

	std::vector<cv::KeyPoint> keypoints_object, keypoints_scene;

	detector.detect(img_object, keypoints_object);
	detector.detect(img_scene, keypoints_scene);

	//-- Step 2: Calculate descriptors (feature vectors)
	cv::SurfDescriptorExtractor extractor;

	cv::Mat descriptors_object, descriptors_scene;

	extractor.compute(img_object, keypoints_object, descriptors_object);
	extractor.compute(img_scene, keypoints_scene, descriptors_scene);

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	cv::FlannBasedMatcher matcher;
	std::vector < cv::DMatch > matches;
	matcher.match(descriptors_object, descriptors_scene, matches);

	double max_dist = 0;
	double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_object.rows; i++) {
		double dist = matches[i].distance;
		if (dist < min_dist)
			min_dist = dist;
		if (dist > max_dist)
			max_dist = dist;
	}

	qDebug("-- Max dist : %f \n", max_dist);
	qDebug("-- Min dist : %f \n", min_dist);

	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector < cv::DMatch > good_matches;

	for (int i = 0; i < descriptors_object.rows; i++) {
		if (matches[i].distance < 3 * min_dist) {
			good_matches.push_back(matches[i]);
		}
	}

	cv::Mat img_matches;
	cv::drawMatches(img_object, keypoints_object, img_scene, keypoints_scene,
			good_matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
			vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//-- Localize the object
	std::vector < cv::Point2f > obj;
	std::vector < cv::Point2f > scene;

	for (size_t i = 0; i < good_matches.size(); i++) {
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}

	//error _ thaonh
//	cv::Mat H = cv::findHomography( obj, scene, CV_RANSAC );
//
//	//-- Get the corners from the image_1 ( the object to be "detected" )
//	std::vector<cv::Point2f> obj_corners(4);
//	obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( img_object.cols, 0 );
//	obj_corners[2] = cvPoint( img_object.cols, img_object.rows ); obj_corners[3] = cvPoint( 0, img_object.rows );
//	std::vector<cv::Point2f> scene_corners(4);
//
//	cv::perspectiveTransform( obj_corners, scene_corners, H);
//
//	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
//	cv::line( img_matches, scene_corners[0] + cv::Point2f( img_object.cols, 0), scene_corners[1] + cv::Point2f( img_object.cols, 0), cv::Scalar(0, 255, 0), 4 );
//	cv::line( img_matches, scene_corners[1] + cv::Point2f( img_object.cols, 0), scene_corners[2] + cv::Point2f( img_object.cols, 0), cv::Scalar( 0, 255, 0), 4 );
//	cv::line( img_matches, scene_corners[2] + cv::Point2f( img_object.cols, 0), scene_corners[3] + cv::Point2f( img_object.cols, 0), cv::Scalar( 0, 255, 0), 4 );
//	cv::line( img_matches, scene_corners[3] + cv::Point2f( img_object.cols, 0), scene_corners[0] + cv::Point2f( img_object.cols, 0), cv::Scalar( 0, 255, 0), 4 );

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(img_matches),
			"Matching Homography result");
	other->move(x() + 40, y() + 40);
	other->show();
}
void ImageViewer::matchingTemplateAction() {
	qDebug() << "ImageViewer::matchingTemplateAction";

	QString fileName = QFileDialog::getOpenFileName(this);
	if (fileName.isEmpty())
		return;

	cv::Mat templ = cv::imread(fileName.toStdString(), 1);

	cv::Mat result;

	int match_method = 0;

	/// Source image to display
	cv::Mat img_display;
	matImage.copyTo(img_display);

	/// Create the result matrix
	int result_cols = matImage.cols - templ.cols + 1;
	int result_rows = matImage.rows - templ.rows + 1;

	result.create(result_cols, result_rows, CV_32FC1);

	/// Do the Matching and Normalize
	cv::matchTemplate(matImage, templ, result, match_method);
	cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

	/// Localizing the best match with minMaxLoc
	double minVal;
	double maxVal;
	cv::Point minLoc;
	cv::Point maxLoc;
	cv::Point matchLoc;

	cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED) {
		matchLoc = minLoc;
	} else {
		matchLoc = maxLoc;
	}

	/// Show me what you got
	cv::rectangle(img_display, matchLoc,
			cv::Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows),
			cv::Scalar::all(0), 2, 8, 0);
	cv::rectangle(result, matchLoc,
			cv::Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows),
			cv::Scalar::all(0), 2, 8, 0);

//	cv::namedWindow("Template matching display Demo", CV_WINDOW_AUTOSIZE);
//	cv::imshow("Template matching display Demo", img_display  );

	cv::namedWindow("Template matching Demo", CV_WINDOW_AUTOSIZE);
	cv::imshow("Template matching Demo", result);

//	new MainWindow(result,ImageConvert::cvMatToQImage(result),x() + 40, y() + 40);

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(img_display),
			"Matching template result");
	other->move(x() + 40, y() + 40);
	other->show();
}

void ImageViewer::pyramidAction() {
	qDebug() << "ImageViewer::pyramidAction ";

	cv::Mat tmp = matImage, dst;
	int type = 1;

	if (type == 0) {
		cv::pyrUp(tmp, dst, cv::Size(tmp.cols * 2, tmp.rows * 2));
	} else {
		cv::pyrDown(tmp, dst, cv::Size(tmp.cols / 2, tmp.rows / 2));
	}

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(dst),
			"Pyrymid result");
	other->move(x() + 40, y() + 40);
	other->show();

}

void ImageViewer::remapAction() {

	qDebug() << "ImageViewer::remapAction ";

	cv::Mat dst, map_x, map_y;
	int ind = 1;

	dst.create(matImage.size(), matImage.type());
	map_x.create(matImage.size(), CV_32FC1);
	map_y.create(matImage.size(), CV_32FC1);

	for (int j = 0; j < matImage.rows; j++) {
		for (int i = 0; i < matImage.cols; i++) {
			switch (ind) {
			case 0:
				if (i > matImage.cols * 0.25 && i < matImage.cols * 0.75
						&& j > matImage.rows * 0.25
						&& j < matImage.rows * 0.75) {
					map_x.at<float>(j, i) = 2 * (i - matImage.cols * 0.25)
							+ 0.5;
					map_y.at<float>(j, i) = 2 * (j - matImage.rows * 0.25)
							+ 0.5;
				} else {
					map_x.at<float>(j, i) = 0;
					map_y.at<float>(j, i) = 0;
				}
				break;
			case 1:
				map_x.at<float>(j, i) = i;
				map_y.at<float>(j, i) = matImage.rows - j;
				break;
			case 2:
				map_x.at<float>(j, i) = matImage.cols - i;
				map_y.at<float>(j, i) = j;
				break;
			case 3:
				map_x.at<float>(j, i) = matImage.cols - i;
				map_y.at<float>(j, i) = matImage.rows - j;
				break;
			} // end of switch
		}
	}

	cv::remap(matImage, dst, map_x, map_y, CV_INTER_LINEAR, cv::BORDER_CONSTANT,
			cv::Scalar(0, 0, 0));

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(dst),
			"Remapping result");
	other->move(x() + 40, y() + 40);
	other->show();

}

void ImageViewer::affineAction() {
	qDebug() << "ImageViewer::affineAction ";

	cv::Point2f srcTri[3];
	cv::Point2f dstTri[3];

	cv::Mat rot_mat(2, 3, CV_32FC1);
	cv::Mat warp_mat(2, 3, CV_32FC1);
	cv::Mat warp_dst, warp_rotate_dst;

	/// Set the dst image the same type and size as src
	warp_dst = cv::Mat::zeros(matImage.rows, matImage.cols, matImage.type());

	/// Set your 3 points to calculate the  Affine Transform
	srcTri[0] = cv::Point2f(0, 0);
	srcTri[1] = cv::Point2f(matImage.cols - 1, 0);
	srcTri[2] = cv::Point2f(0, matImage.rows - 1);

	dstTri[0] = cv::Point2f(matImage.cols * 0.0, matImage.rows * 0.33);
	dstTri[1] = cv::Point2f(matImage.cols * 0.85, matImage.rows * 0.25);
	dstTri[2] = cv::Point2f(matImage.cols * 0.15, matImage.rows * 0.7);

	/// Get the Affine Transform
	warp_mat = cv::getAffineTransform(srcTri, dstTri);

	/// Apply the Affine Transform just found to the src image
	cv::warpAffine(matImage, warp_dst, warp_mat, warp_dst.size());

	/** Rotating the image after Warp */

	/// Compute a rotation matrix with respect to the center of the image
	cv::Point center = cv::Point(warp_dst.cols / 2, warp_dst.rows / 2);
	double angle = -50.0;
	double scale = 0.6;

	/// Get the rotation matrix with the specifications above
	rot_mat = cv::getRotationMatrix2D(center, angle, scale);

	/// Rotate the warped image
	cv::warpAffine(warp_dst, warp_rotate_dst, rot_mat, warp_dst.size());

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(warp_rotate_dst),
			"Affine result");
	other->move(x() + 40, y() + 40);
	other->show();

}

void ImageViewer::histogramAction() {
	qDebug() << "MainWindow::histogramAction ";

	vector < cv::Mat > bgr_planes;
	cv::split(ImageConvert::QImageToCvMat(qImage), bgr_planes);

	/// Establish the number of bins
	int histSize = 256;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true;
	bool accumulate = false;

	cv::Mat b_hist, g_hist, r_hist;

	/// Compute the histograms:
	cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize,
			&histRange, uniform, accumulate);
	cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize,
			&histRange, uniform, accumulate);
	cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize,
			&histRange, uniform, accumulate);

	// Draw the histograms for B, G and R
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double) hist_w / histSize);

	cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

	/// Normalize the result to [ 0, histImage.rows ]
	cv::normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1,
			cv::Mat());
	cv::normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1,
			cv::Mat());
	cv::normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1,
			cv::Mat());

	/// Draw for each channel
	for (int i = 1; i < histSize; i++) {
		cv::line(histImage,
				cv::Point(bin_w * (i - 1),
						hist_h - cvRound(b_hist.at<float>(i - 1))),
				cv::Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
				cv::Scalar(255, 0, 0), 2, 8, 0);
		cv::line(histImage,
				cv::Point(bin_w * (i - 1),
						hist_h - cvRound(g_hist.at<float>(i - 1))),
				cv::Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
				cv::Scalar(0, 255, 0), 2, 8, 0);
		cv::line(histImage,
				cv::Point(bin_w * (i - 1),
						hist_h - cvRound(r_hist.at<float>(i - 1))),
				cv::Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
				cv::Scalar(0, 0, 255), 2, 8, 0);
	}

	cv::namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
	cv::imshow("Histogram", histImage);
}

void ImageViewer::equalizationAction() {
	qDebug() << "ImageViewer::equalizationAction ";
	cv::Mat dst;
	cv::cvtColor(matImage, matImage, CV_BGR2GRAY);
	cv::equalizeHist(matImage, dst);

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(dst),
			"Equalization result");
	other->move(x() + 40, y() + 40);
	other->show();

}

void ImageViewer::backprojectionAction() {
	qDebug() << "ImageViewer::backprojectionAction ";

	cv::Mat hsv, hue;
	int bins = 25;
	cv::cvtColor(matImage, hsv, CV_BGR2HSV);
	/// Use only the Hue value
	hue.create(hsv.size(), hsv.depth());
	int ch[] = { 0, 0 };
	cv::mixChannels(&hsv, 1, &hue, 1, ch, 1);

	cv::MatND hist;
	int histSize = MAX(bins, 2);
	float hue_range[] = { 0, 180 };
	const float* ranges = { hue_range };

	/// Get the Histogram and normalize it
	cv::calcHist(&hue, 1, 0, cv::Mat(), hist, 1, &histSize, &ranges, true,
			false);
	cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());

	/// Get Backprojection
	cv::MatND backproj;
	cv::calcBackProject(&hue, 1, 0, hist, backproj, &ranges, 1, true);

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(backproj),
			"back projection result");
	other->move(x() + 40, y() + 40);
	other->show();
}

void ImageViewer::grayscaleMyImplAction() {
	qDebug() << "ImageViewer::grayscaleMyImplAction ";
	QImage img = ImageConvert::convertGrayScale(qImage);
	ImageViewer* other = new ImageViewer;
	other->loadImage(ImageConvert::QImageToCvMat(img), img,
			"gray scale result");
	other->move(x() + 40, y() + 40);
	other->show();
}
void ImageViewer::binaryMyImplAction() {
	qDebug() << "ImageViewer::binaryMyImplAction";
	QImage img = ImageConvert::convertBinary(qImage);
	cv::Mat mat = ImageConvert::QImageToCvMat(img);
	ImageViewer* other = new ImageViewer;
	other->loadImage(mat, img, "binary threshold result");
	other->move(x() + 40, y() + 40);
	other->show();
}
void ImageViewer::robertsMyImplAction() {
	qDebug() << "ImageViewer::robertsMyImplAction ";
	QImage img = MyImpl::Roberts(qImage);
	cv::Mat mat = ImageConvert::QImageToCvMat(img);
	ImageViewer* other = new ImageViewer;
	other->loadImage(mat, img, "robert result");
	other->move(x() + 40, y() + 40);
	other->show();
}
void ImageViewer::sobelMyImplAction() {
	qDebug() << "ImageViewer::sobelMyImplAction ";
	QImage img = MyImpl::Sobel(qImage);
	cv::Mat mat = ImageConvert::QImageToCvMat(img);
	ImageViewer* other = new ImageViewer;
	other->loadImage(mat, img, "sobel result");
	other->move(x() + 40, y() + 40);
	other->show();
}
void ImageViewer::hitmissMyImplAction() {
	qDebug() << "ImageViewer::hitmissMyImplAction ";
	HitMissParams opt;
	QImage img = MyImpl::HitMiss(qImage, opt);
	cv::Mat mat = ImageConvert::QImageToCvMat(img);
	ImageViewer* other = new ImageViewer;
	other->loadImage(mat, img, "Hit & Miss transform result");
	other->move(x() + 40, y() + 40);
	other->show();
}

void ImageViewer::otsuMyImplAction() {
	qDebug() << "ImageViewer::otsuMyImplAction ";
	QImage img = MyImpl::Otsu(qImage, 3);
	cv::Mat mat = ImageConvert::QImageToCvMat(img);
	ImageViewer* other = new ImageViewer;
	other->loadImage(mat, img, "Otsu-multilevel result");
	other->move(x() + 40, y() + 40);
	other->show();
}
void ImageViewer::growingMyImplAction() {
	qDebug() << "ImageViewer::growingMyImplAction ";
	QImage img = MyImpl::Growing(qImage, 30);
	cv::Mat mat = ImageConvert::QImageToCvMat(img);
	ImageViewer* other = new ImageViewer;
	other->loadImage(mat, img, "Region growing result");
	other->move(x() + 40, y() + 40);
	other->show();
}
void ImageViewer::antMyImplAction() {
	qDebug() << "ImageViewer::antMyImplAction ";
	AntMethodParams opt;
	QImage img = MyImpl::AntMethod(qImage, opt);
	cv::Mat mat = ImageConvert::QImageToCvMat(img);
	ImageViewer* other = new ImageViewer;
	other->loadImage(mat, img, "Social ant result");
	other->move(x() + 40, y() + 40);
	other->show();
}

void ImageViewer::correctMorphAction() {
	qDebug() << "ImageViewer::correctMorphAction";

	cv::Mat img_2; // = cv::imread(child->m_filename.toStdString(),CV_LOAD_IMAGE_GRAYSCALE);
	cv::cvtColor(matImage, img_2, CV_RGB2GRAY);

	QString fileName = QFileDialog::getOpenFileName(this);
	if (fileName.isEmpty())
		return;

	cv::Mat img_1 = cv::imread(fileName.toStdString(), CV_LOAD_IMAGE_GRAYSCALE);

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;

	cv::SurfFeatureDetector detector(minHessian);

	std::vector<cv::KeyPoint> keypoints_1, keypoints_2;

	detector.detect(img_1, keypoints_1);
	detector.detect(img_2, keypoints_2);

	//-- Step 2: Calculate descriptors (feature vectors)
	cv::SurfDescriptorExtractor extractor;

	cv::Mat descriptors_1, descriptors_2;

	extractor.compute(img_1, keypoints_1, descriptors_1);
	extractor.compute(img_2, keypoints_2, descriptors_2);

//	cv::namedWindow("Template matching Demo", CV_WINDOW_AUTOSIZE);
//	cv::imshow("Template matching Demo", descriptors_2  );

	//-- Step 3: Matching descriptor vectors with a brute force matcher
	cv::BFMatcher matcher(cv::NORM_L2);
	std::vector < cv::DMatch > matches;
	matcher.match(descriptors_1, descriptors_2, matches);

	//-- Draw matches
	cv::Mat img_matches;
	cv::drawMatches(img_1, keypoints_1, img_2, keypoints_2, matches,
			img_matches);

	ImageViewer* other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(img_matches),
			"description with brute force matcher");
	other->move(x() + 40, y() + 40);
	other->show();
}

// ====== add by LE Van Linh

/*
 * Compute the PGH measure distance by matching method and accuracy of angle and distance
 * between an image and a images folder
 * @parameter 1: image - the reference image
 * @paramter 2: path - the images folder
 */
void ImageViewer::matchingDirectory(Image image, QString path, QString fileSave,
		Image::SegmentMethod sgmethod) {
	map<string, int> resources = ReadResouces::readResources(
			"data/resources/pgh.rc");
	int angleAcc = resources["PGHAngleAcc"];
	int distanceAcc = resources["PGHDistanceAcc"];
	Scenario::matchingDirectory(image, path, GeometricHistogram::Bhattacharyya,
			((LocalHistogram::AccuracyPGH) angleAcc), distanceAcc, fileSave,
			sgmethod);
}

/*
 * Remove the yellow grid on an image
 */
void ImageViewer::removeYLinesAction() {

	// run on all directory

	//readDirectory("/home/linh/Desktop/mandibule");

	//readDirectory("/home/Images/Morphometrics/elytre/Original_images"); // ok
	//readDirectory("/home/Images/Morphometrics/mandibule-droite/Original_images");
	//readDirectory("/home/Images/Morphometrics/mandibule-gauche/Original_images");
	//readDirectory("/home/Images/Morphometrics/pronotum/Original_images");
	//readDirectory("/home/Images/Morphometrics/tete/Original_images");

	// run on a image
	Image image(fileName);
	cv::Mat enddest = image.removingGrid(90);
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(enddest),
			"Removing the yellow grid -- " + this->fileName);
	other->show();
}

/*
 * Indentify the landmarks on an image by a model
 */
void ImageViewer::getLandmarks() {

	qDebug() << "Identification of landmarks function ...";
	map<string, int> resources = ReadResouces::readResources(
			"data/resources/est.rc");
	int templSize = resources["EstTemplSize"];
	int imageSize = resources["EstImageSize"];
	Image image(fileName);
	QMessageBox msgbox;

	/*
	 * Working on an image
	 */
	msgbox.setText("Select the model image.");
	msgbox.exec();

	QString fileName2 = QFileDialog::getOpenFileName(this);
	if (fileName2.isEmpty())
		return;
	Image sceneImage(fileName2);
	qDebug() << fileName2;

	msgbox.setText("Select the landmark file of model image.");
	msgbox.exec();

	QString lpath = QFileDialog::getOpenFileName(this);

	Image::SegmentMethod sgmethod = chooseSegMethod();
	if (!checkPresent(image, sceneImage, sgmethod)) {
		msgbox.setText(
				"Sorry, we can not detect the similar between two images.");
		msgbox.exec();
		return;
	}

	vector < Point > esLandmarks;
	double angle;
	Point ePoint;

	Mat enddest = Scenario::landmarksMatching(sceneImage, image, lpath,
			templSize, imageSize, sgmethod, esLandmarks, angle, ePoint);
	int save = saveOrNot();
	if (save == 1) {
		msgbox.setText("Choose the folder to save the result");
		msgbox.exec();
		QString folderPath = QFileDialog::getExistingDirectory(this);
		QString savePath = folderPath + "/E" + sceneImage.getName() + ".TPS";
		LandmarkDetection lmDectection;
		lmDectection.save_Estimated_Landmarks_To_File(esLandmarks, savePath);
	}
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(enddest),
			"Landmark -- " + this->fileName);
	other->fileName = fileName;
	other->setContextMenuPolicy(Qt::CustomContextMenu);
	other->esLandmarks = esLandmarks;
	other->angleDiff = angle;
	other->ePoint = ePoint;
	connect(other, SIGNAL(customContextMenuRequested(const QPoint&)), other,
			SLOT(estContextMenu(const QPoint&)));

	//emit estContextMenu(const p,esLandmarks,angle);

	other->show();
	qDebug() << "Done";
}
void ImageViewer::estContextMenu(const QPoint& pos) {
	QPoint globalpos = this->mapToGlobal(pos);
	QMenu menu(this);
	QMenu *putIn = menu.addMenu("Put in");
	QMenu *putOut = menu.addMenu("Put out");

	QAction *act1 = new QAction("Original landmarks", this);
	connect(act1, SIGNAL(triggered()), this, SLOT(putInOrgLandmarks()));
	putIn->addAction(act1);

	QAction *act2 = new QAction("Lines segment", this);
	connect(act2, SIGNAL(triggered()), this, SLOT(putInLines()));
	putIn->addAction(act2);

	QAction *oact1 = new QAction("Original landmarks", this);
	connect(oact1, SIGNAL(triggered()), this, SLOT(putOutOrgLandmarks()));
	putOut->addAction(oact1);
	if (orgLandmarks.size() > 0)
		oact1->setEnabled(true);
	else
		oact1->setEnabled(false);

	QAction *oact2 = new QAction("Lines segment", this);
	connect(oact2, SIGNAL(triggered()), this, SLOT(putOutLines()));
	putOut->addAction(oact2);
	if (lines.size() > 0)
		oact2->setEnabled(true);
	else
		oact2->setEnabled(false);

	menu.exec(globalpos);
}

void ImageViewer::putInOrgLandmarks() {
	qDebug() << "Load context menu";
	Image sceneImage(this->fileName);

	QMessageBox msgbox;
	msgbox.setText("Select the original landmarks.");
	msgbox.exec();
	QString lpath = QFileDialog::getOpenFileName(this);
	vector < Point > orgLandmarks = sceneImage.readLandmarksFile(
			lpath.toStdString());
	this->orgLandmarks = orgLandmarks;

	optionDisplay(sceneImage, this->esLandmarks, orgLandmarks, this->lines,
			this->angleDiff, this->ePoint);
}

void ImageViewer::putInLines() {
	Image sceneImage(this->fileName);
	Mat enddest = sceneImage.getMatrixImage();

	Image::SegmentMethod sgmethod = chooseSegMethod();
	int tValue;
	vector<Line> lineSg = sceneImage.lineSegment(sgmethod, tValue);
	this->lines = lineSg;
	optionDisplay(sceneImage, this->esLandmarks, this->orgLandmarks,
			this->lines, this->angleDiff, this->ePoint);
}

void ImageViewer::putOutLines() {
	Image sceneImage(this->fileName);
	this->lines.clear();
	optionDisplay(sceneImage, this->esLandmarks, this->orgLandmarks,
			this->lines, this->angleDiff, this->ePoint);
}
void ImageViewer::putOutOrgLandmarks() {
	Image sceneImage(this->fileName);
	this->orgLandmarks.clear();
	optionDisplay(sceneImage, this->esLandmarks, this->orgLandmarks,
			this->lines, this->angleDiff, this->ePoint);
}
void ImageViewer::optionDisplay(impls_2015::Image image, vector<Point> esLM,
		vector<Point> orgLM, vector<impls_2015::Line> lines, double angle,
		Point ePoint) {
	Mat enddest = image.getMatrixImage();
	if (orgLM.size() > 0) {
		for (size_t t = 0; t < orgLM.size(); t++) {
			Point temp = orgLM.at(t);
			Point orgLM(temp.x, enddest.rows - temp.y);
			circle(enddest, orgLM, 5, Scalar(0, 0, 255), 2, 8);
		}
	}
	if (lines.size() > 0) {
		for (size_t i = 0; i < lines.size(); i++) {
			Line line = this->lines.at(i);
			line.drawing(enddest);
		}
	}

	Mat result(Image::rotateImage(enddest, angle, ePoint).clone());
	if (esLM.size() > 0) {
		for (size_t t = 0; t < esLM.size(); t++) {
			Point point = esLM.at(t);
			circle(result, point, 5, Scalar(0, 255, 255), 2, 8);
		}
	}
	qImage = ImageConvert::cvMatToQImage(result);
	imageLabel->setPixmap(QPixmap::fromImage(qImage));
}
/*
 * Line segmentation from an image
 */
void ImageViewer::edgeSegmentation() {
	qDebug() << "Edge segmentation.";

	/*
	 * Working on an image
	 */
	Image image(fileName);
	qDebug() << image.getName();

	Image::SegmentMethod sgmethod = chooseSegMethod();

	cv::Mat enddest(image.getMatrixImage().clone());

	vector<Line> lineSegment = Scenario::edgeSegmentation(image, enddest,
			sgmethod);

	int save = saveOrNot();
	QMessageBox msgbox;
	if (save == 1) {
		msgbox.setText("Choose the folder to save the result");
		msgbox.exec();
		QString folderPath = QFileDialog::getExistingDirectory(this);
		QString savePath = folderPath + "/" + image.getName() + ".PGH";
		EdgeSegmentation edgeSeg;
		edgeSeg.savePGHFile(lineSegment, savePath);
	}

	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(enddest),
			"Edge segmentation");
	other->addParameterPanel(
			new impls_2015::EdgeSegmentationPanel(other, fileName, sgmethod,
					image.getThresholdValue()), x() + 20, y() + 20);

	qDebug() << "Done";
}

void ImageViewer::edgeSegmentation_Value_Changed(QString filePath,
		int thresholdValue) {

	Image image(filePath);
	vector<Edge> edges = image.getEdges(thresholdValue);

	vector < Line > lines = image.getApproximateLines(edges);
	Mat enddest(matImage.clone());
	for (size_t i = 0; i < lines.size(); i++) {
		Line line = lines.at(i);
		line.drawing(enddest);
	}

	qImage = ImageConvert::cvMatToQImage(enddest);
	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	statusBar()->showMessage(
			tr("Threshold value: ") + QString::number(thresholdValue));
}
void ImageViewer::edgeSegmentation_Method_Changed(QString filePath,
		impls_2015::Image::SegmentMethod sgmethod) {

	Image image(filePath);
	int tvalue = 0;
	vector < Line > lines = image.lineSegment(sgmethod, tvalue);
	Mat enddest(matImage.clone());
	Scenario::edgeSegmentation(image, enddest, sgmethod);
	qImage = ImageConvert::cvMatToQImage(enddest);
	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	statusBar()->showMessage(tr("Threshold value: ") + QString::number(tvalue));
}
/*
 * Compute the pairwise geometric histogram of an image
 */
void ImageViewer::pairwiseHistogram() {
	qDebug() << "Calculate the pairwise geometric histogram of an image";
	map<string, int> resources = ReadResouces::readResources(
			"data/resources/pgh.rc");
	int angleAcc = resources["PGHAngleAcc"];
	int distanceAcc = resources["PGHDistanceAcc"];

	Image image(fileName);
	cv::Mat enddest;

	Image::SegmentMethod sgmethod = chooseSegMethod();

	vector < vector<int> > matrix;
	Scenario::pairwiseHistogram(image, (LocalHistogram::AccuracyPGH) angleAcc,
			distanceAcc, sgmethod, enddest, matrix);

	int save = saveOrNot();
	if (save == 1) {
		QMessageBox msgbox;
		msgbox.setText("Choose the folder to save the result");
		msgbox.exec();
		QString folderPath = QFileDialog::getExistingDirectory(this);
		QString savePath = folderPath + "/Matrix_" + image.getName() + ".TPS";
		qDebug() << savePath;
		ShapeHistogram shapehist;
		shapehist.writeMatrix(matrix, savePath);
	}
	/*
	 * Working on directory (compute PGH on set of image in a folder and save the result into PGH file)
	 */

	/*
	 QMessageBox msgbox;
	 msgbox.setText("Select the images folder.");
	 msgbox.exec();
	 QString folderPath = QFileDialog::getExistingDirectory(this);
	 Scenario::pairwiseHistogramDirectory(folderPath,LocalHistogram::Degree,250);
	 */

	/*
	 * Working on an image
	 */
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(enddest),
			"Pairwise histogram");
	other->show();
	qDebug() << "Done";
}

/*
 * Compute the PGH measure distance between two images by Bhattacharyya method
 */
void ImageViewer::pwBhattacharyyaMatching() {
	qDebug() << "Pairwise histogram matching using Bhattacharyya metric...";
	map<string, int> resources = ReadResouces::readResources(
			"data/resources/pgh.rc");
	int angleAcc = resources["PGHAngleAcc"];
	int distanceAcc = resources["PGHDistanceAcc"];

	Image image(fileName);

	/*
	 * Working on directory
	 */
	/*
	 QMessageBox msgbox;
	 msgbox.setText("Select the images folder.");
	 msgbox.exec();
	 QString folderPath = QFileDialog::getExistingDirectory(this);
	 matchingDirectory(image, folderPath);*/

	/*
	 * Working on an image
	 */
	QString fileName2 = QFileDialog::getOpenFileName(this);
	if (fileName2.isEmpty())
		return;
	qDebug() << fileName2;
	Image image2(fileName2);
	Image::SegmentMethod sgmethod = chooseSegMethod();
	double matching = Scenario::pghMatching(image, image2,
			GeometricHistogram::Bhattacharyya,
			(LocalHistogram::AccuracyPGH) angleAcc, distanceAcc, sgmethod);
	qDebug() << "Matching Bhattacharyya metric: "
			<< QString::number(matching, 'f', 20);
	qDebug() << "Done";
}

/*
 * Compute the PGH measure distance between two images by Chi-squared method
 */
void ImageViewer::pwChiSquaredMatching() {
	qDebug() << "Pairwise histogram matching using Chi-squared metric...";
	map<string, int> resources = ReadResouces::readResources(
			"data/resources/pgh.rc");
	int angleAcc = resources["PGHAngleAcc"];
	int distanceAcc = resources["PGHDistanceAcc"];
	Image image(fileName);
	QString fileName2 = QFileDialog::getOpenFileName(this);
	if (fileName2.isEmpty())
		return;
	qDebug() << fileName2;
	Image image2(fileName2);
	Image::SegmentMethod sgmethod = chooseSegMethod();
	double matching = Scenario::pghMatching(image, image2,
			GeometricHistogram::Chisquared,
			(LocalHistogram::AccuracyPGH) angleAcc, distanceAcc, sgmethod);
	qDebug() << "Chi-squared metric: " << QString::number(matching, 'f', 20);
	qDebug() << "Done";
}

/*
 * Compute the PGH measure distance between two images by Intersection method
 */
void ImageViewer::pwIntersectionMatching() {
	qDebug() << "Pairwise histogram matching using Intersection metric...";
	map<string, int> resources = ReadResouces::readResources(
			"data/resources/pgh.rc");
	int angleAcc = resources["PGHAngleAcc"];
	int distanceAcc = resources["PGHDistanceAcc"];
	Image image(fileName);
	QString fileName2 = QFileDialog::getOpenFileName(this);
	if (fileName2.isEmpty())
		return;
	qDebug() << fileName2;
	Image image2(fileName2);
	Image::SegmentMethod sgmethod = chooseSegMethod();
	double matching = Scenario::pghMatching(image, image2,
			GeometricHistogram::Intersection,
			(LocalHistogram::AccuracyPGH) angleAcc, distanceAcc, sgmethod);
	qDebug() << "Intersection metric: " << QString::number(matching, 'f', 20);

	qDebug() << "Done";
}

/*
 * Estimate the landmark on scene image based on a model
 */
void ImageViewer::pHoughTransform() {
	qDebug() << "Probabilistic Hough Transform...";

	Image image(fileName);
	QMessageBox msgbox;

	/*
	 * Working on an image
	 */
	msgbox.setText("Select the model image.");
	msgbox.exec();
	QString fileName2 = QFileDialog::getOpenFileName(this);
	if (fileName2.isEmpty())
		return;
	qDebug() << fileName2;
	Image image2(fileName2);

	msgbox.setText("Select the landmark file of model image.");
	msgbox.exec();
	QString reflmPath = QFileDialog::getOpenFileName(this);

	Image::SegmentMethod sgmethod = chooseSegMethod();

	if (!checkPresent(image, image2, sgmethod)) {
		msgbox.setText(
				"Sorry, we can not detect the similar between two images.");
		msgbox.exec();
		return;
	}

	vector < Point > esLandmarks;
	Mat enddest = Scenario::probabilisticHoughTransform(image2, image,
			reflmPath, sgmethod, esLandmarks);

	int save = saveOrNot();
	if (save == 1) {
		msgbox.setText("Choose the folder to save the result");
		msgbox.exec();
		QString folderPath = QFileDialog::getExistingDirectory(this);
		QString savePath = folderPath + "/E" + image2.getName() + ".TPS";
		PHoughTransform pht;
		pht.saveEstLandmarks(esLandmarks, savePath);

	}
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(enddest),
			"Probabilistic Hough Transform");
	other->matImage = enddest;
	other->fileName = fileName2;
	other->setContextMenuPolicy(Qt::CustomContextMenu);
	other->esLandmarks = esLandmarks;
	connect(other, SIGNAL(customContextMenuRequested(const QPoint&)), other,
			SLOT(phtContextMenu(const QPoint&)));
	other->show();
	qDebug() << "Done";
}

void ImageViewer::phtContextMenu(const QPoint& pos) {
	QPoint globalpos = this->mapToGlobal(pos);
	QMenu menu(this);
	//QMenu *putIn = menu.addMenu("Put in");
	//QMenu *putOut = menu.addMenu("Put out");

	QAction *act1 = new QAction("Load original landmarks", this);
	connect(act1, SIGNAL(triggered()), this, SLOT(putOrgLandmarks()));
	menu.addAction(act1);

	QAction *act2 = new QAction("Save estimated landmarks to file", this);
	connect(act2, SIGNAL(triggered()), this, SLOT(savePHTToFile()));
	menu.addAction(act2);

	menu.exec(globalpos);
}

void ImageViewer::putOrgLandmarks() {
	qDebug() << "Load context menu";
	Image sceneImage(this->fileName);

	QMessageBox msgbox;
	msgbox.setText("Select the original landmarks.");
	msgbox.exec();
	QString lpath = QFileDialog::getOpenFileName(this);
	vector<Point> orgLMs;
	Mat result = sceneImage.loadOriginalLandmarks(this->matImage, lpath,
			orgLMs);
	this->orgLandmarks = orgLMs;

	qImage = ImageConvert::cvMatToQImage(result);
	imageLabel->setPixmap(QPixmap::fromImage(qImage));
}
void ImageViewer::savePHTToFile() {
	QMessageBox msgbox;
	msgbox.setText("Choose the folder to save the result");
	msgbox.exec();
	QString folderPath = QFileDialog::getExistingDirectory(this);
	Image sceneImage(this->fileName);
	QString savePath = folderPath + "/E" + sceneImage.getName() + ".TPS";
	PHoughTransform pht;
	pht.saveEstLandmarks(this->esLandmarks, savePath);

}

bool ImageViewer::checkPresent(impls_2015::Image mImage,
		impls_2015::Image sImage, impls_2015::Image::SegmentMethod sgmethod) {
	map<string, int> resources = ReadResouces::readResources(
			"data/resources/pgh.rc");
	int angleAcc = resources["PGHAngleAcc"];
	int distanceAcc = resources["PGHDistanceAcc"];
	double agree = (double) resources["PGHAgree"] / 100;
	double mDistance = Scenario::pghMatching(mImage, sImage,
			GeometricHistogram::Bhattacharyya,
			(LocalHistogram::AccuracyPGH) angleAcc, distanceAcc, sgmethod);
	if (mDistance < agree) {
		return false;
	}
	return true;
}
/*
 * Identify the landmarks of an image base on cross-correlation
 */
void ImageViewer::crossCorrelation() {

	qDebug() << "Cross correlation";
	Image image(fileName);

	QMessageBox msgbox;

	msgbox.setText("Select the landmark file of model image.");
	msgbox.exec();
	QString lpath = QFileDialog::getOpenFileName(this);

	/*
	 * Working on directory
	 */
	/*qDebug() << "Landmarks detected by cross-correlation (in directory)";

	 msgbox.setText("Select the images folder.");
	 msgbox.exec();
	 QString folder = QFileDialog::getExistingDirectory(this);

	 msgbox.setText("Select the saving folder.");
	 msgbox.exec();
	 QString saveFolder = QFileDialog::getExistingDirectory(this);
	 Scenario::cCorelationDirectory(image, folder, saveFolder, lpath);*/

	/*
	 * Working on an image
	 */
	msgbox.setText("Select the model image.");
	msgbox.exec();
	QString fileName2 = QFileDialog::getOpenFileName(this);
	if (fileName2.isEmpty())
		return;
	Image sceneImage(fileName2);
	map<string, int> resources = ReadResouces::readResources(
			"data/resources/ccorrelation.rc");
	int templSize = resources["templSize"];
	vector < Point > landmarks = Scenario::landmarksByCrossCorelation(
			sceneImage, lpath, image, templSize);
	QString scenename = sceneImage.getName();
	qDebug() << scenename;

	Mat enddest(sceneImage.getMatrixImage().clone());
	for (size_t i = 0; i < landmarks.size(); i++) {
		Point lm = landmarks.at(i);
		circle(enddest, Point(lm.x, lm.y), 5, Scalar(0, 255, 255), 2, 4);
	}
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(enddest),
			"Landmark -- " + this->fileName);
	other->show();
	qDebug() << "Done";
}

/*
 * Compute the measure distance (centroid) of an image based on the landmarks,
 * which was estimated by cross-correlation method
 */
void ImageViewer::crossCorrelationDistance() {

	qDebug() << "Compute the measure distance by cross correlation";
	Image image(fileName);

	QMessageBox msgbox;

	msgbox.setText("Select the landmark file of model image.");
	msgbox.exec();
	QString lpath = QFileDialog::getOpenFileName(this);

	/*
	 * Working on directory
	 */
	/*qDebug() << "Landmarks detected by cross-correlation (in directory)";
	 msgbox.setText("Select the images folder.");
	 msgbox.exec();
	 QString folder = QFileDialog::getExistingDirectory(this);
	 Scenario::mDistanceByCrossCorrelationDir(image, lpath, folder);*/

	/*
	 * Working on an image
	 */
	msgbox.setText("Select the model image.");
	msgbox.exec();
	QString fileName2 = QFileDialog::getOpenFileName(this);
	if (fileName2.isEmpty())
		return;
	Image sceneImage(fileName2);
	Point ebary;
	map<string, int> resources = ReadResouces::readResources(
			"data/resources/ccorrelation.rc");
	double templSize = resources["templSize"];
	double eCentroid = Scenario::mDistanceByCrossCorrelation(sceneImage, lpath,
			image, templSize, ebary);
	qDebug() << "Ebary point: (" << ebary.x << ", " << ebary.y << ")";
	qDebug() << "Measure distance estimated: " << eCentroid;

	qDebug() << "Done";
}

/*
 * Compute the measure distance (centroid) of an image based on the landmarks,
 * which was estimated by template matching method (followed article)
 */
void ImageViewer::tplMatchingDistance() {
	qDebug() << "Measure distance by article";

	Image image(fileName);

	QMessageBox msgbox;
	msgbox.setText("Select the landmark file of model image.");
	msgbox.exec();

	QString lpath = QFileDialog::getOpenFileName(this);

	/*
	 * Working on an image
	 */
	msgbox.setText("Select the model image.");
	msgbox.exec();

	QString fileName2 = QFileDialog::getOpenFileName(this);
	if (fileName2.isEmpty())
		return;
	Image sceneImage(fileName2);
	Point ebary;
	impls_2015::Image::SegmentMethod sgmethod = chooseSegMethod();
	map<string, int> resources = ReadResouces::readResources(
			"data/resources/est.rc");
	int templSize = resources["EstTemplSize"];
	int imageSize = resources["EstImageSize"];
	double eCentroid = Scenario::mDistanceByTemplateMatching(sceneImage, image,
			lpath, templSize, imageSize, sgmethod, ebary);
	qDebug() << "Ebary point: (" << ebary.x << ", " << ebary.y << ")";
	qDebug() << "Size estimated: " << eCentroid;
	qDebug() << "Done";
}

// working on directory

impls_2015::Image::SegmentMethod ImageViewer::chooseSegMethod() {
	QMessageBox choicebox;
	choicebox.setText("Choose the segmentation method ?");
	QRadioButton *otsuRadio = new QRadioButton("Otsu method", this);
	choicebox.addButton(otsuRadio, QMessageBox::AcceptRole);

	QRadioButton *otherRadio = new QRadioButton("Other method", this);
	otherRadio->setChecked(true);
	choicebox.addButton(otherRadio, QMessageBox::RejectRole);

	int method = choicebox.exec();
	Image::SegmentMethod sgmethod;
	switch (method) {
	case QMessageBox::AcceptRole: //0
		//qDebug() << "Using Otsu threshold value to segment.";
		sgmethod = Image::Otsu;
		break;
	case QMessageBox::RejectRole: //1
		//qDebug() << "Using another threshold value to segment.";
		sgmethod = Image::Other;
		break;
	default:
		//qDebug() << "Using another threshold value to segment.";
		sgmethod = Image::Other;
		break;
	}
	return sgmethod;
}
int ImageViewer::saveOrNot() {
	QMessageBox msgbox;
	msgbox.setText("Do you want save the results to file ?");
	msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgbox.setDefaultButton(QMessageBox::No);
	int save = msgbox.exec();
	switch (save) {
	case QMessageBox::Yes:
		//qDebug() << "Save the results to file";
		save = 1;
		break;
	case QMessageBox::No:
		//qDebug() << "Do not save the results to file";
		save = 0;
		break;
	default:
		//qDebug() << "Do not save the results to file (default)";
		save = 0;
		break;
	}
	return save;
}

void ImageViewer::loadOriginalLandmarks() {
	Image image(fileName);
	QMessageBox msgbox;

	msgbox.setText("Select the landmark file.");
	msgbox.exec();
	QString reflmPath = QFileDialog::getOpenFileName(this);

	vector < Point > orgLM;
	Mat enddest = Scenario::loadOriginalLandmarks(image, reflmPath, orgLM);
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ImageConvert::cvMatToQImage(enddest),
			"Landmark -- " + this->fileName);
	other->show();
	qDebug() << "Done";
}
void ImageViewer::edgeSegmentDirectory() {
	qDebug() << "Edge segmentation on directory";
	QMessageBox msgbox;

	msgbox.setText("Select the images folder.");
	msgbox.exec();
	QString folderInput = QFileDialog::getExistingDirectory(this);

	msgbox.setText("Select the folder to save the result.");
	msgbox.exec();
	QString folderOutput = QFileDialog::getExistingDirectory(this);

	impls_2015::Image::SegmentMethod sgmethod = chooseSegMethod();
	int save = saveOrNot();

	Scenario::edgeSegmentationDirectory(folderInput, folderOutput, sgmethod,
			save);
	qDebug() << "Done";
}
void ImageViewer::bhattacharyyaDistanceDirectory() {
	qDebug() << "Compare PGH matching using Bhattacharyya metric on directory";
	Image image(fileName);

	/*
	 * Working on directory
	 */

	QMessageBox msgbox;
	msgbox.setText("Select the images folder.");
	msgbox.exec();
	QString folderPath = QFileDialog::getExistingDirectory(this);
	msgbox.setText("Select the save folder.");
	msgbox.exec();
	QString saveDir = QFileDialog::getExistingDirectory(this);
	QString fileSave = saveDir + "/BhattacharyyaDistance" + Image::getTimeName()
			+ ".txt";
	Image::SegmentMethod sgmethod = chooseSegMethod();
	matchingDirectory(image, folderPath, fileSave, sgmethod);
	qDebug() << "Done";
}
void ImageViewer::phtOnDirectory() {
	qDebug() << "Probabilistic Hough Transform on directory";
	Image image(fileName);
	QMessageBox msgbox;

	msgbox.setText("Select the landmark file of model image.");
	msgbox.exec();
	QString reflmPath = QFileDialog::getOpenFileName(this);

	msgbox.setText("Select the scene images folder.");
	msgbox.exec();
	QString sceneImageDir = QFileDialog::getExistingDirectory(this);

	msgbox.setText("Select the scene landmarks folder.");
	msgbox.exec();
	QString sceneLMDir = QFileDialog::getExistingDirectory(this);

	msgbox.setText("Select the save folder.");
	msgbox.exec();
	QString saveDir = QFileDialog::getExistingDirectory(this);

	impls_2015::Image::SegmentMethod sgmethod = chooseSegMethod();
	int save = saveOrNot();

	Scenario::phtDirectory(image, reflmPath, sceneImageDir, sceneLMDir, saveDir,
			sgmethod, save);
	qDebug() << "Done";
}
void ImageViewer::estlmOnDirectory() {
	qDebug() << "Estimated landmarks on directory";
	Image image(fileName);
	QMessageBox msgbox;

	msgbox.setText("Select the landmark file of model image.");
	msgbox.exec();

	QString lpath = QFileDialog::getOpenFileName(this);

	/*
	 * Working on directory
	 */

	msgbox.setText("Select the scene images folder.");
	msgbox.exec();
	QString folder = QFileDialog::getExistingDirectory(this);

	/*msgbox.setText("Select the scene landarks folder.");
	 msgbox.exec();
	 QString orgLMPath = QFileDialog::getExistingDirectory(this);*/

	msgbox.setText("Select the saving folder.");
	msgbox.exec();
	QString saveFolder = QFileDialog::getExistingDirectory(this);

	impls_2015::Image::SegmentMethod sgmethod = chooseSegMethod();
	int save = saveOrNot();
	map<string, int> resources = ReadResouces::readResources(
			"data/resources/est.rc");
	int templSize = resources["EstTemplSize"];
	int imageSize = resources["EstImageSize"];
	Scenario::landmarksMatchingDirectory(image, folder, lpath, saveFolder,
			templSize, imageSize, sgmethod, save);
	qDebug() << "Done";
}
void ImageViewer::computeSizeOnDirectory() {
	qDebug() << "Compute the original size and the estimated size on directory";
	Image image(fileName);
	QMessageBox msgbox;

	msgbox.setText("Select the landmark file of model image.");
	msgbox.exec();

	QString lpath = QFileDialog::getOpenFileName(this);
	/*
	 * Working on directory
	 */
	msgbox.setText("Select the images folder.");
	msgbox.exec();
	QString folder = QFileDialog::getExistingDirectory(this);

	msgbox.setText("Select the scene original landmarks folder.");
	msgbox.exec();
	QString lmFolder = QFileDialog::getExistingDirectory(this);

	msgbox.setText("Select the saving folder.");
	msgbox.exec();
	QString saveFolder = QFileDialog::getExistingDirectory(this);

	impls_2015::Image::SegmentMethod sgmethod = chooseSegMethod();

	map<string, int> resources = ReadResouces::readResources(
			"data/resources/est.rc");
	int templSize = resources["EstTemplSize"];
	int imageSize = resources["EstImageSize"];
	Scenario::mDistanceByTemplateMatchingDirectory(image, lpath, folder,
			lmFolder, templSize, imageSize, saveFolder, sgmethod);

	qDebug() << "Done";
}
