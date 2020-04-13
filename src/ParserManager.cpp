/*
 * ParserManager.cpp
 *
 *  Created on: 3 May 2014
 *      Author: Andrzej phpconvert <andrzej.salamon@gmail.com>
 */

#include "ParserManager.h"

namespace phpconvert {

    ParserManager::ParserManager() {
    }

    ParserManager::~ParserManager() {
        delete parser;
    }

    void ParserManager::setWorker(Parser *p) {
        parser = p;
    }

    Parser *ParserManager::getParser() {
        return parser;
    }

    void ParserManager::parse(AppParams *params) {
        //give parser whole *params
        getParser()->setSourceDir(params->getPath());
        getParser()->setRecursive(params->isRecursive());
        getParser()->setOutputDir(params->getOutputPath());
        getParser()->parse();
    }

} /* namespace phpconvert */
