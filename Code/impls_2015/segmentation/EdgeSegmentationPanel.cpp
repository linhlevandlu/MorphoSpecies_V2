/*
 * EdgeSegmentationPanel.cpp
 *
 *  Created on: Nov 3, 2015
 *      Author: linh
 */

#include "EdgeSegmentationPanel.h"
#include "../../ImageViewer.h"

namespace impls_2015 {
EdgeSegmentationPanel::EdgeSegmentationPanel(QWidget *parent, QString filePath, int threshold) :
		QDialog(parent) {
	thresholdValueSize = new QSlider(Qt::Horizontal, this);
	thresholdValueSize->setRange(0, 255);
	thresholdValueSize->setValue(threshold);
	thresholdValueSize->setSingleStep(3);

	connect(thresholdValueSize, SIGNAL(valueChanged(int)), this,
			SLOT(on_thresholdValueSize_valueChanged(int)));

	QFormLayout *layout = new QFormLayout;
	layout->addRow(new QLabel(tr("Threshold value size: ")),
			thresholdValueSize);
	this->filePath = filePath;

	setLayout(layout);
	setWindowTitle(tr("Parameters"));

}
void EdgeSegmentationPanel::on_thresholdValueSize_valueChanged(int value){
	ImageViewer* parent = qobject_cast<ImageViewer *>(this->parentWidget());
	parent->edgeSegmentation(value,filePath);
}
}

