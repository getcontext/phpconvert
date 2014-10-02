/*
 * AppManager.h
 *
 *  Created on: 12 May 2014
 *      Author: Andrzej Salamon <andrzej.salamon@gmail.com>
 */

#ifndef APPMANAGER_H_
#define APPMANAGER_H_
#include "ParserManager.h"
#include "AppParams.h"

namespace Salamon {

class AppManager {
public:
	AppManager();
	virtual ~AppManager();
	void run(AppParams* params);
protected:
	ParserManager *parser;
};

} /* namespace Salamon */

#endif /* APPMANAGER_H_ */
