/*
 * EdgeSegmentationPanel.h
 *
 *  Created on: Nov 3, 2015
 *      Author: linh
 */

#ifndef EDGESEGMENTATIONPANEL_H_
#define EDGESEGMENTATIONPANEL_H_

#include <QtCore/QDebug>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>

#include "../../ImageViewer.h"

namespace impls_2015 {
class EdgeSegmentationPanel : public QDialog{
	Q_OBJECT
public:
    //static const int DF_KERNEL_SIZE = 100;
    EdgeSegmentationPanel(QWidget *parent,QString filePath,int threshold);
private slots:
	void on_thresholdValueSize_valueChanged(int value);
private:
    QSlider *thresholdValueSize;
    QString filePath;
};
}

#endif /* EDGESEGMENTATIONPANEL_H_ */
