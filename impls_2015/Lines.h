/*
 * Lines.h
 *
 *  Created on: Jun 24, 2015
 *      Author: linh
 */

#ifndef LINES_H_
#define LINES_H_

#include <QtGui/QDialog>
#include <QtGui/QWidget>
#include <QtGui/QSlider>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

namespace impls_2015 {

class Lines: public QDialog {
Q_OBJECT
public:
	Lines(QWidget *parent);
	static const int SLIDER_VALUE = 90;
	static const int BRIGHTNESS_VALUE = 90;
private slots:
	void on_sliderEle_valueChanged(int value);
	void on_minBrightness_valueChanged(int value);
private:
	QSlider *sliderEle;
	QSlider *minBrightness;
};

} /* namespace impls_2015 */
#endif /* LINES_H_ */
