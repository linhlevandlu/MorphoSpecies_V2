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
//
//#include <QDebug>
//#include <QtGui/QVBoxLayout>
//#include <QtGui/QHBoxLayout>
//#include <QtGui/QGridLayout>
//#include <QtGui/QLabel>
//
//#include "otsupanel.h"
//#include "imagepanel.h"
//#include "../algorithms/otsu.h"
//
//using namespace algorithms;
//
//namespace ui {
//void OtsuPanel::actionExcute(){
//	ImagePanel *parent = qobject_cast<ImagePanel *>(this->parentWidget());
//	Otsu convert;
//	parent->m_image = convert.excute(this->m_image, txt->value());
//	parent->update();
//	this->hide();
//}
//
//OtsuPanel::OtsuPanel(QWidget *parent, QImage m_image)
//     : QDialog(parent)
// {
//	this->m_image = m_image;
//
//	QHBoxLayout *topLeftLayout = new QHBoxLayout;
//	topLeftLayout->addWidget(new QLabel(tr("Level")));
//
//	txt = new QSpinBox;
//	txt->setMinimum(2);
//	txt->setMaximum(5);
//	txt->setValue(3);
//	topLeftLayout->addWidget(txt);
//
//	btn = new QPushButton;
//	btn->setText("Excute");
//	topLeftLayout->addWidget(btn);
//
//	connect(btn, SIGNAL(clicked()), this, SLOT(actionExcute()));
//
//
//	setLayout(topLeftLayout);
//
//	setWindowTitle(tr("Parameters"));
//
// }
//} /* namespace ui */
