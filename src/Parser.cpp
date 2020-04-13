/*
 * Parser.cpp
 *
 *  Created on: 3 May 2014
 *      Author: Andrzej phpconvert <andrzej.salamon@gmail.com>
 */

#include "ZendParser.h"
#include "Parser.h"

const char *phpconvert::Parser::RGX_INSTANCEOF = "instanceof\\s+([A-Za-z0-9_]+)";
const char *phpconvert::Parser::RGX_NEW = "new\\s+([A-Za-z0-9_]+)";
const char *phpconvert::Parser::RGX_SIGNATURE = "([A-Za-z0-9_]+)\\s+\\$[a-zA-Z0-9_]+";
const char *phpconvert::Parser::RGX_STATIC_CALL = "([A-Za-z0-9_]+)::";
const char *phpconvert::Parser::RGX_MAIN_TYPE = "(\\s*(final|abstract)?"
                                        "[\\s\n]*(class|interface)[\\s\n]+([A-Za-z0-9_]+)"
                                        "[\\s\n]*(extends[\\s\n]+([A-Za-z0-9_]+)[\\s\n]*)?"
                                        "([\\s\n]*implements[\\s\n]+([A-Za-z0-9_ ,\\s\n]+))?[\\s\n]?\\{)";
const char *phpconvert::Parser::RGX_PHP_OPENING_TAG = "^(<\\?|<\\?php)";
const char *phpconvert::Parser::RGX_BUILTIN_TYPE =
        "([^\\\\/_\[:alnum:]])(%s)([^\\\\/_\[:alnum:]])";
const char *phpconvert::Parser::RGX_TYPE =
        "([^\\\\/_\[:alnum:]])(%s)([^\\\\/_\[:alnum:]])";
const char* phpconvert::Parser::RGX_EXCLUDE_COMMENT = "^((?!\\s*\\\\|/\\*|\\*)";

namespace phpconvert {

    Parser::Parser() {
        setRecursive(false);
    }

    Parser::~Parser() {
    }

    void Parser::setSourceDir(const string p) { //@ref here and next
        sourceDir = p;
    }

    void Parser::setOutputDir(const string p) {
        outputDir = p;
    }

    void Parser::setRecursive(bool val) {
        recursive = val;
    }

    bool Parser::isRecurisve() {
        return recursive;
    }
} /* namespace phpconvert */

