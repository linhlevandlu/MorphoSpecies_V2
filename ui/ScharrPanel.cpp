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

#include "ScharrPanel.h"

#include <QtCore/QDebug>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>

#include "../ImageViewer.h"

namespace ui {


void ScharrPanel::on_sliScale_valueChanged(int value){
	ImageViewer* parent = qobject_cast<ImageViewer *>(this->parentWidget());
	parent->scharrAction(value);
}


ScharrPanel::ScharrPanel(QWidget *parent)
     : QDialog(parent)
 {

	sliScale = new QSlider(Qt::Horizontal, this);
	sliScale->setRange(1,10);
	sliScale->setValue(DF_SCALE);

	connect(sliScale, SIGNAL(valueChanged(int)), this, SLOT(on_sliScale_valueChanged(int)));

	QFormLayout *layout = new QFormLayout;
	layout->addRow(new QLabel(tr("Scale: ")), sliScale);

	setLayout(layout);
	setWindowTitle(tr("Parameters"));

 }

} /* namespace ui */
