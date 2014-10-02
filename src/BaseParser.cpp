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

void BaseParser::setPath(const string p) {
	// TODO Auto-generated destructor stub
	path = p;
}

void BaseParser::setRecursive(bool val) {
	recursive = val;
}

bool BaseParser::isRecurisve() {
	return recursive;
}

} /* namespace Salamon */

