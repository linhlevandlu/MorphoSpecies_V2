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

#include <map>
#include "ant.h"

#include <QDebug>

namespace algorithms {

Ant::Ant(Position pos){
	this->position = pos;
}

vector<Position> Ant::perception(int w, int h){
	vector<Position> list;
	for (int n = -1; n <= 1; n++) {
		if (n + position.row < 0 || n + position.row >= h)
			continue;
		for (int m = -1; m <= 1; m++) {
			if (m + position.col < 0 || m + position.col >= w)
				continue;
			if (m == n && n == 0)
				continue;

			Position pos(n + position.row, m + position.col);

			if (history.count(pos)==0){
				list.push_back(pos);
			}
		}
	}
	return list;
}

Next Ant::decision(vector<Position> neighbors, float* pheromones, float* pheromoneMore,
			int w, QImage src, QImage sobel){
	map<Position,float> prob;

	QRgb rgb;
	int r;

	rgb = src.pixel(position.col, position.row);
	r = qRed(rgb);
	//r = qGray(rgb);

	int grayPos = r;

	Position pos;
	for(size_t i=0; i< neighbors.size(); i++){
		 pos = neighbors[i];
		//WeightMove(p)
		float wMove=1;
		if (pos.col>position.col && pos.row > position.row){
			wMove=2;
		}else if (pos.col>position.col && pos.row <= position.row){
			wMove=1.5f;
		}else if (pos.col<=position.col && pos.row > position.row){
			wMove=1.5f;
		}

		//computeWeightGrad(p)
		rgb = sobel.pixel(pos.col, pos.row);
		r = qRed(rgb);
		//r = qGray(rgb);

		float wGrad = (float)r/256;
		//computeWeightVarv(position,p)
		float wVarv = 0;
		rgb = src.pixel(pos.col, pos.row);
		r = qGray(rgb);
		if (grayPos == r)
			wVarv = 1;
		else
			wVarv = 1/ (float) abs(grayPos - r);
		//computeWeightPhi(p)
		float wPhi = pheromones[pos.row * w + pos.col] + wMove * wVarv * wGrad;

		prob[pos]= (wGrad * wPhi);
	}

	float max=0.0f,sum=0.0f,tmp;

	Position rs;
	map<Position,float>::iterator ii;
	for(ii=prob.begin(); ii!=prob.end(); ++ii)
	{
		if  (pheromoneMore[(*ii).first.row * w + (*ii).first.col]!=0) continue;
		tmp = (*ii).second;
		sum += tmp;
		if (tmp>max) {
			max = tmp;
			rs = (*ii).first;
		}
	}

	return Next (rs, (sum==0)?0:max/sum);
}

void Ant::action(Next next, float* pheromoneMore, int w, float max, bool useThreshold){
	if (next.p.row!=-1){
		this->history.insert(position);
		this->position = next.p;
		int idx = next.p.row * w + next.p.col;
		if (useThreshold){
			if (pheromoneMore[idx] < max) {
				pheromoneMore[idx] = next.v;
			}
		}else{
			pheromoneMore[idx] = next.v;
		}

	}
}

} /* namespace algorithms */
