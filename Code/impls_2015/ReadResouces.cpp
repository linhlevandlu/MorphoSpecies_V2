/*
 * ReadResouces.cpp
 *
 *  Created on: Nov 8, 2015
 *      Author: linh
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
