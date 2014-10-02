/*
 * ParserManager.h
 *
 *  Created on: 3 May 2014
 *      Author: Andrzej Salamon <andrzej.salamon@gmail.com>
 */

#ifndef PARSERMANAGER_H_
#define PARSERMANAGER_H_
#include <string>
#include "BaseParser.h"
using namespace std;

namespace Salamon {

class ParserManager {
public:
	ParserManager();
	virtual ~ParserManager();
	void setWorker(BaseParser *p);
	BaseParser *getParser();
	void parse(const string path, const bool recursive);
protected:
	BaseParser *parser;
};

} /* namespace Salamon */

#endif /* PARSERMANAGER_H_ */
