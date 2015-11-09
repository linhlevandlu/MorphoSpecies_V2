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
#include <QtGui/qgroupbox.h>
#include <QtGui/qpushbutton.h>

#include "../../ImageViewer.h"

namespace impls_2015 {
class EdgeSegmentationPanel : public QDialog{
	Q_OBJECT
public:

    EdgeSegmentationPanel(QWidget *parent,QString filePath, Image::SegmentMethod sgmethod,int threshold);
private slots:
	void on_thresholdValueSize_valueChanged(int value);
	void on_otsuChoice_valueChanged(bool checked);
	void on_otherChoice_valueChanged(bool checked);
	//void on_saveButton_Clicked(bool clicked);
private:
	void initValues(Image::SegmentMethod sgmethod, int threshold);
    QSlider *thresholdValueSize;
    QRadioButton *otsuChoice;
    QRadioButton *otherChoice;
  //  QPushButton *saveButton;
    QHBoxLayout *hbox;

    QString filePath;
    Image::SegmentMethod sgmethod;
    //vector<Line> lines;
};
}

#endif /* EDGESEGMENTATIONPANEL_H_ */
