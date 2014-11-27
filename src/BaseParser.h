/*
 * Parser.h
 *
 *  Created on: 3 May 2014
 *      Author: Andrzej Salamon <andrzej.salamon@gmail.com>
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

using namespace std;

namespace Salamon {

class BaseParser {
public:
	struct PreparedType {
		string type;
		string usage;
		string alias;
		string raw;
		string replace;
		bool isMain = false;
		string extends;
		vector<string> implements;

		bool operator == (PreparedType& obj) const {
			return type.compare(obj.type) == 0 && raw.compare(obj.raw) == 0;
		}
		bool operator < (PreparedType const& obj) const {
			return type.size() > obj.type.size() && raw.size() > obj.raw.size();
		}
	};

	struct File {
		string name;
		string fullPath;
		string rootPath;
		string namespaceName;
		bool isValid;
		string mainType;
		string firstMainTypeFull;
		string content;

		set<string>* mainTypes;
		vector<string> types;
		vector<PreparedType> prepTypes;
		vector<PreparedType> prepTypesMain;
	};

	BaseParser();
	virtual ~BaseParser();
	virtual void parse() = 0;
	void setSourceDir(const string path);
	void setOutputDir(const string path);
	void setRecursive(const bool val);
	bool isRecurisve();
protected:
	string sourceDir;
	string outputDir;
	bool recursive;
};

} /* namespace Salamon */

#endif /* BASEPARSER_H_ */
