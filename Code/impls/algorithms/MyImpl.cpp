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

#include "MyImpl.h"
#include <cmath>
#include "position.h"
#include <QtCore/QDebug>

namespace algorithms {

QImage MyImpl::Roberts(QImage src){

	int xFilter[2][2] = {{1, 0}, { 0, -1}} ;
	int yFilter[2][2] = {{0, 1}, { -1, 0}} ;

	int gxr, gyr, gxg, gyg, gxb, gyb;
	float ff = sqrt(2.0);

	QRgb color;
	int red,green,blue;
	int width = src.width();
	int height = src.height();
	QImage dest(width,height,src.format());
	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			gxr = gyr = gxg = gyg = gxb = gyb = 0;
			for (int n = -1; n <= 0; n++) {
				if (n + row < 0 || n + row >= height) continue;
				for (int m = -1; m <= 0; m++) {
					if (m + col < 0 || m + col >= width) continue;
					color = src.pixel(col + m, row + n);
					red = qRed(color);
					green = qGreen(color);
					blue = qBlue(color);
					gxr += xFilter[m + 1][n + 1] * red;
					gyr += yFilter[m + 1][n + 1] * red;
					gxg += xFilter[m + 1][n + 1] * green;
					gyg += yFilter[m + 1][n + 1] * green;
					gxb += xFilter[n + 1][m + 1] * blue;
					gyb += yFilter[n + 1][m + 1] * blue;
				}
			}
			red = (int) (sqrt((gxr * gxr + gyr * gyr)) / ff);
			green = (int) (sqrt((gxg * gxg + gyg * gyg)) / ff);
			blue = (int) (sqrt((gxb * gxb + gyb * gyb)) / ff);

			dest.setPixel(col, row, qRgb(red, green, blue));
		}
	}
	return dest;
}

QImage MyImpl::Sobel(QImage src){

	int xFilter[3][3] = {{-1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 }} ;
	int yFilter[3][3] = {{1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 }} ;

	int gxr, gyr, gxg, gyg, gxb, gyb;
	float ff = sqrt(2.0);

	QRgb color;
	int red,green,blue;
	int width = src.width();
	int height = src.height();
	QImage dest(width,height,src.format());
	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			gxr = gyr = gxg = gyg = gxb = gyb = 0;
			for (int n = -1; n <= 1; n++) {
				if (n + row < 0 || n + row >= height) continue;
				for (int m = -1; m <= 1; m++) {
					if (m + col < 0 || m + col >= width) continue;
					color = src.pixel(col + m, row + n);
					red = qRed(color);
					green = qGreen(color);
					blue = qBlue(color);
					gxr += xFilter[m + 1][n + 1] * red;
					gyr += yFilter[m + 1][n + 1] * red;
					gxg += xFilter[m + 1][n + 1] * green;
					gyg += yFilter[m + 1][n + 1] * green;
					gxb += xFilter[n + 1][m + 1] * blue;
					gyb += yFilter[n + 1][m + 1] * blue;
				}
			}
			red = (int) (sqrt((gxr * gxr + gyr * gyr)) / ff);
			green = (int) (sqrt((gxg * gxg + gyg * gyg)) / ff);
			blue = (int) (sqrt((gxb * gxb + gyb * gyb)) / ff);

			dest.setPixel(col, row, qRgb(red, green, blue));
		}
	}
	return dest;
}

QImage MyImpl::HitMiss(QImage src, HitMissParams opt){

	int p3 [3][3] =
			{{ -1, 255, -1 },
			{ 0, 255, 255 },
			{ 0, 0, -1 }};
	int p2 [3][3] =
			{{ -1, 255, -1 },
			{ 255, 255, 0 },
			{ -1, 0, 0 }};
	int p1 [3][3] =
			{{ -1, 0, 0 },
			{ 255, 255, 0 },
			{ -1, 255, -1 }};
	int p4 [3][3] =
			{{ 0, 0, -1 },
			{ 0, 255, 255 },
			{ -1, 255, -1 }};

	QRgb clP1 = qRgb(255,0,0);
	QRgb clP2 = qRgb(0,255,0);
	QRgb clP3 = qRgb(0,0,255);
	QRgb clP4 = qRgb(255,255,0);

	int count1=0,count2=0,count3=0,count4=0;
	bool matching1=false,matching2=false,matching3=false,matching4=false;

	QRgb color;
	int gray;
	int width = src.width();
	int height = src.height();
	QImage dest(width,height,src.format());

	for (int row = 1; row < height-1; ++row) {
		for (int col = 1; col < width-1; ++col) {
			matching1=true;
			matching2=true;
			matching3=true;
			matching4=true;
			for (int n = -1; n <= 1; n++) {
				for (int m = -1; m <= 1; m++) {
					color = src.pixel(col + m, row + n);
					gray = qGray(color)<opt.threshold?0:255;
					if (p1[m+1][n+1]!=-1){
						matching1 &= (gray==p1[m+1][n+1]);
					}
					if (p2[m+1][n+1]!=-1){
						matching2 &= (gray==p2[m+1][n+1]);
					}
					if (p3[m+1][n+1]!=-1){
						matching3 &= (gray==p3[m+1][n+1]);
					}
					if (p4[m+1][n+1]!=-1){
						matching4 &= (gray==p4[m+1][n+1]);
					}
				}
			}
			dest.setPixel(col,row,src.pixel(col, row));

			if (matching1 && opt.angel2h) {
				count1++;
				for (int n = -1; n <= 1; n++) {
					for (int m = -1; m <= 1; m++) {
						dest.setPixel(col+m, row+n,clP1);
					}
				}
			}
			if (matching2 && opt.angel4h) {
				count2++;
				for (int n = -1; n <= 1; n++) {
					for (int m = -1; m <= 1; m++) {
						dest.setPixel(col+m, row+n,clP2);
					}
				}
			}
			if (matching3 && opt.angel8h) {
				count3++;
				for (int n = -1; n <= 1; n++) {
					for (int m = -1; m <= 1; m++) {
						dest.setPixel(col+m, row+n,clP3);
					}
				}
			}
			if (matching4 && opt.angel10h) {
				count4++;
				for (int n = -1; n <= 1; n++) {
					for (int m = -1; m <= 1; m++) {
						dest.setPixel(col+m, row+n,clP4);
					}
				}
			}
		}
	}
	return dest;
}

QImage MyImpl::Otsu(QImage src, int level){
	int width = src.width();
	int height = src.height();
	QImage dest(width,height,src.format());

	int L = 256;
	int histogramR[L];
	int histogramG[L];
	int histogramB[L];

	std::fill_n(histogramR, L, 0);
	std::fill_n(histogramG, L, 0);
	std::fill_n(histogramB, L, 0);

	QRgb color;
	int r,g,b;

	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			color = src.pixel(col, row);
			histogramR[qRed(color)]++;
			histogramG[qGreen(color)]++;
			histogramB[qBlue(color)]++;
		}
	}

	int* tR = Otsu_getValues(histogramR, L, level);
	int* tG = Otsu_getValues(histogramG, L, level);
	int* tB = Otsu_getValues(histogramB, L, level);


	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			color = src.pixel(col, row);
			r = qRed(color);
			g = qGreen(color);
			b = qBlue(color);
			for (int i = 0; i < level; ++i) {
				if (r >= tR[i] && r < tR[i + 1]) {
					r = tR[i];
				}
			}
			for (int i = 0; i < level; ++i) {
				if (g >= tG[i] && g < tG[i + 1]) {
					g = tG[i];
				}
			}
			for (int i = 0; i < level; ++i) {
				if (b >= tB[i] && b < tB[i + 1]) {
					b = tB[i];
				}
			}
			dest.setPixel(col, row, qRgb(r,g,b));
		}
	}
//	int L = 256; //impossible pixel rgb is number unsigned integer
//	unsigned int histogram[L];
//
//	std::fill_n(histogram, L, 0);
//
//	QRgb color;
//
//	for (int row = 0; row < height; ++row) {
//		for (int col = 0; col < width; ++col) {
//			color = src.pixel(col, row);
//			histogram[color]++;
//		}
//	}
//
//	unsigned int* tR = getValues(histogram, L, level);
//
//	for (int row = 0; row < height; ++row) {
//		for (int col = 0; col < width; ++col) {
//			color = src.pixel(col, row);
//			for (int i = 0; i < level; ++i) {
//				if (color >= tR[i] && color < tR[i + 1]) {
//					color = tR[i];
//				}
//			}
//			dest.setPixel(col, row, color);
//		}
//	}
	return dest;
}


int* MyImpl::Otsu_getValues(int* histogram, int L, int nbLevel){
	int* t = new int[nbLevel + 1];
	std::fill_n(t, nbLevel + 1, 0);
	float P[L][L];
	float S[L][L];
	float H[L][L];
	P[0][0] = S[0][0] = H[0][0] = histogram[0];
	for (int i = 1; i < L; i++) {
		P[0][i] = P[0][i - 1] + histogram[i];
		S[0][i] = S[0][i - 1] + i * histogram[i];
		if (P[0][i] == 0)
			H[0][i] = 0;
		else
			H[0][i] = S[0][i] * S[0][i] / P[0][i];
	}
	for (int i = 1; i < L; i++) {
		for (int j = 1; j < L; j++) {
			P[i][j] = P[0][j] - P[0][i - 1];
			S[i][j] = S[0][j] - S[0][i - 1];
			if (P[i][j] == 0)
				H[i][j] = 0;
			else
				H[i][j] = S[i][j] * S[i][j] / P[i][j];
		}
	}


	t[0] = 0;
	float maxSig = 0;
	switch (nbLevel) {
	case 2:
		for (int i = 1; i < L - nbLevel; i++) // t1
				{
			float Sq = H[1][i] + H[i + 1][255];
			if (maxSig < Sq) {
				t[1] = i;
				maxSig = Sq;
			}
		}
		break;
	case 3:
		for (int i = 1; i < L - nbLevel; i++) // t1
			for (int j = i + 1; j < L - nbLevel + 1; j++) // t2
					{
				float Sq = H[1][i] + H[i + 1][j] + H[j + 1][255];
				if (maxSig < Sq) {
					t[1] = i;
					t[2] = j;
					maxSig = Sq;
				}
			}
		break;
	case 4:
		for (int i = 1; i < L - nbLevel; i++) // t1
			for (int j = i + 1; j < L - nbLevel + 1; j++) // t2
				for (int k = j + 1; k < L - nbLevel + 2; k++) // t3
						{
					float Sq = H[1][i] + H[i + 1][j] + H[j + 1][k]
							+ H[k + 1][255];
					if (maxSig < Sq) {
						t[1] = i;
						t[2] = j;
						t[3] = k;
						maxSig = Sq;
					}
				}
		break;
	case 5:
		for (int i = 1; i < L - nbLevel; i++) // t1
			for (int j = i + 1; j < L - nbLevel + 1; j++) // t2
				for (int k = j + 1; k < L - nbLevel + 2; k++) // t3
					for (int m = k + 1; m < L - nbLevel + 3; m++) // t4
							{
						float Sq = H[1][i] + H[i + 1][j] + H[j + 1][k]
								+ H[k + 1][m] + H[m + 1][255];
						if (maxSig < Sq) {
							t[1] = i;
							t[2] = j;
							t[3] = k;
							t[4] = m;
							maxSig = Sq;
						}
					}
		break;
	}

	t[nbLevel] = 255;
	return t;
}


QImage MyImpl::Growing(QImage src, int delta){
	int width = src.width();
	int height = src.height();
	QImage dest(width,height,src.format());
	QRgb rgb,rgb2;

	int length = width*height;
	int gray = 0;
	int gray2 = 0;
	short marked[length];
	short markedCan[length];
	int candidate[length];

	std::fill_n(marked,length,0);
	std::fill_n(markedCan,length,0);
	std::fill_n(candidate,length,0);

	int idxCan = 0;
	int crr;
	int ii,idx = 0;

	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			ii = row * width + col;
			if (marked[ii] == 0) {
				idxCan = 0;
				candidate[idxCan++] = ii;
				markedCan[ii] = 1;

				rgb = src.pixel(col, row);
				gray = qGray(rgb);
				dest.setPixel(col, row, qRgb(gray,gray,gray));

				while (idxCan != 0) {
					crr = candidate[--idxCan];
					marked[crr] = 1;
					int prow = crr / width;
					int pcol = crr % width;
					for (int n = -1; n <= 1; n++) {
						if (n + prow < 0 || n + prow >= height)
							continue;
						for (int m = -1; m <= 1; m++) {
							if (m + pcol < 0 || m + pcol >= width)
								continue;
							if (m == n && n == 0)
								continue;
							idx = (n + prow) * width + (m + pcol);
							rgb2 = src.pixel(m + pcol, n + prow);
							gray2 = qGray(rgb2);

							if (marked[idx] == 0 && markedCan[idx] == 0
									&& (abs(gray2 - gray) < delta)) {
								dest.setPixel(m + pcol, n + prow, qRgb(gray,gray,gray));
								candidate[idxCan++] = idx;
								markedCan[idx] = 1;
							}
						}
					}
				}
			}
//			color = src.pixel(col, row);
//			dest.setPixel(col,row, qRgb(qGray(color), qGray(color), qGray(color)));
		}
	}
	return dest;
}

QImage MyImpl::AntMethod(QImage src, AntMethodParams opt){
	int width = src.width();
	int height = src.height();
	QImage dest(width,height,src.format());

	int length = width*height;


	float pheromoneMore[length];
	float pheromones[length];

	std::fill_n(pheromones,length,0);
	std::fill_n(pheromoneMore,length,0);

	vector<Ant*> listAnt = MyImpl::AntMethod_randomAnts(opt.nbAgent,width,height);

	vector<Position> neighbors;
//	Next next;

	QImage sobel = MyImpl::Sobel(src);

	Ant* ant;
	for (int i = 0; i < opt.nbStep; i++) {
		if (i%100==0) {
			qDebug() << "time " << i;
		}
		std::fill_n(pheromoneMore,length,0);
		vector<Ant*>::iterator iit;
		//for (size_t j=0; j< listAnt.size();j++){
		for (iit = listAnt.begin(); iit!= listAnt.end();iit++){
			ant = (*iit);
			neighbors = ant->perception(width,height);
			Next next = ant->decision(neighbors, pheromones, pheromoneMore, width, sobel, src);
			ant->action(next,pheromoneMore,width,opt.maxPheromone,opt.useThreshold);
		}
		for (int ii=0;ii<length;ii++){
			if (opt.useThreshold){
				if (pheromones[ii]<opt.maxPheromone) {
					pheromones[ii] += pheromoneMore[ii];
					pheromones[ii] = (1 - opt.evaPheromone) * pheromones[ii];
				}
			}else{
				pheromones[ii] += pheromoneMore[ii];
				pheromones[ii] = (1 - opt.evaPheromone) * pheromones[ii];
			}
		}
	}

	double max=0,min=opt.maxPheromone;
	double sum=0;
	int count=0;
	float v=0;
	for (int i=0;i<length;i++){
		v = pheromones[i];
		if (v>0.0f) {
			count++;
			if (v<min) min = v;
		}

		if (v>max) max = v;

		sum += v;
	}

	qDebug() << "number pixel have some pheronmone: " << count;
	qDebug() << "avg: " << sum/length;
	qDebug() << "min: " << min;
	qDebug() << "max: " << max;


	int i=0,grayColor=0;
	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			if (!opt.useThreshold) {
				if (pheromones[i] > 0) {
					grayColor = (int) (pheromones[i] * 255 / max);
				} else {
					grayColor = 0;
				}
			}else{

				// new method
				if (pheromones[i] >= opt.maxPheromone) {
					grayColor = 255;
				} else {
					grayColor = 0;
				}
			}

			i++;
			dest.setPixel(col, row, qRgb(grayColor,grayColor,grayColor));
		}
	}
	return dest;
}

vector<Ant*> MyImpl::AntMethod_randomAnts(int numberAgent, int w, int h){
	vector<Ant*> list;
	for (int i=0;i<numberAgent;i++){
		list.push_back(new Ant(Position(rand() % h,rand() % w)));
	}
	return list;
}


} /* namespace algorithms */
