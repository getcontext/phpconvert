//
// Created by wizard on 09.02.19.
//

#include "ZendParser.h"

#ifndef PHPCONVERT_PHP7PARSER_H
#define PHPCONVERT_PHP7PARSER_H

#endif //PHPCONVERT_PHP7PARSER_H
namespace phpconvert {

    class Php7Parser : public ZendParser {
    public:
        Php7Parser();

        virtual ~Php7Parser();
    protected:
        PhpReturnType *detectReturnType(PhpMethod method);
        PhpReturnType *detectReturnType(PhpFunction func);
        //write test
    };
}
