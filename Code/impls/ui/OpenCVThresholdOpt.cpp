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

//#include "OpenCVThresholdOpt.h"
//#include "mainwindow.h"
//
//#include <QDebug>
//#include <QtGui/QFormLayout>
//#include <QtGui/QLabel>
//
//namespace ui {
//
//void OpenCVThresholdOpt::on_sliThresholdType_valueChanged(int value){
////	qDebug() << "thresholdType " << value;
//	MainWindow *win = qobject_cast<MainWindow *>(this->parentWidget());
//	win->thresholdAction(value,sliThresholdValue->value());
//}
//
//void OpenCVThresholdOpt::on_sliThresholdValue_valueChanged(int value){
////	qDebug() << "thresholdValue " << value;
//	MainWindow *win = qobject_cast<MainWindow *>(this->parentWidget());
//	win->thresholdAction(sliThresholdType->value(),value);
//}
//
//
//OpenCVThresholdOpt::OpenCVThresholdOpt(QWidget *parent)
//     : QDialog(parent)
// {
//	sliThresholdType = new QSlider(Qt::Horizontal, this);
//	sliThresholdType->setMinimum(0);
//	sliThresholdType->setMaximum(4);
//	sliThresholdType->setValue(2);
//
//	sliThresholdValue = new QSlider(Qt::Horizontal, this);
//	sliThresholdValue->setMinimum(0);
//	sliThresholdValue->setMaximum(255);
//	sliThresholdValue->setValue(128);
//
//
//	connect(sliThresholdType, SIGNAL(valueChanged(int)), this, SLOT(on_sliThresholdType_valueChanged(int)));
//	connect(sliThresholdValue, SIGNAL(valueChanged(int)), this, SLOT(on_sliThresholdValue_valueChanged(int)));
//
//	lblType = new QLabel(tr("Threshold type: "));
//	lblValue = new QLabel(tr("Threshold value: "));
//
//	QFormLayout *layout = new QFormLayout;
//	layout->addRow(lblType, sliThresholdType);
//	layout->addRow(lblValue, sliThresholdValue);
//	layout->addRow(new QLabel(""), new QLabel("0: Binary\n" \
//	 "1: Binary Inverted\n" \
//	 "2: Threshold Truncated\n" \
//	 "3: Threshold to Zero\n" \
// 	 "4: Threshold to Zero Inverted\n"));
//
//	setLayout(layout);
//	setWindowTitle(tr("Parameters"));
//
// }
//} /* namespace ui */
