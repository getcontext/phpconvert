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
	static const char* RGX_BUILTIN_TYPE;

	static const string RGX_EXCLUDE_COMMENT;

	File buildFile(DirectoryReader::Item* item,
			vector<pair<string, string> >& out, vector<string>& tmpOut,
			vector<string>& tmp);

protected:
	void setupReader();
	void readBuiltInTypes();
	void readKeywords();

	void extractTypes(const string& line, vector<pair<string, string> >& out,
			vector<string>& tmp);
	void extractMainType(File& file, vector<string>& out, vector<string>& tmp);

	void filterPreparedTypes(vector<PreparedType>& types,
			vector<PreparedType>& out);

	void generatePairs(vector<pair<string, string> >& out, vector<string>& src);

	void generatePreparedTypeFull(PreparedType& outPrep,
			vector<string>& tmpVect);

	void generatePreparedTypes(File& file, vector<string>& tmp);

	void generatePreparedTypesGlobal(vector<string>& tmp);

	void extractNamespace(const string& className, string& out,
			vector<string>& tmp);
	void generateNamespace(const string& className, string& out);
	void addNamespace(File& file);

	void addUsages(File& file, set<string> tmpSet);

	void replaceTypes(File& file);
	void replaceTypesMain(File& file);
	void replaceTypesGlobal(File& file);
	void replaceTypesBuiltIn(File& file);

	void prepareTypesMain(File& file, vector<string>& out, vector<string>& tmp);
	void prepareTypes(File& file, vector<pair<string, string> >& out,
			vector<string>& tmp);

	string generateAlias(vector<string>& type, unsigned int parts);
	string generateAlias(string& type, unsigned int parts, vector<string>& tmp);

	void writeTypesRegistryFile();
	void buildFiles(File file, vector<string> tmpOut, int& processed,
			vector<DirectoryReader::Item>* readerResult,
			vector<pair<string, string> >& tmpOutPairs,
			vector<string>& tmpVector);
	void sortFaster(vector<PreparedType>& out);
	void sortSlower(vector<PreparedType>& out);
	void writeFiles(const set<string>& tmpSet, int& generated);
	void replaceType(PreparedType& type, File& file);
	string stripCmments(string const& input);

private:
	DirectoryReader* reader;
	Regexer* regexer;
	Strings* strings;

	set<string>* builtInTypes;
	set<string>* keywords;

	vector<PreparedType>* typesRegistry;
	vector<PreparedType>* typesRegistryUnfiltered;

	vector<File>* results;
};

} /* namespace phpconvert */

#endif /* ZENDPARSER_H_ */
