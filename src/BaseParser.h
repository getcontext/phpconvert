/*
 * Parser.h
 *
 *  Created on: 3 May 2014
 *      Author: Andrzej phpconvert <andrzej.salamon@gmail.com>
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

namespace phpconvert {

class BaseParser {
public:
	struct PreparedType {
		string type;
		string typeLower;
		string usage;
		string alias;
		string raw;
		string replace;
		bool isMain = false;
		string extends;
		vector<string> implements;

		bool operator ==(PreparedType& compared) const {
			return typeLower.compare(compared.typeLower) == 0;
		}
		bool operator <(PreparedType const& compared) const {
			return typeLower.size() > compared.typeLower.size();
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

} /* namespace phpconvert */

#endif /* BASEPARSER_H_ */
