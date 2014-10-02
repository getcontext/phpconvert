/*
 * DirectoryReader.h
 *
 *  Created on: 24 May 2014
 *      Author: Andrzej Salamon <andrzej.salamon@gmail.com>
 */

#ifndef DIRECTORYREADER_H_
#define DIRECTORYREADER_H_

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../SystemException.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

namespace Salamon {

class DirectoryReader {
public:
	enum OS {
		Win,
		Nix
	};
	struct Item {
		bool isFile;
		string name;
		string fullPath;

	};
	DirectoryReader();
	DirectoryReader(const char* dir);
	virtual ~DirectoryReader();
	void read();
	void setPath(const char* p);
	vector<Item> *getResults();
	DirectoryReader::OS getOS();
	string readTextFile(const string path);
	/*
	 * windows based systems directory separator
	 */
	static const string WIN_DS;
	/*
	 * unix/linux based systems directory separator
	 */
	static const string NIX_DS;
protected:
	const char* path;
private:
	vector<Item> *result;

};

} /* namespace Salamon */

#endif /* DIRECTORYREADER_H_ */
