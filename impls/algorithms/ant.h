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

#ifndef ANT_H_
#define ANT_H_

#include <set>
#include <vector>
#include <QtGui/QImage>
#include "position.h"

using namespace std;

namespace algorithms {

class Next{
public:
	Position p;
	float v;
	Next():p(),v(0){}
	Next(Position _p, float _v){
		this->p=_p;
		this->v=_v;
	}
};

class Ant {
public:
	Position position;
	set<Position> history;
	Ant():position(){};
	Ant(Position pos);
	vector<Position> perception(int w, int h);
	Next decision(vector<Position> neighbors, float* pheromones, float* pheromoneMore,
			int w, QImage src, QImage sobel);
	void action(Next next, float* pheromoneMore, int w, float max, bool useThreshold);

};

} /* namespace algorithms */
#endif /* ANT_H_ */
