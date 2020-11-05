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

#include "Parser.h"
#include "../utils/DirectoryReader.h"
#include "../utils/Regexer.h"
#include "../utils/Strings.h"

///////////////////////////////////////////////////////////////////////////////
//  Include Wave itself
#include <boost/wave.hpp>

///////////////////////////////////////////////////////////////////////////////
// Include the lexer stuff
#include <boost/wave/cpplexer/cpp_lex_token.hpp>    // token class
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp> // lexer class

namespace phpconvert {

    class ZendParser : public Parser {
    public:
        ZendParser();

        virtual ~ZendParser();

        virtual void parse();

        DirectoryReader *getReader();

        Regexer *getRegexer();

        parser::File buildFile(DirectoryReader::Item *item,
                       vector<pair<string, string> > &out, vector<string> &tmpOut,
                       vector<string> &tmp);

    protected:
        void setupReader();

        void readBuiltInTypes();

        void readKeywords();

        void extractTypes(const string &line, vector<pair<string, string> > &out,
                          vector<string> &tmp);

        void extractMainType(parser::File &file, vector<string> &out, vector<string> &tmp);

        void filterPreparedTypes(vector<parser::PreparedType> &types,
                                 vector<parser::PreparedType> &out);

        void generatePairs(vector<pair<string, string> > &out, vector<string> &src);

        void generatePreparedTypeFull(parser::PreparedType &outPrep,
                                      vector<string> &tmpVect);

        void generatePreparedTypes(parser::File &file, vector<string> &tmp);

        void generatePreparedTypesGlobal(vector<string> &tmp);

        void extractNamespace(const string &className, string &out,
                              vector<string> &tmp);

        void generateNamespace(const string &className, string &out);

        void addNamespace(parser::File &file);

        void addUsages(parser::File &file, set<string> tmpSet);

        void replaceTypes(parser::File &file);

        void replaceTypesMain(parser::File &file);

        void replaceTypesGlobal(parser::File &file);

        void replaceTypesBuiltIn(parser::File &file);

        void prepareTypesMain(parser::File &file, vector<string> &out, vector<string> &tmp);

        void prepareTypes(parser::File &file, vector<pair<string, string> > &out,
                          vector<string> &tmp);

        string generateAlias(vector<string> &type, unsigned int parts);

        string generateAlias(string &type, unsigned int parts, vector<string> &tmp);

        void writeTypesRegistryFile();

        void buildFiles(parser::File file, vector<string> tmpOut, int &processed,
                        vector<DirectoryReader::Item> *readerResult,
                        vector<pair<string, string> > &tmpOutPairs,
                        vector<string> &tmpVector);

        void sortFaster(vector<parser::PreparedType> &out);

        void sortSlower(vector<parser::PreparedType> &out);

        void writeFiles(const set<string> &tmpSet, int &generated);

        void replaceType(parser::PreparedType &type, parser::File &file);

        string stripCmments(string const &input);

    private:
        DirectoryReader *reader;
        Regexer *regexer;
        Strings *strings;
        set<string> *builtInTypes;
        set<string> *keywords;
        vector<parser::PreparedType> *typesRegistry;
        vector<parser::PreparedType> *typesRegistryUnfiltered;
        vector<parser::File> *results;
    };

} /* namespace phpconvert */

#endif /* ZENDPARSER_H_ */
