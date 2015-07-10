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

#include "SurfPanel.h"

#include <QtCore/QDebug>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>

#include "../ImageViewer.h"

namespace ui {

void SurfPanel::on_sliMin_valueChanged(int value){
	ImageViewer* parent = qobject_cast<ImageViewer *>(this->parentWidget());
	parent->featureSurfAction(value);
}


SurfPanel::SurfPanel(QWidget *parent)
     : QDialog(parent)
 {
	sliMin = new QSlider(Qt::Horizontal, this);
	sliMin->setRange(100,5000);
	sliMin->setValue(DF_MIN_HESSIAN);
	sliMin->setSingleStep(50);

	connect(sliMin, SIGNAL(valueChanged(int)), this, SLOT(on_sliMin_valueChanged(int)));

	QFormLayout *layout = new QFormLayout;
	layout->addRow(new QLabel(tr("Min hessian: ")), sliMin);

	setLayout(layout);
	setWindowTitle(tr("Parameters"));

 }

} /* namespace ui */
