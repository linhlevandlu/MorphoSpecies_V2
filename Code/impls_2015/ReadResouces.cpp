/*
 * ReadResouces.cpp
 *
 *  Created on: Nov 8, 2015
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

#include "ReadResouces.h"

namespace impls_2015 {

ReadResouces::ReadResouces() {
	// TODO Auto-generated constructor stub

}

ReadResouces::~ReadResouces() {
	// TODO Auto-generated destructor stub
}
map<string, int> ReadResouces::readResources(string filePath) {
	ifstream openFile(filePath.c_str());
	string lineText;
	map<string, int> resourceMap;
	if (openFile.is_open()) {
		while (getline(openFile, lineText)) {
			QString line = lineText.c_str();
			QStringList listString = line.replace(" ", "").split("=");
			resourceMap[listString.at(0).toStdString().c_str()] = listString.at(
					1).toInt();
		}
		openFile.close();
	}
	return resourceMap;
}
} /* namespace impls_2015 */
