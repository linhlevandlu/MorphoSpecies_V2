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

//#include <QDebug>
//#include <QtGui/QVBoxLayout>
//#include <QtGui/QHBoxLayout>
//#include <QtGui/QGridLayout>
//#include <QtGui/QLabel>
//
//#include "growingpanel.h"
//#include "imagepanel.h"
//#include "../algorithms/growing.h"
//
//using namespace algorithms;
//
//namespace ui {
//
//void GrowingPanel::actionExcute(){
//	ImagePanel *parent = qobject_cast<ImagePanel *>(this->parentWidget());
//	Growing convert;
//	parent->m_image = convert.excute(this->m_image, txt->value());
//	parent->update();
//	this->hide();
//}
//
//GrowingPanel::GrowingPanel(QWidget *parent, QImage m_image)
//     : QDialog(parent)
// {
//	this->m_image = m_image;
//
//	QHBoxLayout *topLeftLayout = new QHBoxLayout;
//	topLeftLayout->addWidget(new QLabel(tr("Delta")));
//
//	txt = new QSpinBox;
//	txt->setMinimum(1);
//	txt->setMaximum(255);
//	txt->setValue(30);
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
//
//} /* namespace ui */
