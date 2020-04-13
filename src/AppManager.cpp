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
//        parser = new ParserManager()
    }

    AppManager::~AppManager() {
        delete parser;
    }

    void AppManager::run(AppParams *params) {
        setParams(params);

        if (!getParams()->isValid()) {
            throw SystemException(SystemException::INVALID_PARAMETERS);
        }

        ZendParser *worker = new ZendParser(); //@todo detect plugin
        Parser *w = worker;  //not sure about downcasting... could be factory

        parser->setWorker(w);
        parser->parse(getParams());

        delete w;
        delete worker;
    }

    AppParams *AppManager::getParams() const {
        return params;
    }

    void AppManager::setParams(AppParams *params) {
        this->params = params;
    }

} /* namespace phpconvert */
