/*
 * Parser.h
 *
 *  Created on: 3 May 2014
 *      Author: Andrzej Salamon <andrzej.salamon@gmail.com>
 */

#ifndef BASEPARSER_H_
#define BASEPARSER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

using namespace std;

namespace Salamon {

class BaseParser {
public:
	struct PreparedType {
		string type;
		string usage;
		string alias;
	};

	struct File {
		string name;
		string fullPath;
		string rootPath;
		string namespaceName;
		bool isValid;
		vector<PreparedType> preparedTypes;
		string mainType;
		vector<string> types;
		string content;
	};

	BaseParser();
	virtual ~BaseParser();
	virtual void parse() = 0;
	void setPath(const string path);
	void setRecursive(const bool val);
	bool isRecurisve();
protected:
	string path;
	bool recursive;
	vector<File>* result;
};

} /* namespace Salamon */

#endif /* BASEPARSER_H_ */
