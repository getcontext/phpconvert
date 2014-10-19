/*
 * Parser.cpp
 *
 *  Created on: 3 May 2014
 *      Author: Andrzej Salamon <andrzej.salamon@gmail.com>
 */

#include "BaseParser.h"

namespace Salamon {

BaseParser::BaseParser() {
	// TODO Auto-generated constructor stub

}

BaseParser::~BaseParser() {
	// TODO Auto-generated destructor stub
}

void BaseParser::setSourceDir(const string p) {
	// TODO Auto-generated destructor stub
	sourceDir = p;
}

void BaseParser::setOutputDir(const string p) {
	// TODO Auto-generated destructor stub
	outputDir = p;
}

void BaseParser::setRecursive(bool val) {
	recursive = val;
}

bool BaseParser::isRecurisve() {
	return recursive;
}

} /* namespace Salamon */

