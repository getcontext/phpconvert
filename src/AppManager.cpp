/*
 * AppManager.cpp
 *
 *  Created on: 12 May 2014
 *      Author: Andrzej phpconvert <andrzej.salamon@gmail.com>
 */

#include "AppManager.h"
#include "plugin/ZendParser.h"
#include "SystemException.h"

namespace phpconvert {

    AppManager::AppManager() {
        ParserManager *parserObj = new ParserManager();
        parser = parserObj;
    }

    AppManager::~AppManager() {
        delete parser;
    }

    void AppManager::run(AppParams *params) {
        if (!params->isValid()) {
            throw SystemException(SystemException::INVALID_PARAMETERS);
        }
        ZendParser *worker = new ZendParser(); //@todo detect plugin
        BaseParser *w = worker;
        parser->setWorker(w);
        parser->parse(params);
    }

} /* namespace phpconvert */
