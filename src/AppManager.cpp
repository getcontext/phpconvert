/*
 * AppManager.cpp
 *
 *  Created on: 12 May 2014
 *      Author: Andrzej Salamon <andrzej.salamon@gmail.com>
 */

#include "AppManager.h"
#include "plugin/ZendParser.h"
#include "SystemException.h"

namespace Salamon {

AppManager::AppManager() {
	ParserManager* parserObj = new ParserManager();
	parser = parserObj;
}

AppManager::~AppManager() {
	// TODO Auto-generated destructor stub
}

void AppManager::run(AppParams* params) {
	if(!params->isValid()) {
		throw SystemException(SystemException::INVALID_PARAMETERS);
	}
	ZendParser* worker = new ZendParser();
	BaseParser* w = worker;
	parser->setWorker(w);
	parser->parse(params->path, params->recursive);
}

} /* namespace Salamon */
