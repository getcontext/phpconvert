/*
 * AppParams.cpp
 *
 *  Created on: 17 May 2014
 *      Author: Andrzej Salamon <andrzej.salamon@gmail.com>
 */

#include "AppParams.h"

namespace Salamon {

AppParams* AppParams::createFromRequest(int argc, char* argv[]) {
	string current;
	AppParams* obj = new AppParams();

	obj->setValid(false);
	obj->recursive = false;

	for (int i = 1; i < argc; i += 1) {
		current = argv[i];
		if (current.compare("-r") == 0) {
			obj->recursive = true;
		} else if (current.compare("-d") == 0) {
			obj->path = argv[i + 1];
			obj->setValid(true);
			i++;
		} else if (current.compare("-o") == 0) {
			obj->outputPath = argv[i + 1];
			i++;
		}
	}

	return obj;
}

void AppParams::setRecursive(const bool recursive) {
	this->recursive = recursive;
}

bool AppParams::isValid() {
	return valid;
}

void AppParams::setValid(const bool valid) {
	this->valid = valid;
}

} /* namespace Salamon */
