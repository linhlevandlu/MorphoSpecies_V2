/*
 * EdgeSegmentationPanel.cpp
 *
 *  Created on: Nov 3, 2015
 *      Author: linh
 */

#include "EdgeSegmentationPanel.h"
#include "../../ImageViewer.h"

namespace impls_2015 {
EdgeSegmentationPanel::EdgeSegmentationPanel(QWidget *parent, QString filePath,
		Image::SegmentMethod sgmethod, int threshold) :
		QDialog(parent) {

	this->filePath = filePath;
	this->sgmethod = sgmethod;
	//this->lines = lineSegment;
	initValues(sgmethod, threshold);

	QFormLayout *layout = new QFormLayout;
	layout->addRow(new QLabel(tr("Threshold value size: ")),
			thresholdValueSize);
	layout->addRow(new QLabel(tr("Change segmentation method: ")), hbox);
	//layout->addRow(new QLabel("Save to PGH file"), saveButton);

	setLayout(layout);
	setWindowTitle(tr("Parameters"));

}

void EdgeSegmentationPanel::initValues(Image::SegmentMethod sgmethod,
		int threshold) {

	thresholdValueSize = new QSlider(Qt::Horizontal, this);
	thresholdValueSize->setRange(0, 255);
	thresholdValueSize->setValue(threshold);
	thresholdValueSize->setSingleStep(3);
	connect(thresholdValueSize, SIGNAL(valueChanged(int)), this,
			SLOT(on_thresholdValueSize_valueChanged(int)));

	otsuChoice = new QRadioButton("Otsu method", this);
	connect(otsuChoice, SIGNAL(toggled(bool)), this,
			SLOT(on_otsuChoice_valueChanged(bool)));

	otherChoice = new QRadioButton("Other method", this);
	connect(otherChoice, SIGNAL(toggled(bool)), this,
			SLOT(on_otherChoice_valueChanged(bool)));
	if (sgmethod == Image::Otsu)
		otsuChoice->setChecked(true);
	else
		otherChoice->setChecked(true);
	hbox = new QHBoxLayout;
	hbox->addWidget(otsuChoice);
	hbox->addWidget(otherChoice);

	/*saveButton = new QPushButton("Save", this);
	 connect(saveButton, SIGNAL(clicked(bool)), this,
	 SLOT(on_saveButton_Clicked(bool)));*/
}
void EdgeSegmentationPanel::on_thresholdValueSize_valueChanged(int value) {
	ImageViewer* parent = qobject_cast<ImageViewer *>(this->parentWidget());
	parent->edgeSegmentation_Value_Changed(filePath, value);

}
void EdgeSegmentationPanel::on_otsuChoice_valueChanged(bool checked) {
	if (checked) {
		ImageViewer* parent = qobject_cast<ImageViewer *>(this->parentWidget());
		sgmethod = Image::Otsu;
		parent->edgeSegmentation_Method_Changed(filePath, sgmethod);
	}
}
void EdgeSegmentationPanel::on_otherChoice_valueChanged(bool checked) {
	if (checked) {
		ImageViewer* parent = qobject_cast<ImageViewer *>(this->parentWidget());
		sgmethod = Image::Other;
		parent->edgeSegmentation_Method_Changed(filePath, sgmethod);
	}
}
/*void EdgeSegmentationPanel::on_saveButton_Clicked(bool clicked) {
 //qDebug()<<this->lines.size();
 ImageViewer* parent = qobject_cast<ImageViewer *>(this->parentWidget());
 parent->edgeSegmentation_Save_PGH(filePath,this->lines);

 }*/
}

