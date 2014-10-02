/*
 * ParserManager.cpp
 *
 *  Created on: 3 May 2014
 *      Author: Andrzej Salamon <andrzej.salamon@gmail.com>
 */

#include "ParserManager.h"

namespace Salamon {

ParserManager::ParserManager() {
	// TODO Auto-generated constructor stub

}

ParserManager::~ParserManager() {
	// TODO Auto-generated destructor stub
}

void ParserManager::setWorker(BaseParser* p) {
	parser = p;
}

BaseParser* ParserManager::getParser() {
	return parser;
}

void ParserManager::parse(const string path, const bool recursive) {
	getParser()->setPath(path);
	getParser()->setRecursive(recursive);
	getParser()->parse();
}

} /* namespace Salamon */
