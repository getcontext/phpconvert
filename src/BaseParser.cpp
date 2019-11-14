/*
 * Parser.cpp
 *
 *  Created on: 3 May 2014
 *      Author: Andrzej phpconvert <andrzej.salamon@gmail.com>
 */

#include "BaseParser.h"

namespace phpconvert {

    BaseParser::BaseParser() {
        setRecursive(false);
    }

    BaseParser::~BaseParser() {
    }

    void BaseParser::setSourceDir(const string p) {
        sourceDir = p;
    }

    void BaseParser::setOutputDir(const string p) {
        outputDir = p;
    }

    void BaseParser::setRecursive(bool val) {
        recursive = val;
    }

    bool BaseParser::isRecurisve() {
        return recursive;
    }
} /* namespace phpconvert */

