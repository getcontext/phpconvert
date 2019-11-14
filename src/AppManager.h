/*
 * AppManager.h
 *
 *  Created on: 12 May 2014
 *      Author: Andrzej phpconvert <andrzej.salamon@gmail.com>
 */

#ifndef APPMANAGER_H_
#define APPMANAGER_H_

#include "ParserManager.h"
#include "AppParams.h"

namespace phpconvert {

    class AppManager {
    public:
        AppManager();

        virtual ~AppManager();

        void run(AppParams *params);

        AppParams *getParams() const;

        void setParams(AppParams *params);

    protected:
        ParserManager *parser;
        AppParams *params;
    };

} /* namespace phpconvert */

#endif /* APPMANAGER_H_ */
