/*
 * Parser.h
 *
 *  Created on: 3 May 2014
 *      Author: Andrzej S <andrzej.salamon@gmail.com>
 */

#ifndef BASEPARSER_H_
#define BASEPARSER_H_

#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <exception>
#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>    // token class
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp> // lexer class
#include "Parser.h"
#include "../utils/DirectoryReader.h"
#include "../utils/Regexer.h"
#include "../utils/Strings.h"
#include "parser/PreparedType.h"
#include "parser/File.h"
//#include "parser/PreparedType.h"
//#include "parser/File.h"

using namespace std;

namespace phpconvert {

    class Parser {
    public:
        struct PreparedType : public parser::PreparedType { //@todo extract it to separate class
            //@todo new introduced field, fully qualified namespace with c/i name

        };

        struct File : public parser::File {

        };

        Parser();

        virtual ~Parser() = 0;

        virtual void parse() = 0;

        void setSourceDir(const string path);

        void setOutputDir(const string path);

        void setRecursive(const bool val);

        bool isRecurisve();

        static const char *RGX_INSTANCEOF;
        static const char *RGX_NEW;
        static const char *RGX_SIGNATURE;
        static const char *RGX_STATIC_CALL;
        static const char *RGX_MAIN_TYPE;
        static const char *RGX_THROW_NEW;
        static const char *RGX_PHP_OPENING_TAG;
        //@todo detect if it is script or (class/interface or more of them)
        static const char *RGX_BUILTIN_TYPE;
        static const char *RGX_TYPE;
        static const char *RGX_EXCLUDE_COMMENT;
    protected:
        string sourceDir;
        string outputDir;
        bool recursive;
        //utility vector
        vector<string> *vectorString;
        set<string> *setString;
    };

} /* namespace phpconvert */

#endif /* BASEPARSER_H_ */
