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

#ifndef SURFPANEL_H_
#define SURFPANEL_H_

#include <QtGui/QDialog>
#include <QtGui/QWidget>
#include <QtGui/QSlider>

namespace ui {

class SurfPanel : public QDialog
{
    Q_OBJECT

public:
    static const int DF_MIN_HESSIAN=400;
    SurfPanel(QWidget *parent);

private slots:
	void on_sliMin_valueChanged(int value);
private:
    QSlider *sliMin;
};

} /* namespace ui */
#endif /* SURFPANEL_H_ */
