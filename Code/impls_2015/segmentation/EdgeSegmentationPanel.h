/*
 * EdgeSegmentationPanel.h
 *
 *  Created on: Nov 3, 2015
 *  Image processing for morphometrics (IPM) Version 0.2
 *	Copyright (C) 2015 LE Van Linh (linhlevandlu@gmail.com)
 *
 *	This program is free software: you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License as published by
 * 	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see http://www.gnu.org/licenses/.
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
