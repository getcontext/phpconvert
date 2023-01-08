//
// Created by wizard on 09.02.19.
//

#include "ZendParser.h"

#ifndef PHPCONVERT_PHP7PARSER_H
#define PHPCONVERT_PHP7PARSER_H

namespace phpconvert {
    class Php7Parser : public ZendParser {
    public:
        Php7Parser();
        virtual ~Php7Parser();
        //@todo handle CameCaseDelimeterClassName , make it enum, no explict calls, isCamelCase(cost char *word)
        constexpr static const char *const DELIMETER = "";
    protected:
//        PhpReturnType *detectReturnType(PhpMethod method);
//        PhpReturnType *detectReturnType(PhpFunction func);
        //write test
    };
}
#endif //PHPCONVERT_PHP7PARSER_H