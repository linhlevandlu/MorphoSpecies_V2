#############################################################################
# Makefile for building: Ipm
# Generated by qmake (2.01a) (Qt 4.8.6) on: ven. juil. 10 10:54:58 2015
# Project:  Ipm.pro
# Template: app
# Command: /usr/lib/x86_64-linux-gnu/qt4/bin/qmake -o Makefile Ipm.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -m64 -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -m64 -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++-64 -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtOpenGL -I/usr/include/qt4 -I/usr/include -I/usr/X11R6/include -I.
LINK          = g++
LFLAGS        = -m64 -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib/x86_64-linux-gnu -L/usr/X11R6/lib64 -L/usr/lib -lOpenCL -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann -lopencv_nonfree -lQtOpenGL -lQtGui -lQtCore -lGL -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/lib/x86_64-linux-gnu/qt4/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = Ipm.cpp \
		ImageViewer.cpp \
		ImageConvert.cpp \
		ui/NormalBlurPanel.cpp \
		ui/GaussianBlurPanel.cpp \
		ui/MedianBlurPanel.cpp \
		ui/BilateralFilterPanel.cpp \
		ui/ConvolutionFilterPanel.cpp \
		ui/ErosionPanel.cpp \
		ui/DilationPanel.cpp \
		ui/MorphoExtPanel.cpp \
		ui/ThresholdingPanel.cpp \
		ui/SobelPanel.cpp \
		ui/ScharrPanel.cpp \
		ui/LaplacePanel.cpp \
		ui/CannyPanel.cpp \
		impls/algorithms/MyImpl.cpp \
		impls/algorithms/ant.cpp \
		impls/algorithms/position.cpp \
		ui/SurfPanel.cpp \
		impls_2015/Lines.cpp \
		impls_2015/YellowGrid.cpp moc_ImageViewer.cpp \
		moc_NormalBlurPanel.cpp \
		moc_GaussianBlurPanel.cpp \
		moc_MedianBlurPanel.cpp \
		moc_BilateralFilterPanel.cpp \
		moc_ConvolutionFilterPanel.cpp \
		moc_ErosionPanel.cpp \
		moc_DilationPanel.cpp \
		moc_MorphoExtPanel.cpp \
		moc_ThresholdingPanel.cpp \
		moc_SobelPanel.cpp \
		moc_ScharrPanel.cpp \
		moc_LaplacePanel.cpp \
		moc_CannyPanel.cpp \
		moc_SurfPanel.cpp \
		moc_Lines.cpp \
		qrc_Ipm.cpp
OBJECTS       = Ipm.o \
		ImageViewer.o \
		ImageConvert.o \
		NormalBlurPanel.o \
		GaussianBlurPanel.o \
		MedianBlurPanel.o \
		BilateralFilterPanel.o \
		ConvolutionFilterPanel.o \
		ErosionPanel.o \
		DilationPanel.o \
		MorphoExtPanel.o \
		ThresholdingPanel.o \
		SobelPanel.o \
		ScharrPanel.o \
		LaplacePanel.o \
		CannyPanel.o \
		MyImpl.o \
		ant.o \
		position.o \
		SurfPanel.o \
		Lines.o \
		YellowGrid.o \
		moc_ImageViewer.o \
		moc_NormalBlurPanel.o \
		moc_GaussianBlurPanel.o \
		moc_MedianBlurPanel.o \
		moc_BilateralFilterPanel.o \
		moc_ConvolutionFilterPanel.o \
		moc_ErosionPanel.o \
		moc_DilationPanel.o \
		moc_MorphoExtPanel.o \
		moc_ThresholdingPanel.o \
		moc_SobelPanel.o \
		moc_ScharrPanel.o \
		moc_LaplacePanel.o \
		moc_CannyPanel.o \
		moc_SurfPanel.o \
		moc_Lines.o \
		qrc_Ipm.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/shared.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/opengl.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		Ipm.pro
QMAKE_TARGET  = Ipm
DESTDIR       = 
TARGET        = Ipm

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: Ipm.pro  /usr/share/qt4/mkspecs/linux-g++-64/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/shared.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/opengl.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/x86_64-linux-gnu/libQtOpenGL.prl \
		/usr/lib/x86_64-linux-gnu/libQtGui.prl \
		/usr/lib/x86_64-linux-gnu/libQtCore.prl
	$(QMAKE) -o Makefile Ipm.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/shared.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/opengl.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/x86_64-linux-gnu/libQtOpenGL.prl:
/usr/lib/x86_64-linux-gnu/libQtGui.prl:
/usr/lib/x86_64-linux-gnu/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile Ipm.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/Ipm1.0.0 || $(MKDIR) .tmp/Ipm1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/Ipm1.0.0/ && $(COPY_FILE) --parents ImageViewer.h ImageConvert.h ui/NormalBlurPanel.h ui/GaussianBlurPanel.h ui/MedianBlurPanel.h ui/BilateralFilterPanel.h ui/ConvolutionFilterPanel.h ui/ErosionPanel.h ui/DilationPanel.h ui/MorphoExtPanel.h ui/ThresholdingPanel.h ui/SobelPanel.h ui/ScharrPanel.h ui/LaplacePanel.h ui/CannyPanel.h impls/algorithms/MyImpl.h impls/algorithms/ant.h impls/algorithms/position.h ui/SurfPanel.h impls_2015/Lines.h impls_2015/YellowGrid.h .tmp/Ipm1.0.0/ && $(COPY_FILE) --parents Ipm.qrc .tmp/Ipm1.0.0/ && $(COPY_FILE) --parents Ipm.cpp ImageViewer.cpp ImageConvert.cpp ui/NormalBlurPanel.cpp ui/GaussianBlurPanel.cpp ui/MedianBlurPanel.cpp ui/BilateralFilterPanel.cpp ui/ConvolutionFilterPanel.cpp ui/ErosionPanel.cpp ui/DilationPanel.cpp ui/MorphoExtPanel.cpp ui/ThresholdingPanel.cpp ui/SobelPanel.cpp ui/ScharrPanel.cpp ui/LaplacePanel.cpp ui/CannyPanel.cpp impls/algorithms/MyImpl.cpp impls/algorithms/ant.cpp impls/algorithms/position.cpp ui/SurfPanel.cpp impls_2015/Lines.cpp impls_2015/YellowGrid.cpp .tmp/Ipm1.0.0/ && (cd `dirname .tmp/Ipm1.0.0` && $(TAR) Ipm1.0.0.tar Ipm1.0.0 && $(COMPRESS) Ipm1.0.0.tar) && $(MOVE) `dirname .tmp/Ipm1.0.0`/Ipm1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/Ipm1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_ImageViewer.cpp moc_NormalBlurPanel.cpp moc_GaussianBlurPanel.cpp moc_MedianBlurPanel.cpp moc_BilateralFilterPanel.cpp moc_ConvolutionFilterPanel.cpp moc_ErosionPanel.cpp moc_DilationPanel.cpp moc_MorphoExtPanel.cpp moc_ThresholdingPanel.cpp moc_SobelPanel.cpp moc_ScharrPanel.cpp moc_LaplacePanel.cpp moc_CannyPanel.cpp moc_SurfPanel.cpp moc_Lines.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_ImageViewer.cpp moc_NormalBlurPanel.cpp moc_GaussianBlurPanel.cpp moc_MedianBlurPanel.cpp moc_BilateralFilterPanel.cpp moc_ConvolutionFilterPanel.cpp moc_ErosionPanel.cpp moc_DilationPanel.cpp moc_MorphoExtPanel.cpp moc_ThresholdingPanel.cpp moc_SobelPanel.cpp moc_ScharrPanel.cpp moc_LaplacePanel.cpp moc_CannyPanel.cpp moc_SurfPanel.cpp moc_Lines.cpp
moc_ImageViewer.cpp: ImageViewer.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) ImageViewer.h -o moc_ImageViewer.cpp

moc_NormalBlurPanel.cpp: ui/NormalBlurPanel.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) ui/NormalBlurPanel.h -o moc_NormalBlurPanel.cpp

moc_GaussianBlurPanel.cpp: ui/GaussianBlurPanel.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) ui/GaussianBlurPanel.h -o moc_GaussianBlurPanel.cpp

moc_MedianBlurPanel.cpp: ui/MedianBlurPanel.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) ui/MedianBlurPanel.h -o moc_MedianBlurPanel.cpp

moc_BilateralFilterPanel.cpp: ui/BilateralFilterPanel.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) ui/BilateralFilterPanel.h -o moc_BilateralFilterPanel.cpp

moc_ConvolutionFilterPanel.cpp: ui/ConvolutionFilterPanel.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) ui/ConvolutionFilterPanel.h -o moc_ConvolutionFilterPanel.cpp

moc_ErosionPanel.cpp: ui/ErosionPanel.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) ui/ErosionPanel.h -o moc_ErosionPanel.cpp

moc_DilationPanel.cpp: ui/DilationPanel.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) ui/DilationPanel.h -o moc_DilationPanel.cpp

moc_MorphoExtPanel.cpp: ui/MorphoExtPanel.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) ui/MorphoExtPanel.h -o moc_MorphoExtPanel.cpp

moc_ThresholdingPanel.cpp: ui/ThresholdingPanel.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) ui/ThresholdingPanel.h -o moc_ThresholdingPanel.cpp

moc_SobelPanel.cpp: ui/SobelPanel.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) ui/SobelPanel.h -o moc_SobelPanel.cpp

moc_ScharrPanel.cpp: ui/ScharrPanel.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) ui/ScharrPanel.h -o moc_ScharrPanel.cpp

moc_LaplacePanel.cpp: ui/LaplacePanel.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) ui/LaplacePanel.h -o moc_LaplacePanel.cpp

moc_CannyPanel.cpp: ui/CannyPanel.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) ui/CannyPanel.h -o moc_CannyPanel.cpp

moc_SurfPanel.cpp: ui/SurfPanel.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) ui/SurfPanel.h -o moc_SurfPanel.cpp

moc_Lines.cpp: impls_2015/Lines.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) impls_2015/Lines.h -o moc_Lines.cpp

compiler_rcc_make_all: qrc_Ipm.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_Ipm.cpp
qrc_Ipm.cpp: Ipm.qrc \
		images/new.png \
		images/copy.png \
		images/cut.png \
		images/save.png \
		images/1downarrow.png \
		images/1uparrow.png \
		images/paste.png \
		images/ip.ico \
		images/open.png
	/usr/lib/x86_64-linux-gnu/qt4/bin/rcc -name Ipm Ipm.qrc -o qrc_Ipm.cpp

compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_rcc_clean 

####### Compile

Ipm.o: Ipm.cpp ImageViewer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Ipm.o Ipm.cpp

ImageViewer.o: ImageViewer.cpp ImageViewer.h \
		ImageConvert.h \
		ui/NormalBlurPanel.h \
		ui/GaussianBlurPanel.h \
		ui/MedianBlurPanel.h \
		ui/BilateralFilterPanel.h \
		ui/ErosionPanel.h \
		ui/DilationPanel.h \
		ui/MorphoExtPanel.h \
		ui/ConvolutionFilterPanel.h \
		ui/ThresholdingPanel.h \
		ui/SobelPanel.h \
		ui/ScharrPanel.h \
		ui/LaplacePanel.h \
		ui/CannyPanel.h \
		ui/SurfPanel.h \
		impls/algorithms/MyImpl.h \
		impls/algorithms/ant.h \
		impls/algorithms/position.h \
		impls_2015/Lines.h \
		impls_2015/YellowGrid.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ImageViewer.o ImageViewer.cpp

ImageConvert.o: ImageConvert.cpp ImageConvert.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ImageConvert.o ImageConvert.cpp

NormalBlurPanel.o: ui/NormalBlurPanel.cpp ui/NormalBlurPanel.h \
		ImageViewer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o NormalBlurPanel.o ui/NormalBlurPanel.cpp

GaussianBlurPanel.o: ui/GaussianBlurPanel.cpp ui/GaussianBlurPanel.h \
		ImageViewer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o GaussianBlurPanel.o ui/GaussianBlurPanel.cpp

MedianBlurPanel.o: ui/MedianBlurPanel.cpp ui/MedianBlurPanel.h \
		ImageViewer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MedianBlurPanel.o ui/MedianBlurPanel.cpp

BilateralFilterPanel.o: ui/BilateralFilterPanel.cpp ui/BilateralFilterPanel.h \
		ImageViewer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o BilateralFilterPanel.o ui/BilateralFilterPanel.cpp

ConvolutionFilterPanel.o: ui/ConvolutionFilterPanel.cpp ui/ConvolutionFilterPanel.h \
		ImageViewer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ConvolutionFilterPanel.o ui/ConvolutionFilterPanel.cpp

ErosionPanel.o: ui/ErosionPanel.cpp ui/ErosionPanel.h \
		ImageViewer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ErosionPanel.o ui/ErosionPanel.cpp

DilationPanel.o: ui/DilationPanel.cpp ui/DilationPanel.h \
		ImageViewer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o DilationPanel.o ui/DilationPanel.cpp

MorphoExtPanel.o: ui/MorphoExtPanel.cpp ui/MorphoExtPanel.h \
		ImageViewer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MorphoExtPanel.o ui/MorphoExtPanel.cpp

ThresholdingPanel.o: ui/ThresholdingPanel.cpp ui/ThresholdingPanel.h \
		ImageViewer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ThresholdingPanel.o ui/ThresholdingPanel.cpp

SobelPanel.o: ui/SobelPanel.cpp ui/SobelPanel.h \
		ImageViewer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o SobelPanel.o ui/SobelPanel.cpp

ScharrPanel.o: ui/ScharrPanel.cpp ui/ScharrPanel.h \
		ImageViewer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ScharrPanel.o ui/ScharrPanel.cpp

LaplacePanel.o: ui/LaplacePanel.cpp ui/LaplacePanel.h \
		ImageViewer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o LaplacePanel.o ui/LaplacePanel.cpp

CannyPanel.o: ui/CannyPanel.cpp ui/CannyPanel.h \
		ImageViewer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o CannyPanel.o ui/CannyPanel.cpp

MyImpl.o: impls/algorithms/MyImpl.cpp impls/algorithms/MyImpl.h \
		impls/algorithms/ant.h \
		impls/algorithms/position.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MyImpl.o impls/algorithms/MyImpl.cpp

ant.o: impls/algorithms/ant.cpp impls/algorithms/ant.h \
		impls/algorithms/position.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ant.o impls/algorithms/ant.cpp

position.o: impls/algorithms/position.cpp impls/algorithms/position.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o position.o impls/algorithms/position.cpp

SurfPanel.o: ui/SurfPanel.cpp ui/SurfPanel.h \
		ImageViewer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o SurfPanel.o ui/SurfPanel.cpp

Lines.o: impls_2015/Lines.cpp impls_2015/Lines.h \
		ImageViewer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Lines.o impls_2015/Lines.cpp

YellowGrid.o: impls_2015/YellowGrid.cpp impls_2015/YellowGrid.h \
		ImageConvert.h \
		ImageViewer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o YellowGrid.o impls_2015/YellowGrid.cpp

moc_ImageViewer.o: moc_ImageViewer.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_ImageViewer.o moc_ImageViewer.cpp

moc_NormalBlurPanel.o: moc_NormalBlurPanel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_NormalBlurPanel.o moc_NormalBlurPanel.cpp

moc_GaussianBlurPanel.o: moc_GaussianBlurPanel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_GaussianBlurPanel.o moc_GaussianBlurPanel.cpp

moc_MedianBlurPanel.o: moc_MedianBlurPanel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_MedianBlurPanel.o moc_MedianBlurPanel.cpp

moc_BilateralFilterPanel.o: moc_BilateralFilterPanel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_BilateralFilterPanel.o moc_BilateralFilterPanel.cpp

moc_ConvolutionFilterPanel.o: moc_ConvolutionFilterPanel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_ConvolutionFilterPanel.o moc_ConvolutionFilterPanel.cpp

moc_ErosionPanel.o: moc_ErosionPanel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_ErosionPanel.o moc_ErosionPanel.cpp

moc_DilationPanel.o: moc_DilationPanel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_DilationPanel.o moc_DilationPanel.cpp

moc_MorphoExtPanel.o: moc_MorphoExtPanel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_MorphoExtPanel.o moc_MorphoExtPanel.cpp

moc_ThresholdingPanel.o: moc_ThresholdingPanel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_ThresholdingPanel.o moc_ThresholdingPanel.cpp

moc_SobelPanel.o: moc_SobelPanel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_SobelPanel.o moc_SobelPanel.cpp

moc_ScharrPanel.o: moc_ScharrPanel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_ScharrPanel.o moc_ScharrPanel.cpp

moc_LaplacePanel.o: moc_LaplacePanel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_LaplacePanel.o moc_LaplacePanel.cpp

moc_CannyPanel.o: moc_CannyPanel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_CannyPanel.o moc_CannyPanel.cpp

moc_SurfPanel.o: moc_SurfPanel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_SurfPanel.o moc_SurfPanel.cpp

moc_Lines.o: moc_Lines.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_Lines.o moc_Lines.cpp

qrc_Ipm.o: qrc_Ipm.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qrc_Ipm.o qrc_Ipm.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

