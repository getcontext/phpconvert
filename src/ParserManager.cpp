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

void ParserManager::parse(AppParams* params) {
	getParser()->setSourceDir(params->path);
	getParser()->setRecursive(params->recursive);
	getParser()->setOutputDir(params->outputPath);
	getParser()->parse();
}

} /* namespace Salamon */
