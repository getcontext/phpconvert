/*
 * PhpParser.h
 *
 * parses and extracs zend1 shit
 *
 *  Created on: 3 May 2014
 *      Author: Andrzej phpconvert <andrzej.salamon@gmail.com>
 */

#ifndef ZENDPARSER_H_
#define ZENDPARSER_H_

#include "../BaseParser.h"
#include "../utils/DirectoryReader.h"
#include "../utils/RegexHelper.h"
#include "../utils/StringHelper.h"

///////////////////////////////////////////////////////////////////////////////
//  Include Wave itself
#include <boost/wave.hpp>

///////////////////////////////////////////////////////////////////////////////
// Include the lexer stuff
#include <boost/wave/cpplexer/cpp_lex_token.hpp>    // token class
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp> // lexer class

namespace phpconvert {

    class ZendParser : public BaseParser {
    public:
        ZendParser();

        virtual ~ZendParser();

        virtual void parse();

        static const string RGX_INSTANCEOF;

        static const string RGX_NEW;
        static const string RGX_SIGNATURE;
        static const string RGX_STATIC_CALL;
        static const string RGX_MAIN_TYPE;
        static const string RGX_THROW_NEW;
        static const char *RGX_BUILTIN_TYPE;
        static const string RGX_EXCLUDE_COMMENT;

        File buildFile(DirectoryReader::Item *item,
                       vector<pair<string, string> > &out, vector<string> &tmpOut,
                       vector<string> &tmp);

    protected:

        DirectoryReader *getReader();

        RegexHelper *getRegexer();

        void setupReader();

        void readBuiltInTypes();

        void readKeywords();

        void extractTypes(const string &line, vector<pair<string, string> > &out,
                          vector<string> &tmp);

        void extractMainType(File &file, vector<string> &out, vector<string> &tmp);

        void filterPreparedTypes(vector<PreparedType> &types,
                                 vector<PreparedType> &out);

        void generatePairs(vector<pair<string, string> > &out, vector<string> &src);

        void generatePreparedTypeFull(PreparedType &outPrep,
                                      vector<string> &tmpVect);

        void generatePreparedTypes(File &file, vector<string> &tmpVector);

        void generatePreparedTypesGlobal(vector<string> &tmp);

        void extractNamespace(const string &className, string &out,
                              vector<string> &tmp);

        void generateNamespace(const string &className, string &out);

        void generateNamespace(const string &className, string &out, int index);

        void addNamespace(File &file);

        void addUsages(File &file, set<string> tmpSet);

        void replaceTypes(File &file);

        void replaceTypesMain(File &file);

        void replaceTypesGlobal(File &file);

        void replaceTypesBuiltIn(File &file);

        void prepareTypesMain(File &file, vector<string> &out, vector<string> &tmp);

        void prepareTypes(File &file, vector<pair<string, string> > &out,
                          vector<string> &tmp);

        string generateAlias(vector<string> &type, unsigned int parts);

        string generateAlias(string &type, unsigned int parts, vector<string> &tmp);

        void writeTypesRegistryFile();

        void buildFiles(File file, vector<string> tmpOut, int &processed,
                        vector<DirectoryReader::Item> *readerResult,
                        vector<pair<string, string> > &tmpOutPairs,
                        vector<string> &tmpVector);

        void sortFaster(vector<PreparedType> &out);

        void sortSlower(vector<PreparedType> &out);

        void write(const set<string> &tmpSet, int &generated);

        void replaceType(PreparedType &type, File &file);

        string stripCmments(string const &input);

        bool isInBraces(string const &input); //is replace in braces ?

    private:
        DirectoryReader *reader;
        RegexHelper *regexer;
        StringHelper *stringHelper;

        set<string> *builtInTypes;
        set<string> *keywords;

        vector<PreparedType> *typesRegistry;
        vector<PreparedType> *typesRegistryUnfiltered;

        vector<File> *results;

        bool isBuiltInType(const PreparedType &preparedType);

        bool isInMainTypes(const File &file, const PreparedType &preparedType) const;

        bool hasMainType(const File &file) const;

        bool isKeyword(const string &tmpClassNameLower);

        bool isMainType(const File &file, const PreparedType &preparedType) const;

        void
        processFileProcedural(const File &file, string &tmpString, const string &tmpClassNameLower,
                              PreparedType &preparedType,
                              vector<string> &tmpVector, stringstream &stream);

        void processFileObjectOriented(const File &file, set<PreparedType> &duplicatesSet, const string &className,
                                       const string &classNameLower, size_t size, PreparedType &preparedType,
                                       vector<string> &namespaceVector, stringstream &stream);

        bool isDuplicate(const set<PreparedType> &duplicateSet, const PreparedType &preparedType) const;

        void toLower(string &tmp) const;

        bool isBuiltInType(const string &typeName);

        bool isFound(const File &file, const PreparedType &typeCopy) const;

        void debug(const File &file, const PreparedType &typeCopy, const string &classNameLower, string msg) const;

        void processRestricted(const File &file, const string &classNameLower, size_t size, PreparedType &preparedType,
                               vector<string> &namespaceVector, stringstream &stream);

        bool isRestricted(const string &className, const string &classNameLower);

        bool hasDelimeter(const PreparedType &outPrep) const;

        void fixAlias(const File &file,     const PreparedType &preparedType) const;

        void
        generateUsage(size_t size, PreparedType &preparedType, vector<string> &namespaceVector, stringstream &stream);

        void findRequireOrInclude(); //@todo add more stuff here, regex in file content
        static const char *const PHP_EXT;
        static const char *const TYPES_REGISTRY_FILE_NAME;
        static const char *const PHP_TAG_OPEN;
        static const char *const PHP_NAMESPACE_TAG_REPLACE;
        static const char *const NAMESPACE_SEPARATOR;
        static const char *const DELIMETER;
    };

} /* namespace phpconvert */

#endif /* ZENDPARSER_H_ */
