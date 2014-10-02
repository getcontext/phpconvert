/*
 * PhpParser.cpp
 *
 *  Created on: 3 May 2014
 *      Author: Andrzej Salamon <andrzej.salamon@gmail.com>
 */

#include "ZendParser.h"

namespace Salamon {

const string ZendParser::RGX_INSTANCEOF = "instanceof\\s+([A-Za-z0-9_]+)";
const string ZendParser::RGX_NEW = "new\\s+([A-Za-z0-9_]+)";
const string ZendParser::RGX_SIGNATURE = "([A-Za-z0-9_]+)\\s+\\$[a-zA-Z0-9_]+";
const string ZendParser::RGX_STATIC_CALL = "([A-Za-z0-9_]+)::";
const string ZendParser::RGX_MAIN_TYPE =
		"\\s*(class|interface)[\\s\n]+([A-Za-z0-9_]+)[\\s\n]*(extends[\\s\n]+([A-Za-z0-9_]+))?([\\s\n]*implements[\\s\n]+([A-Za-z0-9_ ,\\s\n]+))?[\\s\n]?\\{";
const string ZendParser::RGX_THROW_NEW = "throw new\\s+([A-Za-z0-9_]+)";

ZendParser::ZendParser() {
	reader = new DirectoryReader();
}

ZendParser::~ZendParser() {
	// TODO Auto-generated destructor stub
}

DirectoryReader* ZendParser::getReader() {
	return reader;
}

void ZendParser::setupReader() {
	reader->setPath(path.c_str());
}

void ZendParser::parse() {
	setupReader();
	getReader()->read();
	vector<DirectoryReader::Item> *results = reader->getResults();

	for (vector<DirectoryReader::Item>::iterator it = results->begin();
			it != results->end(); ++it) {
		File file = buildFile(&(*it));
//		cout<<file.isValid;
		if (file.isValid) {
			cout << file.fullPath << "\n";
			cout << file.mainType << "\n";
			cout << file.namespaceName << "\n";
		}
	}
}

vector<string> ZendParser::extractMainType(const string line) {
	vector<string> out;
	vector<string> tmp;
	Regexer r;
	tmp = r.findAll(line.c_str(), RGX_MAIN_TYPE, -1);
	out.push_back(tmp[2]);
	out.push_back(tmp[4]);
	out.push_back(tmp[6]);
	return out;
}

vector<string> ZendParser::extractTypes(const string line) {
	vector<string> out;
	vector<string> tmp;

	Regexer r;

	tmp = r.findAll(line.c_str(), RGX_NEW, 1);
	out.insert(out.begin(), tmp.begin(), tmp.end());
	tmp = r.findAll(line.c_str(), RGX_INSTANCEOF, 1);
	out.insert(out.begin(), tmp.begin(), tmp.end());
	tmp = r.findAll(line.c_str(), RGX_SIGNATURE, 1);
	out.insert(out.begin(), tmp.begin(), tmp.end());
	tmp = r.findAll(line.c_str(), RGX_STATIC_CALL, 1);
	out.insert(out.begin(), tmp.begin(), tmp.end());
	tmp = r.findAll(line.c_str(), RGX_THROW_NEW, 1);
	out.insert(out.begin(), tmp.begin(), tmp.end());

	return out;
}

ZendParser::File ZendParser::buildFile(DirectoryReader::Item* item) {

	File file;
	file.isValid = true;

	if (!item->isFile) {
		file.isValid = false;
		return file;
	}

	Strings s;
	file.name = item->name;
	file.fullPath = item->fullPath;
	file.content = this->reader->readTextFile(file.fullPath);

	vector<string> mainType = extractMainType(file.content);

	string mainTypeName = mainType[0];
	if (!mainTypeName.empty()) {
		file.mainType = mainTypeName;
	} else {
		file.isValid = false;
		file.content.clear();
		return file;
	}

	file.namespaceName = extractNamespace(file.mainType);
	file.types.push_back(file.mainType);

	string extends = mainType[1];
	string implements = mainType[2];

	if (!extends.empty()) {
		file.types.push_back(extends);
	}
	if (!implements.empty()) {
		vector<string> implementsTypes = s.split(",", implements);
		copy(implementsTypes.begin(), implementsTypes.end(),
				inserter(file.types, file.types.end()));

	}

	file.preparedTypes = generatePreparedTypes(
			filterTypes(extractTypes(file.content)));
//	for (const PreparedType& type : file.preparedTypes) {
//		cout << type.alias + "\n";
//	}

	return file;

}

const string ZendParser::extractNamespace(const string className) {
	string out;
	vector<string> tmp;
	Strings s;
	unsigned int i = 0;

	tmp = s.split("_", className);

	for (string& part : tmp) {
		if (i + 1 < tmp.size()) {
			out += part;
			if (i + 2 < tmp.size()) {
				out += "\\";
			}
		}
		i++;
	}

	return out;
}

vector<string> ZendParser::filterTypes(vector<string> types) {
	vector<string> typesFiltered;
	for (string& type : types) {
		size_t found = type.find("_");
		if (found != string::npos) {
			boost::trim(type);
			if (type.compare("require_once") == 0
					|| type.compare("include_once") == 0)
				continue;
			typesFiltered.push_back(type);
		}
	}

	sort(typesFiltered.begin(), typesFiltered.end());
	typesFiltered.erase(unique(typesFiltered.begin(), typesFiltered.end()),
			typesFiltered.end());

	sort(typesFiltered.begin(), typesFiltered.end(),
			[](const string& a, const string& b) -> bool
			{
				return a.size() > b.size();
			});

	return typesFiltered;
}

vector<ZendParser::PreparedType> ZendParser::generatePreparedTypes(
		vector<string> types) {
	vector<PreparedType> out;
	vector<vector<string>> tmp;
	set<string> overlapping;
	Strings s;

	for (string& type : types) {
		tmp.push_back(s.split("_", type));
	}

	for (vector<string>& type : tmp) {
		string className = type[type.size() - 1];
		int count = 0;
		for (vector<string>& type2 : tmp) {
			string className2 = type2[type2.size() - 1];
			if (className.compare(className2) == 0) {
				count++;
			}
		}

		if (count > 1) {
			overlapping.insert(className);
		}
	}

	for (vector<string>& type : tmp) {
		string className = type[type.size() - 1];
		PreparedType preparedType;
		preparedType.type = accumulate(type.begin(), type.end(), string("_"));
		if (overlapping.find(className) != overlapping.end()) {
			preparedType.alias = type[type.size() - 2] + type[type.size() - 1];
		} else {
			preparedType.alias = type[type.size() - 1];
		}
		preparedType.usage = extractNamespace(preparedType.type);
		out.push_back(preparedType);
	}
	return out;
}

} /* namespace Salamon */

