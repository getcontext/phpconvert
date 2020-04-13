//
// Created by wizard on 13.04.2020.
//

#ifndef PHPCONVERT_FILE_H
#define PHPCONVERT_FILE_H

#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <set>
#include <exception>
#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>    // token class
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp> // lexer class
#include "utils/DirectoryReader.h"
#include "utils/Regexer.h"
#include "utils/Strings.h"
#include "parser/PreparedType.h"

namespace phpconvert {
    /*namespace phpconvert::parser c++17 {*/
    namespace parser {
        class File {
        public: //@todo encapsulate
            string name;
            string fullPath;
            string rootPath;
            string namespaceName;
            bool isValid;
            string mainType;
            string firstMainTypeFull;
            string content;
            set<string> *mainTypes;
            vector<string> types;
            vector<PreparedType> prepTypes;
            vector<PreparedType> prepTypesMain;
        };
    }
}

#endif //PHPCONVERT_FILE_H
