######################################################################
# Automatically generated by qmake (2.01a) Wed Mar 12 21:49:44 2014
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += "/usr/include"
LIBS += -L"/usr/lib" -lOpenCL \
		-L"/usr/lib" -lopencv_core -lopencv_imgproc \
													-lopencv_highgui -lopencv_ml \
													-lopencv_video -lopencv_features2d \
													-lopencv_calib3d -lopencv_objdetect \
													-lopencv_contrib -lopencv_legacy \
													-lopencv_flann -lopencv_nonfree
	

QT           += opengl
CONFIG       += console
RESOURCES     = Ipm.qrc

# Input
HEADERS += ImageViewer.h ImageConvert.h ui/NormalBlurPanel.h \
	ui/GaussianBlurPanel.h ui/MedianBlurPanel.h ui/BilateralFilterPanel.h ui/ConvolutionFilterPanel.h\
	ui/ErosionPanel.h ui/DilationPanel.h ui/MorphoExtPanel.h \
	ui/ThresholdingPanel.h ui/SobelPanel.h ui/ScharrPanel.h ui/LaplacePanel.h ui/CannyPanel.h \
	impls/algorithms/MyImpl.h impls/algorithms/Ant.h impls/algorithms/Position.h \
	ui/SurfPanel.h impls_2015/Line.h \
	impls_2015/IDrawOperation.h impls_2015/Edge.h impls_2015/IExtraction.h \
	impls_2015/Landmark.h  impls_2015/Scenario.h impls_2015/ShapeHistogram.h \
	impls_2015/GFeatures.h impls_2015/LocalHistogram.h impls_2015/Image.h \
	impls_2015/histograms/HistogramMethod.h impls_2015/histograms/GeometricHistogram.h \
	impls_2015/landmarks/LandmarkMethod.h impls_2015/landmarks/LandmarkDetection.h \
	impls_2015/segmentation/SegmentMethod.h impls_2015/segmentation/EdgeSegmentation.h \
	impls_2015/pht/PHoughTransform.h impls_2015/pht/HoughMethod.h impls_2015/pht/HoughSpace.h \
	impls_2015/pht/PHTEntry.h impls_2015/pht/TableEntry.h
	
SOURCES += Ipm.cpp ImageViewer.cpp ImageConvert.cpp ui/NormalBlurPanel.cpp \
	ui/GaussianBlurPanel.cpp ui/MedianBlurPanel.cpp ui/BilateralFilterPanel.cpp ui/ConvolutionFilterPanel.cpp\
	ui/ErosionPanel.cpp ui/DilationPanel.cpp ui/MorphoExtPanel.cpp \
	ui/ThresholdingPanel.cpp ui/SobelPanel.cpp ui/ScharrPanel.cpp ui/LaplacePanel.cpp ui/CannyPanel.cpp \
	impls/algorithms/MyImpl.cpp impls/algorithms/Ant.cpp impls/algorithms/Position.cpp \
	ui/SurfPanel.cpp impls_2015/Line.cpp \
	impls_2015/IDrawOperation.cpp impls_2015/Edge.cpp impls_2015/IExtraction.cpp \
	impls_2015/Landmark.cpp impls_2015/Scenario.cpp impls_2015/ShapeHistogram.cpp \
	impls_2015/GFeatures.cpp impls_2015/LocalHistogram.cpp impls_2015/Image.cpp \
	impls_2015/histograms/HistogramMethod.cpp impls_2015/histograms/GeometricHistogram.cpp \
	impls_2015/landmarks/LandmarkMethod.cpp impls_2015/landmarks/LandmarkDetection.cpp \
	impls_2015/segmentation/SegmentMethod.cpp impls_2015/segmentation/EdgeSegmentation.cpp \
	impls_2015/pht/PHoughTransform.cpp  impls_2015/pht/HoughMethod.cpp impls_2015/pht/HoughSpace.cpp \
	impls_2015/pht/PHTEntry.cpp impls_2015/pht/TableEntry.cpp
