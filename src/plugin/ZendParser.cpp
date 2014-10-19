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
		"(final|abstract)?[\\s\n]*(class|interface)[\\s\n]+([A-Za-z0-9_]+)[\\s\n]*(extends[\\s\n]+([A-Za-z0-9_\\s\n]+))?([\\s\n]*implements[\\s\n]+([A-Za-z0-9_ ,\\s\n]+))?[\\s\n]?\\{";
const string ZendParser::RGX_THROW_NEW = "throw new\\s+([A-Za-z0-9_]+)";

ZendParser::ZendParser() {
	reader = new DirectoryReader();
//	builtInTypes = new set<string>();
	readBuiltInTypes();
}

ZendParser::~ZendParser() {
	// TODO Auto-generated destructor stub
}

DirectoryReader* ZendParser::getReader() {
	return reader;
}

void ZendParser::setupReader() {
	reader->setPath(sourceDir.c_str());
}

void ZendParser::parse() {
	setupReader();
	getReader()->read(getReader()->getPath(), "/");
	vector<DirectoryReader::Item> *results = reader->getResults();
	getReader()->createDir(outputDir);
	Strings s;
	int processed = 0;
	for (vector<DirectoryReader::Item>::iterator it = results->begin();
			it != results->end(); ++it) {
//		if (it->name.compare("Exception.php") != 0) {
//			continue;
//		}
		if (!it->isFile) {
			continue;
		}

		File file = buildFile(&(*it));

		if (!file.isValid) {
			continue;
		}
//		cout << file.fullPath << "\n";
		string tmp;

		getReader()->createDir(outputDir + it->dir);

		for (PreparedType& type : file.preparedTypes) {
			if (type.type.compare(file.mainType) != 0
					&& this->builtInTypes.find(type.type)
							!= this->builtInTypes.end()) {
				const char* formatString;
				string r;
				formatString = "$1\\\\$2$3";
				r = "([^\\\\/_[:alnum:]])(" + type.type + ")([^[:alnum:]])";
				file.content = s.regexReplace(file.content, r, formatString);
			}

		}

		tmp = "<?php\n\nnamespace " + file.namespaceName + ";\n\n";
		file.content = s.replace(file.content, "<?php", tmp);

		tmp = "\n\n\n";
		for (PreparedType& type : file.preparedTypes) {
			if (type.type.compare(file.mainType) != 0
					&& this->builtInTypes.find(type.type)
							== this->builtInTypes.end()) {
				tmp += "use " + type.usage + " as " + type.alias + ";\n";
			}
		}
		tmp += "\n\n\n";
		file.content = s.replace(file.content, file.mainTypeFull,
				tmp + file.mainTypeFull);

		for (PreparedType& type : file.preparedTypes) {
			if (this->builtInTypes.find(type.type)
					== this->builtInTypes.end()) {
				file.content = s.replace(file.content, type.type, type.alias);
			}
		}

		getReader()->writeTextFile(outputDir + it->dir + file.name,
				file.content);

		processed++;
		cout << "finished : " + file.fullPath + "\n";
//		break;
	}

	cout << "Total files processed : " << processed << "\n\n";

	cout << "finished...";
}

vector<string> ZendParser::extractMainType(const string line) {
	vector<string> out;
	vector<string> tmp;
	Regexer r;
	tmp = r.findAll(line.c_str(), RGX_MAIN_TYPE, -1);
	if (tmp.size() > 0) {
		out.push_back(tmp[0]);
		out.push_back(tmp[3]);
		out.push_back(tmp[5]);
		out.push_back(tmp[7]);
	}
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
	if (mainType.size() <= 0) {
		file.isValid = false;
		return file;
	}

	string mainTypeName = mainType[1];
	if (!mainTypeName.empty()) {
		file.mainType = mainTypeName;
	} else {
		file.isValid = false;
		file.content.clear();
		return file;
	}

	copy(builtInTypes.begin(), builtInTypes.end(),
			inserter(file.types, file.types.end()));

	file.mainTypeFull = mainType[0];
	file.namespaceName = extractNamespace(file.mainType);
	file.types.push_back(file.mainType);

	string extends = mainType[2];
	if (!extends.empty()) {
		file.types.push_back(extends);
	}

	string implements = mainType[3];
	if (!implements.empty()) {
		vector<string> implementsTypes = s.split(",", implements);
		copy(implementsTypes.begin(), implementsTypes.end(),
				inserter(file.types, file.types.end()));

	}

	vector<string> prepTypes = extractTypes(file.content);
	file.types.insert(file.types.begin(), prepTypes.begin(), prepTypes.end());
	file.types = filterTypes(file.types);
	file.preparedTypes = generatePreparedTypes(file.types, file.mainType);

//	file.content.clear();

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

const string ZendParser::toNamespace(const string className) {
	string out;
	vector<string> tmp;
	Strings s;
	unsigned int i = 0;

	tmp = s.split("_", className);

	for (string& part : tmp) {
		out += part;
		if (i + 1 < tmp.size()) {
			out += "\\";
		}
		i++;
	}

	return out;
}

vector<string> ZendParser::filterTypes(vector<string> types) {
	vector<string> typesFiltered;
	for (string& type : types) {
		boost::trim(type);
		if (type.empty())
			continue;
		size_t found = type.find("_");
		if (found != string::npos) {
			if (type.compare("require_once") == 0
					|| type.compare("require") == 0
					|| type.compare("include_once") == 0
					|| type.compare("include") == 0)
				continue;
			typesFiltered.push_back(type);
		} else if (this->builtInTypes.find(type) != this->builtInTypes.end()) {
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
		vector<string>& types, string mainType) {
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

		if (count > 1
				|| this->builtInTypes.find(className)
						!= this->builtInTypes.end()) {
			overlapping.insert(className);
		}
	}

	for (vector<string>& type : tmp) {
		string className = type[type.size() - 1];
		PreparedType preparedType;
		stringstream stream;
		copy(type.begin(), type.end(),
				std::ostream_iterator<string>(stream, "_"));
		preparedType.type = stream.str().substr(0, stream.str().length() - 1);

		if (className.compare("Interface") == 0
				|| className.compare("Abstract") == 0) {
			stream.str(string());
			stream.clear();
			preparedType.alias = type[type.size() - 2] + type[type.size() - 1];
			copy(type.begin(), type.end() - 1,
					std::ostream_iterator<string>(stream, "_"));
			preparedType.usage = toNamespace(
					stream.str().substr(0, stream.str().length() - 1) + "_"
							+ preparedType.alias);
		} else if (overlapping.find(className) != overlapping.end()
				&& preparedType.type.compare(mainType) != 0
				&& this->builtInTypes.find(preparedType.type)
						== this->builtInTypes.end()) {
			preparedType.alias = type[type.size() - 2] + type[type.size() - 1];
			preparedType.usage = toNamespace(preparedType.type);
		} else if (this->builtInTypes.find(preparedType.type)
				!= this->builtInTypes.end()) {
			preparedType.alias = "\\" + preparedType.type;
		} else {
			preparedType.alias = type[type.size() - 1];
			preparedType.usage = toNamespace(preparedType.type);
		}

		out.push_back(preparedType);

	}
	return out;
}

void ZendParser::readBuiltInTypes() {
	string rs = getReader()->readTextFile("builtintypes.txt");
	vector<string> v = (new Strings())->split("\n", rs);
	set<std::string> s(v.begin(), v.end());
//	for (string t : s) {
//		t = " " + t;
//	}
	this->builtInTypes = s;
}

} /* namespace Salamon */

