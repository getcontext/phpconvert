/*
 * PhpParser.cpp
 *
 *  Created on: 3 May 2014
 *      Author: Andrzej phpconvert <andrzej.salamon@gmail.com>
 */

#include "ZendParser.h"

namespace phpconvert {
//make them static
    const string ZendParser::RGX_INSTANCEOF = "instanceof\\s+([A-Za-z0-9_]+)";
    const string ZendParser::RGX_NEW = "new\\s+([A-Za-z0-9_]+)";
    const string ZendParser::RGX_SIGNATURE = "([A-Za-z0-9_]+)\\s+\\$[a-zA-Z0-9_]+"; //make it variables + function signatures
    const string ZendParser::RGX_STATIC_CALL = "([A-Za-z0-9_]+)::";
    const string ZendParser::RGX_MAIN_TYPE = "(\\s*(final|abstract)?"
                                             "[\\s\n]*(class|interface)[\\s\n]+([A-Za-z0-9_]+)"
                                             "[\\s\n]*(extends[\\s\n]+([A-Za-z0-9_]+)[\\s\n]*)?"
                                             "([\\s\n]*implements[\\s\n]+([A-Za-z0-9_ ,\\s\n]+))?[\\s\n]?\\{)";

    const char *ZendParser::RGX_BUILTIN_TYPE =
            "([^\\\\/_\[:alnum:]])(%s)([^\\\\/_\[:alnum:]])";

//		"([^\\\\/_\[:alnum:]])(%s)([^\\\\/_\[:alnum:]])";

    const string ZendParser::RGX_EXCLUDE_COMMENT = "^((?!\\s*\\\\|/\\*|\\*)";

//const char* ZendParser::RGX_TYPE =
    static const char *const PHP_EXT = ".php"; //@todo move to header, can't be (php3, etc all extensions)

    static const char *const TYPES_REGISTRY_FILE_NAME = "typesregistry.txt";

    static const char *const PHP_TAG_OPEN = "<?php";

    static const char *const PHP_NAMESPACE_TAG_REPLACE = "<?php\n\nnamespace "; //@todo cant combine tag_open+tag_replace on compile time

    static const char *const NAMESPACE_SEPARATOR = "\\";

    static const char *const DELIMETER = "_";

    ZendParser::ZendParser() {
        reader = new DirectoryReader();
        stringHelper = new StringHelper();

        results = new vector<File>();
        typesRegistry = new vector<PreparedType>();
        typesRegistryUnfiltered = new vector<PreparedType>();

        readKeywords();
        readBuiltInTypes();
    }

    ZendParser::~ZendParser() { //make all pointers delete try catch (cascade)
        delete reader;
        delete stringHelper;
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
        string tmp = PHP_NAMESPACE_TAG_REPLACE + file.namespaceName + ";\n\n";
        string rep = PHP_TAG_OPEN; //@todo fix, make it array, add "<?" , it is also beginning of php files
        //@commit extract isPhp() method
        this->stringHelper->replace(file.content, rep, tmp);
    }

    void ZendParser::replaceTypesBuiltIn(File &file) {
        string replaceFormat, builtInType, fileName, className;
        char out[250];
        vector<string> tmp;

        this->stringHelper->split(tmp, ".", file.name);
        fileName = tmp[0];

        for (set<string>::iterator it = builtInTypes->begin();
             it != builtInTypes->end(); ++it) {
            builtInType = *it;
            if (builtInType.empty()
                || file.content.find(builtInType) == string::npos
                || isKeyword(builtInType))
                continue;

            this->stringHelper->split(tmp, DELIMETER, builtInType); //extract spacer like "_" (smiling a bit)
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

//				stringHelper->regexReplace(val.replace, regexSearch, replaceFormat);
//
//				stringHelper->regexReplace(file.content, val.raw, val.replace);

            stringHelper->regexReplace(file.content, regexSearch, replaceFormat);
            stringHelper->regexReplace(file.firstMainTypeFull, regexSearch,
                                       replaceFormat);
//			}
//		}
        }
    }

    void ZendParser::addUsages(File &file, set<string> tmpSet) {
        if (file.mainType.empty()) //is structural //@todo does not matter if oop or not
            return;
        tmpSet.clear();
        string replace = "\n\n\n";
        for (PreparedType &type: file.prepTypes) {
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
        //@todo this stuff doesn't work for inlined require include
        this->stringHelper->replace(file.content, file.firstMainTypeFull, replace);
    }

    void ZendParser::replaceType(PreparedType &type, File &file) {
        string replaceFormat;
        char out[250];
//	regexSearch = "([^'\"])(" + type.type + ")([^'\"])";
//	replaceFormat = "$1 " + type.alias + " $3";

        sprintf(out, RGX_BUILTIN_TYPE, type.type.c_str());
        string regexSearch(out);
        replaceFormat = "$1" + type.alias + "$3";
//debug(file, type, "", "replaceFormat: "+replaceFormat+", regexSearch: "+regexSearch+", replaceType");
//	cout << regexSearch + "-"+
//	replaceFormat + "\n";

        stringHelper->regexReplace(file.content, regexSearch, replaceFormat);

//	stringHelper->regexReplace(file.content, regexSearch, replaceFormat);
    }

    void ZendParser::replaceTypes(File &file) {
//        vector<PreparedType> tmp;
        for (PreparedType &type: file.prepTypes) {
//		cout << type.type + "-" +type.alias + "\n";
//            if (isBuiltInType(type)) {
            replaceType(type, file);
//            }
        }
    }

    void ZendParser::replaceTypesMain(File &file) {
//        vector<string> tmp;
        for (PreparedType &type: file.prepTypesMain) {
//            type.alias = type.type;
            replaceType(type, file);
        }
    }

    void ZendParser::replaceTypesGlobal(File &file) {
        PreparedType preparedTypeCopy;
        vector<PreparedType>::iterator type = typesRegistry->begin();
        string replaceFormat, typeLower;
        char out[250];
        for (; type != typesRegistry->end(); ++type) {
            preparedTypeCopy = *type;
            typeLower = preparedTypeCopy.type;
            if (isFound(file, preparedTypeCopy)
                || isBuiltInType(preparedTypeCopy)
                || isKeyword(typeLower)
                    ) {
                continue;
            }

            sprintf(out, RGX_BUILTIN_TYPE, preparedTypeCopy.type.c_str());
            string regexSearch(out);
            replaceFormat = "$1" + preparedTypeCopy.alias + "$3";

//            debug(file, preparedTypeCopy, "", "replaceFormat: "+replaceFormat+", regexSearch: "+regexSearch+", replaceType");

            stringHelper->regexReplace(file.content, regexSearch, replaceFormat);
//				stringHelper->regexReplace(file.firstMainTypeFull, regexSearch,
//						replaceFormat);

//				stringHelper->regexReplace(val.replace, regexSearch, replaceFormat);
//
//				stringHelper->regexReplace(file.content, val.raw, val.replace);

//				this->stringHelper->replace(file.content, preparedTypeCopy.type, preparedTypeCopy.alias);
        }
    }

    bool ZendParser::isFound(const BaseParser::File &file,
                             const BaseParser::PreparedType &typeCopy) const {
        return file.content.find(typeCopy.type) == string::npos;
    }

    void ZendParser::writeTypesRegistryFile() {
        string typesRegistryString;
        vector<PreparedType>::iterator type = typesRegistry->begin();
        for (; type != typesRegistry->end(); ++type) {
            typesRegistryString += "Type:" + (*type).type + "\n" +
                                   "Alias:" + (*type).alias + "\n"
//                                   + (*type).raw + "\n"
                    ;
        }
        getReader()->writeTextFile(outputDir + DirectoryReader::getDirectorySeparator() + TYPES_REGISTRY_FILE_NAME,
                                   typesRegistryString);
    }

    void ZendParser::buildFiles(File file, vector<string> tmpOut, int &processed,
                                vector<DirectoryReader::Item> *readerResult,
                                vector<pair<string, string> > &tmpOutPairs,
                                vector<string> &tmpVector) {
        vector<DirectoryReader::Item>::iterator it;
        for (it = readerResult->begin(); it != readerResult->end(); ++it) {
            if (!it->isFile ||
                it->name.find(PHP_EXT) == string::npos) { //@todo find php3 and all possible native built in extensions
                continue;
            }

            file = buildFile(&(*it), tmpOutPairs, tmpOut, tmpVector);
            processed++;
            if (!file.isValid) {
                continue;
            }
            file.rootPath = it->dir;
            for (PreparedType type: file.prepTypes) {
                typesRegistryUnfiltered->push_back(type);
            }
            for (PreparedType type: file.prepTypesMain) {
                typesRegistryUnfiltered->push_back(type);
            }
            results->push_back(file);
        }
    }

    void ZendParser::write(const set<string> &tmpSet, int &generated) {
        generated = 0;
        File fileCopy;
        vector<ZendParser::File>::iterator file;
        for (file = results->begin(); file != results->end(); ++file) {
//            if (file->name.compare("View.php") != 0) {
//                if (file->name.compare("Interface.php") != 0) {
//                    if (file->name.compare("Exception.php") != 0) {
//                        continue;
//                    }
//                }
//            }
//            else if (file->name.compare("Interface.php") != 0) {
//            }
//            else if (file->name.compare("Exception.php") != 0) {
//            }
//            else {
//                continue;
//            }

//		if (file->name.compare("Role.php") != 0) {
//			continue;
//		}
//        if (file->name.compare("Registry.php")) {
//            continue;
//        }
//		if (file->name.compare("Exception.php")!=0) {
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
            replaceTypesBuiltIn(fileCopy); //@todo problem here
            replaceTypesGlobal(fileCopy);
//		cout << fileCopy.mainType << "\n";
//            if (fileCopy.mainType.length() > 0) {

                addNamespace(fileCopy);
                addUsages(fileCopy, tmpSet);
                replaceTypes(fileCopy);
//            }


            getReader()->writeTextFile(
                    outputDir + DirectoryReader::getDirectorySeparator() + fileCopy.rootPath + fileCopy.name,
                    fileCopy.content);
            generated++;
        }
    }

    void ZendParser::parse() {
        setupReader();
//@todo add overwrite
//        getReader()->removeDir(outputDir);
//        getReader()->createDir(outputDir);

        if (isRecurisve())
            getReader()->read(getReader()->getPath(), DirectoryReader::getDirectorySeparator()); //fix it for windows
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

//        write(tmpSet, generated);
//
//        writeTypesRegistryFile();

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
        for (pair<string, string> pair: out) {

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

    //@todo isOOP(has class/if) - isProcedural (no classes/if) - has it sense ?

    void ZendParser::extractMainType(File &file, vector<string> &out,
                                     vector<string> &tmp) {
        out.clear();
        tmp.clear();
        int step = 9;

        const string lineStripped = stripCmments(file.content);
        regexer->findAll(tmp, lineStripped.c_str(), RGX_MAIN_TYPE, -1);
//	if (file.name.compare("Navigation.php") == 0) {
//		cout << "\n";
//        int i = 0;
//        for (string v : tmp) {
//            cout << i++ << ":" << v << "\n";
//        }
//    }

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
            found = mainType.find(DELIMETER);
            extends = out[i + 3];
            implements = out[i + 5];

            prepType.type = mainType;
            prepType.raw = mainTypeFull;
            prepType.replace = mainTypeFull;

            this->stringHelper->split(tmp, DELIMETER, mainType);
            className = tmp[tmp.size() - 1];

            this->stringHelper->split(tmp, ".", file.name);
            fileName = tmp[0];

            if (!extends.empty()) {
                prepType.extends = extends;
                tmpPrepType.isMain = false;
                tmpPrepType.type = extends;
                file.prepTypes.push_back(tmpPrepType);
            }

            if (!implements.empty()) {
                this->stringHelper->split(prepType.implements, ",", implements);
                for (string v: prepType.implements) {
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
            for (PreparedType t: file.prepTypesMain) {
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
        file.content = getReader()->readTextFile(item->fullPath); //make it char*
        file.mainTypes = new set<string>(); //@todo delete it

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

        stringHelper->split(tmp, DELIMETER, className);

        for (string &part: tmp) {
            if (i + 1 < tmp.size()) {
                out += part;
                if (i + 2 < tmp.size()) {
                    out += NAMESPACE_SEPARATOR;
                }
            }
            i++;
        }
    }

    void ZendParser::generateNamespace(const string &className, string &out) {
        out.clear();
        vector<string> tmp;
        unsigned int i = 0;

        this->stringHelper->split(tmp, DELIMETER, className);

        for (string &part: tmp) {
            out += part;
            if (i + 1 < tmp.size()) {
                out += NAMESPACE_SEPARATOR;
            }
            i++;
        }
    }
    void ZendParser::generateNamespace(const string &className, string &out, int index) {
        out.clear();
        vector<string> tmp;
        unsigned int i = 0;

        this->stringHelper->split(tmp, DELIMETER, className);

        for (string &part: tmp) {
            out += part;
            if (i + 1 < tmp.size()) {
                out += NAMESPACE_SEPARATOR;
                if(i==index) return;
            }
            i++;
        }
    }

    void ZendParser::sortFaster(vector<PreparedType> &out) { //@todo const here (vector)
        set<PreparedType> foos(out.begin(), out.end());
        out.clear();
        std::set<PreparedType>::iterator it;
        for (it = foos.begin(); it != foos.end(); ++it) { //@todo const here (vector)
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
        string typeCopyLower;
        vector<string> duplicates;
        vector<string>::iterator it;
        for (PreparedType &type: types) {
            boost::trim(type.type);
            if (type.type.empty())
                continue;
            typeCopyLower = type.type;
            toLower(typeCopyLower);
            type.typeLower = typeCopyLower;
            found = typeCopyLower.find(DELIMETER);
            it = find(duplicates.begin(), duplicates.end(), typeCopyLower);
            if ((it != duplicates.end()
                 || isKeyword(typeCopyLower)
                 || isBuiltInType(type)
                 || found == string::npos)) {
                continue;
            }

            out.push_back(type);
            duplicates.push_back(typeCopyLower);
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

        stringHelper->split(tmp, DELIMETER, type);

        return generateAlias(tmp, parts);
    }

    void ZendParser::generatePreparedTypes(File &file, vector<string> &tmpVector) {
        tmpVector.clear();

        set<PreparedType> duplicatesSet; //@todo must know full namespace too, not only class name
        stringstream basicStringstream;
        string className, classNameLower, classNameCompared, classNameComparedLower;
//	PreparedType preparedType;
        size_t size;

        for (PreparedType &type: file.prepTypes) { //make this stream iterator
            className = generateAlias(type.type, 1, tmpVector);
            classNameLower = className;
            toLower(classNameLower);
            int count = 0;
            for (PreparedType typeCompared: file.prepTypes) {
                classNameCompared = generateAlias(typeCompared.type, 1, tmpVector);
                classNameComparedLower = classNameCompared;
                transform(classNameComparedLower.begin(), classNameComparedLower.end(),
                          classNameComparedLower.begin(), ::tolower);
                if (classNameLower.compare(classNameComparedLower) == 0) {
                    count++;
                }
                if (type.type.compare(typeCompared.type) == 0) { //is self type , so continue
                    continue;
                }
            }
            if (count > 1
                || isBuiltInType(type.type)) {
//                cout << "\nduplicate: " << type.type;
                duplicatesSet.insert(type);
            }
        }

        for (PreparedType &preparedType: file.prepTypes) {

            stringHelper->split(tmpVector, DELIMETER, preparedType.type);

            className = tmpVector[tmpVector.size() - 1]; //@todo use full type to commpare

            basicStringstream.str(string());
            basicStringstream.clear();
            copy(tmpVector.begin(), tmpVector.end(),
                 std::ostream_iterator<string>(basicStringstream, DELIMETER));
            preparedType.type = basicStringstream.str().substr(0, basicStringstream.str().length() - 1);

//            debug(file, preparedType, classNameLower, preparedType.type + ": preparedType.type = basicStringstream.str().substr(0, basicStringstream.str().length() - 1)");

            classNameComparedLower = className;
            toLower(classNameComparedLower);

            if (!hasMainType(file)) {
//                if (file.name == "Abstract.php") cout << "Abstract.php Procedural\n";
//                cout << file.name + " Procedural\n";
                processFileProcedural(file, classNameCompared, classNameComparedLower, preparedType, tmpVector,
                                      basicStringstream);
            } else {
//                if (file.name == "Abstract.php") cout << "Abstract.php OOP\n";
                processFileObjectOriented(file, duplicatesSet, className, classNameComparedLower, size, preparedType,
                                          tmpVector, basicStringstream);
            }
        }

    }

    void
    ZendParser::processFileObjectOriented(const BaseParser::File &file, set<PreparedType> &duplicatesSet,
                                          const string &className,
                                          const string &classNameLower, size_t size,
                                          BaseParser::PreparedType &preparedType, vector<string> &namespaceVector,
                                          stringstream &stream) {
//@todo it must be a check, if class is not a collision with others globablly from different namespaces
        if (isKeyword(classNameLower)) {
            debug(file, preparedType, classNameLower, "isKeyword(classNameLower)");
            size = 2; //namespace + className
            preparedType.alias = generateAlias(namespaceVector, size);
            generateUsage(size, preparedType, namespaceVector, stream);
        } else if (isBuiltInType(className) && !hasDelimeter(preparedType)) { //is php reserverd type
            debug(file, preparedType, className, "isBuiltInType(preparedType)");
            preparedType.alias = "\\\\" + preparedType.type;
            preparedType.usage = "";
        } else if (isMainType(file, preparedType)) {
            debug(file, preparedType, className, "isMainType(file, preparedType)");
            size = 1;
            if (isBuiltInType(className)) { //isSameTypeFromDifferentNamespace
                size = 2;
            }
            preparedType.alias = generateAlias(namespaceVector, size);
            preparedType.usage = "";
            debug(file, preparedType, className, "alias: " + preparedType.alias);

        } else if (isDuplicate(duplicatesSet, preparedType)) { //many
            debug(file, preparedType, className, "isDuplicate(duplicatesSet, preparedType)");
//            size = namespaceVector.size(); //use full name alias, do not shorten
            size = 3;
            preparedType.alias = generateAlias(namespaceVector,
                                               size); //alias might be longer than class name, when same classes from different namespace
            generateNamespace(preparedType.type, preparedType.usage , 1); //here type is safe, non-colliding - use 3size alias and 1size class name
            size = 1;
            if (isBuiltInType(className)) { //isSameTypeFromDifferentNamespace
                size = 2;
            }
            generateUsage(size, preparedType, namespaceVector, stream);

        } else {
            size = 1;
            if (isBuiltInType(className)) { //isSameTypeFromDifferentNamespace
                size = 2;
            }
            debug(file, preparedType, className, "else (no case)");
            preparedType.alias = generateAlias(namespaceVector, size);
//            generateNamespace(preparedType.type, preparedType.usage);
            generateUsage(size, preparedType, namespaceVector, stream);

        }
    }

    void ZendParser::generateUsage(size_t size, BaseParser::PreparedType &preparedType, vector<string> &namespaceVector,
                                   stringstream &stream) {
        stream.clear();
        stream.str(string()); //make it double secured
        copy(namespaceVector.begin(), namespaceVector.end() - 1,
             ostream_iterator<string>(stream, DELIMETER)); //@todo extract it to PreparedType
        generateNamespace(stream.str().substr(0, stream.str().length() - 1) + DELIMETER
                          + generateAlias(namespaceVector, size), preparedType.usage);
    }

    bool ZendParser::isRestricted(const string &className, const string &classNameLower) { //@todo has no sense
        return isKeyword(classNameLower) //cant be keyword here , keywords must not be touched
               || isBuiltInType(className); //@todo isDuplicate too
    }

    void ZendParser::debug(const BaseParser::File &file, const PreparedType &typeCopy, const string &classNameLower,
                           string msg) const {
        if (file.fullPath.find("Zend/Paginator/Exception") != std::string::npos)
            cout << file.fullPath + file.name + ", type full: " + typeCopy.type + ", classname: " + classNameLower +
                    ", " + msg + "\n";
    }

    bool ZendParser::isDuplicate(const set<PreparedType> &duplicateSet,
                                 const PreparedType &preparedType) const {
        return duplicateSet.find(preparedType) != duplicateSet.end();
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
                 ostream_iterator<string>(stream, DELIMETER));

            generateNamespace(
                    stream.str().substr(0, stream.str().length() - 1) + DELIMETER
                    + alias, tmpString);

            preparedType.alias = NAMESPACE_SEPARATOR + tmpString;
//				    cout <<"isKeyword alias: "<<preparedType.alias<<"\n";
        } else {
//                    cout <<"standard type alias: \n";
            generateNamespace(preparedType.type, preparedType.alias);
            preparedType.alias = NAMESPACE_SEPARATOR + preparedType.alias;
//				    cout <<preparedType.alias<<"\n";
        }
    }

    /**
     * is type main type of file (like class or interface)
     * @param file
     * @param preparedType
     * @return
     */
    bool ZendParser::isMainType(const BaseParser::File &file,
                                const BaseParser::PreparedType &preparedType) const {
        return preparedType.type.compare(file.mainType) == 0;
    }

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
        return isBuiltInType(preparedType.type);
    }

    /**
     * Does type is built in php type
     * @param typeName
     * @return
     */
    bool ZendParser::isBuiltInType(const string &typeName) {
        return builtInTypes->find(typeName)
               != builtInTypes->end();
    }

    void ZendParser::generatePreparedTypeFull(PreparedType &outPrep,
                                              vector<string> &tmpVect) {
        if (hasDelimeter(outPrep)) {
            outPrep.alias = NAMESPACE_SEPARATOR + outPrep.type; //extract const - ns sep.
            return;
        }

        stringstream stream;
        string tmpString;
        size_t size = 1;

        this->stringHelper->split(tmpVect, DELIMETER, outPrep.type);

        tmpString = tmpVect[tmpVect.size() - 1];
        toLower(tmpString);

        if (isKeyword(tmpString)) {
            size = 2;
        }

        extractNamespace(outPrep.type, tmpString, tmpVect);
        tmpString += DELIMETER + generateAlias(outPrep.type, size, tmpVect);
        generateNamespace(tmpString, outPrep.alias);

        outPrep.alias = NAMESPACE_SEPARATOR + outPrep.alias;
    }

    bool
    ZendParser::hasDelimeter(const BaseParser::PreparedType &outPrep) const {
        return outPrep.type.find(DELIMETER) != string::npos;
    }

    void ZendParser::toLower(string &tmp) const { transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower); }

    void ZendParser::generatePreparedTypesGlobal(vector<string> &tmp) {
        vector<PreparedType>::iterator type = typesRegistry->begin();
        for (; type != typesRegistry->end(); ++type) {
            generatePreparedTypeFull(*type, tmp);
        }
    }

    void ZendParser::readBuiltInTypes() {
        vector<string> v;
        string fileName = "builtintypes.txt";
        string txt = getReader()->readTextFile(fileName);
        stringHelper->split(v, "\n", txt);
        this->builtInTypes = new set<std::string>(v.begin(), v.end()); //lowercase it
        cout << "builtInTypes size " << builtInTypes->size() << "\n";
    }

    void ZendParser::readKeywords() {
        vector<string> v;
        string fileName = "keywords.txt";
        string txt = getReader()->readTextFile(fileName);
        stringHelper->split(v, "\n", txt);
        this->keywords = new set<std::string>(v.begin(), v.end()); //lowercase it
        cout << "keywords size " << keywords->size() << "\n";
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

    void ZendParser::fixAlias(const File &file, const PreparedType &preparedType) const {

    }
} /* namespace Salamon */

