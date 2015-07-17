/*
 * Lines.cpp
 *
 *  Created on: Jun 24, 2015
 *      Author: linh
 */

#include "Lines.h"
#include <QtCore/qdebug.h>
#include <QtCore/QDebug>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>
#include <QtGui/qslider.h>
#include "../ImageViewer.h"

namespace impls_2015 {

Lines::Lines(QWidget *parent) :
		QDialog(parent) {

	sliderEle = new QSlider(Qt::Horizontal, this);
	sliderEle->setRange(0, 190);
	sliderEle->setSingleStep(1);
	sliderEle->setValue(SLIDER_VALUE);
	connect(sliderEle, SIGNAL(valueChanged(int)), this,
			SLOT(on_sliderEle_valueChanged(int)));

	minBrightness = new QSlider(Qt::Horizontal, this);
	minBrightness->setRange(50, 250);
	minBrightness->setSingleStep(1);
	minBrightness->setValue(BRIGHTNESS_VALUE);
	connect(minBrightness, SIGNAL(valueChanged(int)), this,
			SLOT(on_minBrightness_valueChanged(int)));

	QFormLayout *layout = new QFormLayout;
	layout->addRow(new QLabel(tr("Minimum Brightness: ")), minBrightness);
	layout->addRow(new QLabel(tr("Gray synchronize: ")), sliderEle);

	setLayout(layout);
	setWindowTitle(tr("Parameters"));
	this->setAttribute(Qt::WA_QuitOnClose, false);
}

void Lines::on_minBrightness_valueChanged(int value) {
	ImageViewer* parent = qobject_cast<ImageViewer *>(this->parentWidget());
	//parent->removeYLinesAction(value,"abc");
}
void Lines::on_sliderEle_valueChanged(int value) {
	ImageViewer* parent = qobject_cast<ImageViewer *>(this->parentWidget());
	//parent->removeYLinesAction("abc", value);
}
} /* namespace impls_2015 */
