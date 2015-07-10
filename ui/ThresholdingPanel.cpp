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

#include "ThresholdingPanel.h"

#include <QDebug>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>

#include "../ImageViewer.h"

namespace ui {

void ThresholdingPanel::on_sliThresholdType_valueChanged(int value){
	ImageViewer* parent = qobject_cast<ImageViewer *>(this->parentWidget());
	parent->thresholdAction(sliThresholdValue->value(),value);
}

void ThresholdingPanel::on_sliThresholdValue_valueChanged(int value){
	ImageViewer* parent = qobject_cast<ImageViewer *>(this->parentWidget());
	parent->thresholdAction(value, sliThresholdType->value());
}


ThresholdingPanel::ThresholdingPanel(QWidget *parent)
     : QDialog(parent)
 {
	sliThresholdType = new QSlider(Qt::Horizontal, this);
	sliThresholdType->setRange(0,4);
	sliThresholdType->setValue(DF_TYPE);

	sliThresholdValue = new QSlider(Qt::Horizontal, this);
	sliThresholdValue->setRange(0,255);
	sliThresholdValue->setValue(DF_VALUE);


	connect(sliThresholdType, SIGNAL(valueChanged(int)), this, SLOT(on_sliThresholdType_valueChanged(int)));
	connect(sliThresholdValue, SIGNAL(valueChanged(int)), this, SLOT(on_sliThresholdValue_valueChanged(int)));


	QFormLayout *layout = new QFormLayout;
	layout->addRow(new QLabel(tr("Threshold type: ")), sliThresholdType);
	layout->addRow(new QLabel(tr("Threshold value: ")), sliThresholdValue);
	layout->addRow(new QLabel(""), new QLabel("0: Binary\n" \
	 "1: Binary Inverted\n" \
	 "2: Threshold Truncated\n" \
	 "3: Threshold to Zero\n" \
 	 "4: Threshold to Zero Inverted\n"));

	setLayout(layout);
	setWindowTitle(tr("Parameters"));

 }
} /* namespace ui */
