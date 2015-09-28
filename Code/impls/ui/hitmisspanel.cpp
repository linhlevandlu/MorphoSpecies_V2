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

//#include <QtGui/QFormLayout>
//#include <QtGui/QLabel>
//
//#include "imagepanel.h"
//#include "hitmisspanel.h"
//#include "../algorithms/hitmiss.h"
//
//using namespace algorithms;
//
//namespace ui {
//
//void HitMissPanel::actionExcute(){
//	ImagePanel *parent = qobject_cast<ImagePanel *>(this->parentWidget());
//
//	HitMiss convert;
//	HitMissParams opt;
//	opt.angel2h = ckb1->isChecked();
//	opt.angel4h = ckb2->isChecked();
//	opt.angel8h = ckb3->isChecked();
//	opt.angel10h = ckb4->isChecked();
//	opt.threshold = txt->value();
//	parent->m_image = convert.excute(this->m_image,opt);
//
//	parent->update();
//	this->hide();
//}
//
//HitMissPanel::HitMissPanel(QWidget *parent, QImage m_image)
//     : QDialog(parent)
// {
//	this->m_image = m_image;
//
//	txt = new QSpinBox;
//	txt->setMinimum(0);
//	txt->setMaximum(255);
//	txt->setValue(128);
//
//	btn = new QPushButton;
//	btn->setText("Excute");
//
//	ckb1 = new QCheckBox;
//	ckb1->setChecked(true);
//
//	ckb2 = new QCheckBox;
//	ckb2->setChecked(true);
//
//	ckb3 = new QCheckBox;
//	ckb3->setChecked(true);
//
//	ckb4 = new QCheckBox;
//	ckb4->setChecked(true);
//
//	connect(btn, SIGNAL(clicked()), this, SLOT(actionExcute()));
//
//	QFormLayout *layout = new QFormLayout;
//	layout->addRow(new QLabel(tr("Threshold")), txt);
//	layout->addRow(new QLabel(tr("Angel 45")), ckb1);
//	layout->addRow(new QLabel(tr("Angel 135:")), ckb2);
//	layout->addRow(new QLabel(tr("Angel 225:")), ckb3);
//	layout->addRow(new QLabel(tr("Angel 315:")), ckb4);
//	layout->addRow(new QLabel(""), btn);
//
//	setLayout(layout);
//	setWindowTitle(tr("Parameters"));
//
// }
//
//} /* namespace ui */
