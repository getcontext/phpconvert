//
// Created by wizard on 12.04.2020.
//

#ifndef PHPCONVERT_PREPAREDTYPE_H
#define PHPCONVERT_PREPAREDTYPE_H

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
#include "Parser.h"
#include "utils/DirectoryReader.h"
#include "utils/Regexer.h"
#include "utils/Strings.h"

namespace phpconvert {
    namespace parser {
        class PreparedType {

        public:
            string id;
            string type;
            string typeLower;
            string usage;
            string alias;
            string raw;
            string replace;
            bool isMain = false;
            string extends;
            vector<string> implements;

            bool operator==(PreparedType &compared) const {
                return typeLower.compare(compared.typeLower) == 0;
            }

            bool operator<(PreparedType const &compared) const {
                return typeLower.size() > compared.typeLower.size();
            }
        };
    }
}


#endif //PHPCONVERT_PREPAREDTYPE_H
