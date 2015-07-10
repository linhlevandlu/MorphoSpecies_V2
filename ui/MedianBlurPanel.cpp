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

#include "MedianBlurPanel.h"

#include <QtCore/QDebug>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>

#include "../ImageViewer.h"

namespace ui {

void MedianBlurPanel::on_sliKernelSize_valueChanged(int value){
	ImageViewer* parent = qobject_cast<ImageViewer *>(this->parentWidget());
	parent->medianBlurAction(value*2+1);
}


MedianBlurPanel::MedianBlurPanel(QWidget *parent)
     : QDialog(parent)
 {
	sliKernelSize = new QSlider(Qt::Horizontal, this);
	sliKernelSize->setRange(0,15);
	sliKernelSize->setValue(DF_KERNEL_SIZE);

	connect(sliKernelSize, SIGNAL(valueChanged(int)), this, SLOT(on_sliKernelSize_valueChanged(int)));

	QFormLayout *layout = new QFormLayout;
	layout->addRow(new QLabel(tr("Kernel size: ")), sliKernelSize);

	setLayout(layout);
	setWindowTitle(tr("Parameters"));

 }

} /* namespace ui */
