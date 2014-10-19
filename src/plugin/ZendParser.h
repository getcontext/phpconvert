/*
 * PhpParser.h
 *
 * parses and extracs zend1 shit
 *
 *  Created on: 3 May 2014
 *      Author: Andrzej Salamon <andrzej.salamon@gmail.com>
 */

#ifndef ZENDPARSER_H_
#define ZENDPARSER_H_

#include "../BaseParser.h"
#include "../utils/DirectoryReader.h"
#include "../utils/Regexer.h"
#include "../utils/Strings.h"

namespace Salamon {

class ZendParser: public BaseParser {
public:
	ZendParser();
	virtual ~ZendParser();
	virtual void parse();

	DirectoryReader* getReader();
	Regexer* getRegexer();

	static const string RGX_INSTANCEOF;
	static const string RGX_NEW;
	static const string RGX_SIGNATURE;
	static const string RGX_STATIC_CALL;
	static const string RGX_MAIN_TYPE;
	static const string RGX_THROW_NEW;

protected:
	void setupReader();
	File buildFile(DirectoryReader::Item* item);
	vector<string> extractMainType(const string line);
	vector<string> extractTypes(const string line);
	vector<string> filterTypes(vector<string> types);
	vector<PreparedType> generatePreparedTypes(vector<string>& types, string mainType);
	const string extractNamespace(const string className);
	const string toNamespace(const string className);
	void readBuiltInTypes();
private:
	DirectoryReader* reader;
	Regexer regexer;
	set<string> builtInTypes;

};

} /* namespace Salamon */

#endif /* ZENDPARSER_H_ */
