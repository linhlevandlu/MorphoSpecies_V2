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

#include "ErosionPanel.h"

#include <QtCore/QDebug>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>

#include "../ImageViewer.h"

namespace ui {

void ErosionPanel::on_sliKernelSize_valueChanged(int value){
	ImageViewer* parent = qobject_cast<ImageViewer *>(this->parentWidget());
	parent->erosionAction(value*2+1,sliElement->value());
}

void ErosionPanel::on_sliElement_valueChanged(int value){
	ImageViewer* parent = qobject_cast<ImageViewer *>(this->parentWidget());
	parent->erosionAction(sliKernelSize->value()*2+1,value);
}


ErosionPanel::ErosionPanel(QWidget *parent)
     : QDialog(parent)
 {
	sliKernelSize = new QSlider(Qt::Horizontal, this);
	sliKernelSize->setRange(0,10);
	sliKernelSize->setValue(DF_KERNEL_SIZE);

	sliElement = new QSlider(Qt::Horizontal, this);
	sliElement->setRange(0,2);
	sliElement->setValue(DF_TYPE);

	connect(sliKernelSize, SIGNAL(valueChanged(int)), this, SLOT(on_sliKernelSize_valueChanged(int)));
	connect(sliElement, SIGNAL(valueChanged(int)), this, SLOT(on_sliElement_valueChanged(int)));

	QFormLayout *layout = new QFormLayout;
	layout->addRow(new QLabel(tr("Kernel size: ")), sliKernelSize);
	layout->addRow(new QLabel(tr("Element type: ")), sliElement);
	layout->addRow(new QLabel(""), new QLabel(tr("0: Rect - 1: Cross - 2: Ellipse")));

	setLayout(layout);
	setWindowTitle(tr("Parameters"));

 }

} /* namespace ui */
