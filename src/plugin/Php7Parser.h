//
// Created by wizard on 09.02.19.
//

#ifndef PHPCONVERT_PHP7PARSER_H
#define PHPCONVERT_PHP7PARSER_H

#include "ZendParser.h"

#endif //PHPCONVERT_PHP7PARSER_H
namespace phpconvert {

    class Php7Parser : public ZendParser {
    public:
        Php7Parser();

        virtual ~Php7Parser();
    };
}