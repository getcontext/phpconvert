/*
 * PhpParser.cpp
 *
 *  Created on: 3 May 2014
 *      Author: Andrzej phpconvert <andrzej.salamon@gmail.com>
 */

#include "ZendParser.h"

namespace phpconvert {

    const string ZendParser::RGX_INSTANCEOF = "instanceof\\s+([A-Za-z0-9_]+)";
    const string ZendParser::RGX_NEW = "new\\s+([A-Za-z0-9_]+)";
    const string ZendParser::RGX_SIGNATURE = "([A-Za-z0-9_]+)\\s+\\$[a-zA-Z0-9_]+";
    const string ZendParser::RGX_STATIC_CALL = "([A-Za-z0-9_]+)::";
    const string ZendParser::RGX_MAIN_TYPE = "(\\s*(final|abstract)?"
                                             "[\\s\n]*(class|interface)[\\s\n]+([A-Za-z0-9_]+)"
                                             "[\\s\n]*(extends[\\s\n]+([A-Za-z0-9_]+)[\\s\n]*)?"
                                             "([\\s\n]*implements[\\s\n]+([A-Za-z0-9_ ,\\s\n]+))?[\\s\n]?\\{)";

    const char *ZendParser::RGX_BUILTIN_TYPE =
            "([^\\\\/_\[:alnum:]])(%s)([^\\\\/_\[:alnum:]])";

//const char* ZendParser::RGX_TYPE =
//		"([^\\\\/_\[:alnum:]])(%s)([^\\\\/_\[:alnum:]])";

    const string ZendParser::RGX_EXCLUDE_COMMENT = "^((?!\\s*\\\\|/\\*|\\*)";

    ZendParser::ZendParser() {
        reader = new DirectoryReader();
        strings = new Strings();

        results = new vector<File>();
        typesRegistry = new vector<PreparedType>();
        typesRegistryUnfiltered = new vector<PreparedType>();

        readBuiltInTypes();
        readKeywords();
    }

    ZendParser::~ZendParser() {
        delete reader;
        delete strings;
        delete results;
        delete typesRegistry;
        delete typesRegistryUnfiltered;
    }

    DirectoryReader *ZendParser::getReader() {
        return reader;
    }

    void ZendParser::setupReader() {
        reader->setPath(sourceDir.c_str());
    }

    void ZendParser::addNamespace(File &file) {
        if (file.mainType.empty())
            return;
        string tmp = "<?php\n\nnamespace " + file.namespaceName + ";\n\n";
        string rep = "<?php"; //@todo fix, make it array, add "<?" , it is also beginning of php files
        //@commit extract isPhp() method
        this->strings->replace(file.content, rep, tmp);
    }

    void ZendParser::replaceTypesBuiltIn(File &file) {
        string replaceFormat, builtInType, fileName, className;
        char out[250];
        vector<string> tmp;

        this->strings->split(tmp, ".", file.name);
        fileName = tmp[0];

        for (set<string>::iterator it = builtInTypes->begin();
             it != builtInTypes->end(); ++it) {
            builtInType = *it;
            if (builtInType.empty()
                || file.content.find(builtInType) == string::npos)
                continue;

            this->strings->split(tmp, "_", builtInType); //extract spacer like "_" (smiling a bit)
            className = tmp[tmp.size() - 1];

//		if (className.compare(builtInType) == 0
//				|| builtInType.compare(fileName) == 0
//				)
//				 {
//			continue;
//		}

//		for (PreparedType val : file.prepTypes) {
//
//			if ((!val.isMain && val.type.compare(builtInType) != 0)
//					&& this->builtInTypes->find(builtInType)
//							!= this->builtInTypes->end()) {
            sprintf(out, RGX_BUILTIN_TYPE, builtInType.c_str());
            string regexSearch(out);
            replaceFormat = "$1\\\\" + builtInType + "$3";

//				strings->regexReplace(val.replace, regexSearch, replaceFormat);
//
//				strings->regexReplace(file.content, val.raw, val.replace);

            strings->regexReplace(file.content, regexSearch, replaceFormat);
            strings->regexReplace(file.firstMainTypeFull, regexSearch,
                                  replaceFormat);
//			}
//		}
        }
    }

    void ZendParser::addUsages(File &file, set<string> tmpSet) {
        if (file.mainType.empty())
            return;
        tmpSet.clear();
        string replace = "\n\n\n";
        for (PreparedType &type : file.prepTypes) {
            if (type.type.compare(file.mainType) != 0) {
                if (tmpSet.find(type.type) != tmpSet.end()) {
                    continue;
                }
                replace += "use " + type.usage + " as " + type.alias + ";\n";
                tmpSet.insert(type.type);
            }
        }
        replace += "\n\n\n";
        replace += file.firstMainTypeFull;
        this->strings->replace(file.content, file.firstMainTypeFull, replace);
    }

    void ZendParser::replaceType(PreparedType &type, File &file) {
        string replaceFormat;
        char out[250];
//	regexSearch = "([^'\"])(" + type.type + ")([^'\"])";
//	replaceFormat = "$1 " + type.alias + " $3";

        sprintf(out, RGX_BUILTIN_TYPE, type.type.c_str());
        string regexSearch(out);
        replaceFormat = "$1" + type.alias + "$3";

//	cout << regexSearch + "-"+
//	replaceFormat + "\n";

        strings->regexReplace(file.content, regexSearch, replaceFormat);

//	strings->regexReplace(file.content, regexSearch, replaceFormat);
    }

    void ZendParser::replaceTypes(File &file) {
        vector<PreparedType> tmp;
        for (PreparedType &type : file.prepTypes) {
//		cout << type.type + "-" +type.alias + "\n";
            if (this->builtInTypes->find(type.type) == this->builtInTypes->end()) {
                replaceType(type, file);
            }
        }
    }

    void ZendParser::replaceTypesMain(File &file) {
        vector<string> tmp;
        for (PreparedType &type : file.prepTypesMain) {
            type.alias = type.type;
            replaceType(type, file);
        }
    }

    void ZendParser::replaceTypesGlobal(File &file) {
        PreparedType typeCopy;
        vector<PreparedType>::iterator type = typesRegistry->begin();
        string replaceFormat;
        char out[250];
        for (; type != typesRegistry->end(); ++type) {
            typeCopy = *type;
            if (file.content.find(typeCopy.type) == string::npos
                || this->builtInTypes->find(typeCopy.type)
                   != this->builtInTypes->end()) {
                continue;
            }

            sprintf(out, RGX_BUILTIN_TYPE, typeCopy.type.c_str());
            string regexSearch(out);
            replaceFormat = "$1" + typeCopy.alias + "$3";

            strings->regexReplace(file.content, regexSearch, replaceFormat);
//				strings->regexReplace(file.firstMainTypeFull, regexSearch,
//						replaceFormat);

//				strings->regexReplace(val.replace, regexSearch, replaceFormat);
//
//				strings->regexReplace(file.content, val.raw, val.replace);

//				this->strings->replace(file.content, typeCopy.type, typeCopy.alias);
        }
    }

    void ZendParser::writeTypesRegistryFile() {
        string typesRegistryString;
        vector<PreparedType>::iterator type = typesRegistry->begin();
        for (; type != typesRegistry->end(); ++type) {
            typesRegistryString += (*type).type + " | " + (*type).raw + "\n";
        }
        getReader()->writeTextFile(outputDir + DirectoryReader::getDirectorySeparator() + "typesregistry.txt",
                                   typesRegistryString);
    }

    void ZendParser::buildFiles(File file, vector<string> tmpOut, int &processed,
                                vector<DirectoryReader::Item> *readerResult,
                                vector<pair<string, string> > &tmpOutPairs, vector<string> &tmpVector) {
        for (vector<DirectoryReader::Item>::iterator it = readerResult->begin();
             it != readerResult->end(); ++it) {
            if (!it->isFile ||
                it->name.find(".php") == string::npos) { //@todo find php3 and all possible native built in extensions
                continue;
            }

//		if (it->name.compare("Select.php") != 0) {
//			continue;
//		}

            file = buildFile(&(*it), tmpOutPairs, tmpOut, tmpVector);
            processed++;
            if (!file.isValid) {
                continue;
            }
            file.rootPath = it->dir;
            for (PreparedType type : file.prepTypes) {
                typesRegistryUnfiltered->push_back(type);
            }
            for (PreparedType type : file.prepTypesMain) {
                typesRegistryUnfiltered->push_back(type);
            }
            results->push_back(file);
        }
    }

    void ZendParser::writeFiles(const set<string> &tmpSet, int &generated) {
        generated = 0;
        File fileCopy;
        for (vector<ZendParser::File>::iterator file = results->begin();
             file != results->end(); ++file) {
//				if (file->name.compare("View.php") != 0) {
//					continue;
//				}
//		if (file->name.compare("Role.php") != 0) {
//			continue;
//		}
//				if (file->name.compare("Registry.php")) {
//					continue;
//				}
//		if (file->name.compare("Exception.php")) {
//			continue;
//		}
//		if (file->name.compare("Initializer.php")) {
//			continue;
//		}
//		if (file->name.compare("Select.php")) {
//			continue;
//		}
            fileCopy = *file;
            getReader()->createDir(outputDir + DirectoryReader::getDirectorySeparator() + file->rootPath);
            replaceTypesBuiltIn(fileCopy);
//		cout << fileCopy.mainType << "\n";
            if (fileCopy.mainType.length() > 0) {

                addNamespace(fileCopy);
                addUsages(fileCopy, tmpSet);
                replaceTypes(fileCopy);
            }

//            replaceTypesGlobal(fileCopy);

            getReader()->writeTextFile(
                    outputDir + DirectoryReader::getDirectorySeparator() + fileCopy.rootPath + fileCopy.name,
                    fileCopy.content);
            generated++;
        }
    }

    void ZendParser::parse() {
        setupReader();

        getReader()->removeDir(outputDir);
        getReader()->createDir(outputDir);

        if (isRecurisve())
            getReader()->read(getReader()->getPath(), "/"); //fix it for windows
        else
            getReader()->read();

        vector<DirectoryReader::Item> *readerResult = reader->getResults();

        int generated = 0, processed = 0;
        vector<string> tmpOut, tmpVector;
        set<string> tmpSet;
        vector<pair<string, string>> tmpOutPairs;
        File file;

        buildFiles(file, tmpOut, processed, readerResult, tmpOutPairs, tmpVector);

        filterPreparedTypes(*typesRegistryUnfiltered, *typesRegistry);
        generatePreparedTypesGlobal(tmpVector);

        writeFiles(tmpSet, generated);

        writeTypesRegistryFile();

        cout << "\n";
        cout << "files processed : " << processed << "\n";
        cout << "types found : " << typesRegistry->size() << "\n";
        cout << "files generated : " << generated << "\n";
        cout << "\n";
        cout << "finished...";
    }

    void ZendParser::extractTypes(const string &line,
                                  vector<pair<string, string>> &out, vector<string> &tmp) {
        vector<string> tmpOut;
        const string lineStripped = stripCmments(line);

//	cout << "--NEW--" << "\n";
        regexer->findAll(tmp, lineStripped, RGX_NEW, -1);
//	for(string v:tmp) {
//		cout << v << "\n";
//	}
        tmpOut.insert(tmpOut.begin(), tmp.begin(), tmp.end());


        regexer->findAll(tmp, lineStripped, RGX_INSTANCEOF, -1);
        tmpOut.insert(tmpOut.begin(), tmp.begin(), tmp.end());

        regexer->findAll(tmp, lineStripped, RGX_STATIC_CALL, -1);
        tmpOut.insert(tmpOut.begin(), tmp.begin(), tmp.end());

        regexer->findAll(tmp, lineStripped, RGX_SIGNATURE, -1);
        tmpOut.insert(tmpOut.begin(), tmp.begin(), tmp.end());


        generatePairs(out, tmpOut);


//	for(pair<string, string> v:out) {
//		cout << v.second << "\n";
//	}
//	for(string v:tmpOut) {
//		cout << v << "\n";
//	}
    }

    void ZendParser::prepareTypes(File &file, vector<pair<string, string>> &out,
                                  vector<string> &tmp) {
//	cout << "file: " <<file.name << " ---------- \n";
        extractTypes(file.content, out, tmp);
        PreparedType prepType;
        for (pair<string, string> pair : out) {

            prepType.type = pair.first;
            prepType.raw = pair.second;
            prepType.replace = pair.second;
//		cout << "first:" << pair.first << ",second:" << pair.second << "\n";
            file.prepTypes.push_back(prepType);
        }
        vector<PreparedType> outPrep;
        filterPreparedTypes(file.prepTypes, outPrep);
        file.prepTypes = outPrep;
        generatePreparedTypes(file, tmp);
    }

    void ZendParser::extractMainType(File &file, vector<string> &out,
                                     vector<string> &tmp) {
        out.clear();
        tmp.clear();
        int step = 9;
        regexer->findAll(tmp, file.content.c_str(), RGX_MAIN_TYPE, -1);
//	if (file.name.compare("Role.php") == 0) {
//		cout << "\n";
//	}
//	int i = 0;
//	for (string v : tmp) {
//		cout << i++ << ":" << v << "\n";
//	}

        size_t size = tmp.size();
        if (size > 0) {
            for (size_t i = 0; i < size - 1; i += step) {
                out.push_back(tmp[0]);
                out.push_back(tmp[4]); //class|interface

                out.push_back(tmp[5]);
                out.push_back(tmp[6]); //extends

                out.push_back(tmp[7]);
                out.push_back(tmp[8]); //implements
            }
        }
    }

    void ZendParser::prepareTypesMain(File &file, vector<string> &out,
                                      vector<string> &tmp) {
        out.clear();
        tmp.clear();

        file.isValid = false;

        extractMainType(file, out, tmp);

        if (out.size() <= 0) {
            return;
        }

        file.isValid = true;
        string className, fileName, mainTypeFull, mainType, extends, implements,
                delim;
        size_t found, size;

        size = out.size();
        int step = 6;
        PreparedType tmpPrepType;
        PreparedType prepType;
        prepType.isMain = true;
        for (unsigned int i = 0; i < size - 1; i += step) {
            mainTypeFull = out[i];
            mainType = out[i + 1];
            found = mainType.find("_");
            extends = out[i + 3];
            implements = out[i + 5];

            prepType.type = mainType;
            prepType.raw = mainTypeFull;
            prepType.replace = mainTypeFull;

            this->strings->split(tmp, "_", mainType);
            className = tmp[tmp.size() - 1];

            this->strings->split(tmp, ".", file.name);
            fileName = tmp[0];

            if (!extends.empty()) {
                prepType.extends = extends;
                tmpPrepType.isMain = false;
                tmpPrepType.type = extends;
                file.prepTypes.push_back(tmpPrepType);
            }

            if (!implements.empty()) {
                this->strings->split(prepType.implements, ",", implements);
                for (string v : prepType.implements) {
                    tmpPrepType.isMain = false;
                    tmpPrepType.type = v;
                    file.prepTypes.push_back(tmpPrepType);
                }
            }

            //make first class in file having usages above
            if (!i) {
                file.firstMainTypeFull = mainTypeFull;
            }

            if ((className.compare(fileName) == 0 || mainType.compare(fileName) == 0)
                && found != string::npos) {
                file.mainType = mainType;
                file.firstMainTypeFull = mainTypeFull;
            }

            file.prepTypes.push_back(prepType);
            file.prepTypesMain.push_back(prepType);
        }

        if (file.mainType.length() <= 0) {
            for (PreparedType t : file.prepTypesMain) {
                this->builtInTypes->insert(t.type);
            }
        }
    }

    ZendParser::File ZendParser::buildFile(DirectoryReader::Item *item,
                                           vector<pair<string, string>> &out, vector<string> &tmpOut,
                                           vector<string> &tmp) {
        File file;
        file.isValid = false;

        if (!item->isFile) {
            return file;
        }

        file.name = item->name;
        file.fullPath = item->fullPath;
        file.content = this->reader->readTextFile(item->fullPath);
        file.mainTypes = new set<string>();

        prepareTypesMain(file, tmpOut, tmp);

        if (!file.isValid) {
            file.isValid = true;
            return file;
        }

        if (!file.mainType.empty())
            extractNamespace(file.mainType, file.namespaceName, tmp);

        prepareTypes(file, out, tmp);

        return file;
    }

    void ZendParser::extractNamespace(const string &className, string &out,
                                      vector<string> &tmp) {
        tmp.clear();
        out.clear();
        unsigned int i = 0;

        strings->split(tmp, "_", className);

        for (string &part : tmp) {
            if (i + 1 < tmp.size()) {
                out += part;
                if (i + 2 < tmp.size()) {
                    out += "\\";
                }
            }
            i++;
        }
    }

    void ZendParser::generateNamespace(const string &className, string &out) {
        out.clear();
        vector<string> tmp;
        unsigned int i = 0;

        this->strings->split(tmp, "_", className);

        for (string &part : tmp) {
            out += part;
            if (i + 1 < tmp.size()) {
                out += "\\";
            }
            i++;
        }
    }

    void ZendParser::sortFaster(vector<PreparedType> &out) {
        set<PreparedType> foos(out.begin(), out.end());
        out.clear();
        std::set<PreparedType>::iterator it;
        for (it = foos.begin(); it != foos.end(); ++it) {
            out.push_back(*it);
        }
    }

    void ZendParser::sortSlower(vector<PreparedType> &out) {
        sort(out.begin(), out.end());
        out.erase(unique(out.begin(), out.end()), out.end());
        sort(out.begin(), out.end());
//	sort(out.begin(), out.end(),
//			[](const PreparedType& a, const PreparedType& b) -> bool
//			{
//				return a.type.size() > b.type.size();
//			});
    }

    void ZendParser::filterPreparedTypes(vector<PreparedType> &types,
                                         vector<PreparedType> &out) {
        out.clear();
        size_t found;
        string typeCopy;
        vector<string> duplicates;
        vector<string>::iterator it;
        for (PreparedType &type : types) {
            boost::trim(type.type);
            if (type.type.empty())
                continue;
            typeCopy = type.type;
            transform(typeCopy.begin(), typeCopy.end(), typeCopy.begin(),
                      ::tolower);
            type.typeLower = typeCopy;
            found = typeCopy.find("_");
            it = find(duplicates.begin(), duplicates.end(), typeCopy);
            if ((it != duplicates.end()
                 || this->keywords->find(typeCopy) != this->keywords->end()
                 || this->builtInTypes->find(type.type)
                    != this->builtInTypes->end() || found == string::npos)) {
                continue;
            }

            out.push_back(type);
            duplicates.push_back(typeCopy);
        }

//	sortFaster(out);

        sortSlower(out);
    }

    void ZendParser::generatePairs(vector<pair<string, string> > &out,
                                   vector<string> &src) {
        out.clear();
        for (unsigned int i = 0; i < src.size(); i += 2) {
            out.push_back(make_pair(src[i + 1], src[i]));
        }
    }

    string ZendParser::generateAlias(vector<string> &type, unsigned int parts) {
        if (type.size() <= 0)
            return "";
        if (type.size() == 1)
            return type[0];

        unsigned int limit = type.size() - 1;
        unsigned int start = limit - (parts - 1);
        string alias;

        if (parts <= 0 || parts >= type.size())
            start = 0;

        while (start <= limit) {
            alias += type[start];
            start++;
        }
        return alias;
    }

    string ZendParser::generateAlias(string &type, unsigned int parts,
                                     vector<string> &tmp) {
        tmp.clear();

        strings->split(tmp, "_", type);

        return generateAlias(tmp, parts);
    }

    void ZendParser::generatePreparedTypes(File &file, vector<string> &tmpVector) {
        tmpVector.clear();

        set<string> overlapping;
        stringstream stream;
        string className, classNameLower, tmpString, tmpClassNameLower;
//	PreparedType preparedType;
        size_t size;

        for (PreparedType type : file.prepTypes) {
            className = generateAlias(type.type, 1, tmpVector);
            classNameLower = className;
            transform(classNameLower.begin(), classNameLower.end(),
                      classNameLower.begin(), ::tolower);
            int count = 0;
            for (PreparedType typeCompared : file.prepTypes) {
                tmpString = generateAlias(typeCompared.type, 1, tmpVector);
                tmpClassNameLower = tmpString;
                transform(tmpClassNameLower.begin(), tmpClassNameLower.end(),
                          tmpClassNameLower.begin(), ::tolower);
                if (classNameLower.compare(tmpClassNameLower) == 0) {
                    count++;
                }
            }
            if (count > 1
                || this->builtInTypes->find(className)
                   != this->builtInTypes->end()) {
                overlapping.insert(className);
            }
        }

        for (PreparedType &preparedType : file.prepTypes) {

            strings->split(tmpVector, "_", preparedType.type);

            className = tmpVector[tmpVector.size() - 1];

            stream.str(string());
            stream.clear();
            copy(tmpVector.begin(), tmpVector.end(),
                 std::ostream_iterator<string>(stream, "_"));
            preparedType.type = stream.str().substr(0, stream.str().length() - 1);

            tmpClassNameLower = className;
            transform(tmpClassNameLower.begin(), tmpClassNameLower.end(),
                      tmpClassNameLower.begin(), ::tolower);

            if (!hasMainType(file)) {
                processFileProcedural(file, tmpString, tmpClassNameLower, preparedType, tmpVector, stream);
            } else {
//                cout << "file: " + file.name + " has no classes/interfaces\n";
                processFileObjectOriented(file, overlapping, className, tmpClassNameLower, size, preparedType,
                                          tmpVector, stream);
            }
        }

    }

    void
    ZendParser::processFileObjectOriented(const BaseParser::File &file, set<string> &overlapping,
                                          const string &className,
                                          const string &tmpClassNameLower, size_t size,
                                          BaseParser::PreparedType &preparedType, vector<string> &tmpVector,
                                          stringstream &stream) {
        if (isKeyword(tmpClassNameLower)
            && !isMainType(file, preparedType)) {
            if (tmpVector.size() == 2) {
                size = 2;
            } else {
                size = tmpVector.size() - 1;
            }
            preparedType.alias = generateAlias(tmpVector, size);
            stream.str(string());
            stream.clear();
            copy(tmpVector.begin(), tmpVector.end() - 1,
                 ostream_iterator<string>(stream, "_"));
            generateNamespace(
                    stream.str().substr(0, stream.str().length() - 1) + "_"
                    + generateAlias(tmpVector, 2), preparedType.usage);

        } else if (isMainType(file, preparedType)) {
            if (isKeyword(tmpClassNameLower)) {
                size = 2;
            } else {
                size = 1;
            }
            preparedType.alias = generateAlias(tmpVector, size);
            preparedType.usage = "";
        } else if (builtInTypes->find(preparedType.type)
                   != builtInTypes->end()) {
            preparedType.alias = "\\\\" + preparedType.type;
            preparedType.usage = "";
        } else if (overlapping.find(className) != overlapping.end()) {
            if (tmpVector.size() == 2) {
                size = 2;
            } else {
                size = tmpVector.size() - 1;
            }
            preparedType.alias = generateAlias(tmpVector, size);
            generateNamespace(preparedType.type, preparedType.usage);
        } else {
            preparedType.alias = generateAlias(tmpVector, 1);
            generateNamespace(preparedType.type, preparedType.usage);
        }
    }

    void
    ZendParser::processFileProcedural(const BaseParser::File &file, string &tmpString, const string &tmpClassNameLower,
                                      BaseParser::PreparedType &preparedType, vector<string> &tmpVector,
                                      stringstream &stream) {
        if (isBuiltInType(preparedType)) {
            preparedType.alias = "\\\\" + preparedType.type;
        } else if (isInMainTypes(file, preparedType)) {
            preparedType.alias = preparedType.type;
            builtInTypes->insert(preparedType.type);
        } else if (isKeyword(tmpClassNameLower)) {
            string alias = generateAlias(tmpVector, 2);
            stream.str(string());
            stream.clear();
            copy(tmpVector.begin(), tmpVector.end() - 1,
                 ostream_iterator<string>(stream, "_"));

            generateNamespace(
                    stream.str().substr(0, stream.str().length() - 1) + "_"
                    + alias, tmpString);

            preparedType.alias = "\\" + tmpString;
//				    cout <<"isKeyword alias: "<<preparedType.alias<<"\n";
        } else {
//                    cout <<"standard type alias: \n";
            generateNamespace(preparedType.type, preparedType.alias);
            preparedType.alias = "\\" + preparedType.alias;
//				    cout <<preparedType.alias<<"\n";
        }
    }

    bool ZendParser::isMainType(const BaseParser::File &file,
                                const BaseParser::PreparedType &preparedType) const { return preparedType.type.compare(file.mainType) == 0; }

    bool ZendParser::isKeyword(const string &tmpClassNameLower) {
        return keywords->find(tmpClassNameLower)
               != keywords->end();
    }

    bool ZendParser::hasMainType(const BaseParser::File &file) const { return !file.mainType.empty(); }

    bool ZendParser::isInMainTypes(const BaseParser::File &file, const BaseParser::PreparedType &preparedType) const {
        return file.mainTypes->find(preparedType.type)
                       != file.mainTypes->end();
    }

    bool ZendParser::isBuiltInType(const BaseParser::PreparedType &preparedType) {
        return builtInTypes->find(preparedType.type)
               != builtInTypes->end();
    }

    void ZendParser::generatePreparedTypeFull(PreparedType &outPrep,
                                              vector<string> &tmpVect) {
        if (outPrep.type.find("_") == string::npos) {
            outPrep.alias = "\\" + outPrep.type; //extract const - ns sep.
            return;
        }

        stringstream stream;
        string tmp;
        size_t size = 1;

        this->strings->split(tmpVect, "_", outPrep.type);

        tmp = tmpVect[tmpVect.size() - 1];
        transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);

        if (this->keywords->find(tmp) != this->keywords->end()) {
            size = 2;
        }

        extractNamespace(outPrep.type, tmp, tmpVect);
        tmp += "_" + generateAlias(outPrep.type, size, tmpVect);
        generateNamespace(tmp, outPrep.alias);

        outPrep.alias = "\\" + outPrep.alias;
    }

    void ZendParser::generatePreparedTypesGlobal(vector<string> &tmp) {
        vector<PreparedType>::iterator type = typesRegistry->begin();
        for (; type != typesRegistry->end(); ++type) {
            generatePreparedTypeFull(*type, tmp);
        }
    }

    void ZendParser::readBuiltInTypes() {
        vector<string> v;
        strings->split(v, "\n", getReader()->readTextFile("builtInTypes.txt"));
        this->builtInTypes = new set<std::string>(v.begin(), v.end());
    }

    void ZendParser::readKeywords() {
        vector<string> v;
        strings->split(v, "\n", getReader()->readTextFile("keywords.txt"));
        this->keywords = new set<std::string>(v.begin(), v.end());
    }

/**
 * @todo refactor it with parsing comments. server......
 *
 */
    std::string ZendParser::stripCmments(std::string const &input) {
        std::string output;
        typedef boost::wave::cpplexer::lex_token<> token_type;
        typedef boost::wave::cpplexer::lex_iterator<token_type> lexer_type;
        typedef token_type::position_type position_type;

        position_type pos;

        lexer_type it = lexer_type(input.begin(), input.end(), pos,
                                   boost::wave::language_support(
                                           boost::wave::support_cpp
                                           | boost::wave::support_option_long_long));
        lexer_type end = lexer_type();

        for (; it != end; ++it) {
            if (*it != boost::wave::T_CCOMMENT
                && *it != boost::wave::T_CPPCOMMENT) {
                output += std::string(it->get_value().begin(),
                                      it->get_value().end());
            }
        }
        return output;
    }
} /* namespace Salamon */

