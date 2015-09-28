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

#ifndef MYIMPL_H_
#define MYIMPL_H_

#include <QtGui/QImage>
#include <vector>

#include "ant.h"

using namespace std;

namespace algorithms {

class HitMissParams{
public:
	bool angel8h;
	bool angel4h;
	bool angel10h;
	bool angel2h;
	int threshold;
	HitMissParams():
		angel8h(true),
		angel4h(true),
		angel10h(true),
		angel2h(true),
		threshold(128){}
};

class AntMethodParams{
public:
	int nbStep;
	int nbAgent;
	float evaPheromone;
	float maxPheromone;
	bool useThreshold;
	AntMethodParams():
		nbStep(1000),
		nbAgent(5000),
		evaPheromone(0.001f),
		maxPheromone(20.0f),
		useThreshold(false){}
};

class MyImpl {
public:
	static QImage Roberts(QImage src);
	static QImage Sobel(QImage src);
	static QImage HitMiss(QImage src, HitMissParams opt);
	static QImage Otsu(QImage src, int level);
	static int* Otsu_getValues(int* histogram, int L, int nbLevel);
	static QImage Growing(QImage src, int delta);
	static QImage AntMethod(QImage src, AntMethodParams opt);
	static vector<Ant*> AntMethod_randomAnts(int numberAgent, int w, int h);
};

} /* namespace algorithms */
#endif /* MYIMPL_H_ */
