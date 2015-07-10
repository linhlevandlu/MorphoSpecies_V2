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
//#include <QtGui/QFormLayout>
//#include <QtGui/QLabel>
//
//#include "imagepanel.h"
//#include "antmethodpanel.h"
//#include "../algorithms/antmethod.h"
//
//using namespace algorithms;
//
//namespace ui {
//
//void AntMethodPanel::actionExcute(){
//	ImagePanel *parent = qobject_cast<ImagePanel *>(this->parentWidget());
//
//	AntMethod convert;
//	AntMethodParams opt;
//	opt.useThreshold = ckb->isChecked();
//	opt.nbAgent = txtNbAgent->value();
//	opt.nbStep = txtNbstep->value();
//	opt.evaPheromone = txtEvaPheromone->value();
//	opt.maxPheromone = txtMaxPheromone->value();
//	parent->m_image = convert.excute(this->m_image,opt);
//
//	parent->update();
//	this->hide();
//}
//
//AntMethodPanel::AntMethodPanel(QWidget *parent, QImage m_image)
//     : QDialog(parent)
// {
//	this->m_image = m_image;
//
//	txtNbAgent = new QSpinBox;
//	txtNbAgent->setMinimum(1);
//	txtNbAgent->setMaximum(100000);
//	txtNbAgent->setValue(5000);
//
//	txtNbstep = new QSpinBox;
//	txtNbstep->setMinimum(1);
//	txtNbstep->setMaximum(100000);
//	txtNbstep->setValue(1000);
//
//	txtEvaPheromone = new QDoubleSpinBox;
//	txtEvaPheromone->setMinimum(0.0f);
//	txtEvaPheromone->setMaximum(10.0f);
//	txtEvaPheromone->setDecimals(3);
//	txtEvaPheromone->setValue(0.001f);
//
//	txtMaxPheromone = new QDoubleSpinBox;
//	txtMaxPheromone->setMinimum(0.0f);
//	txtMaxPheromone->setMaximum(255.0f);
//	txtMaxPheromone->setValue(20.0f);
//
//	btn = new QPushButton;
//	btn->setText("Excute");
//
//	ckb = new QCheckBox;
//	ckb->setChecked(true);
//
//	ckbGPU = new QCheckBox;
//	ckbGPU->setChecked(false);
//
//	connect(btn, SIGNAL(clicked()), this, SLOT(actionExcute()));
//
//	QFormLayout *layout = new QFormLayout;
//	layout->addRow(new QLabel(tr("Use GPUs")), ckbGPU);
//	layout->addRow(new QLabel(tr("Number steps")), txtNbstep);
//	layout->addRow(new QLabel(tr("Number agents")), txtNbAgent);
//	layout->addRow(new QLabel(tr("Evaporate pheromone")), txtEvaPheromone);
//	layout->addRow(new QLabel(tr("Use threshold")), ckb);
//	layout->addRow(new QLabel(tr("Maximum pheromone")), txtMaxPheromone);
//	layout->addRow(new QLabel(""), btn);
//
//	setLayout(layout);
//	setWindowTitle(tr("Parameters"));
//
// }
//
//} /* namespace ui */
