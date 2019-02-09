/*
 * ParserManager.h
 *
 *  Created on: 3 May 2014
 *      Author: Andrzej phpconvert <andrzej.salamon@gmail.com>
 */

#ifndef PARSERMANAGER_H_
#define PARSERMANAGER_H_

#include <string>
#include "BaseParser.h"
#include "AppParams.h"

using namespace std;

namespace phpconvert {

    class ParserManager {
    public:
        ParserManager();

        virtual ~ParserManager();

        void setWorker(BaseParser *p);

        BaseParser *getParser();

        void parse(AppParams *params);

    protected:
        BaseParser *parser;
    };

} /* namespace phpconvert */

#endif /* PARSERMANAGER_H_ */
