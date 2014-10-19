/*
 * Strings.h
 *
 *  Created on: Sep 18, 2014
 *      Author: andy
 */

#ifndef STRINGS_H_
#define STRINGS_H_

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>


using namespace std;
using namespace boost;


namespace Salamon {

class Strings {
public:
	Strings();
	virtual ~Strings();
	vector<string> split(const string delim, const string target);
	string replace(const string target,const string from, const string to);
	string regexReplace(const string target,const string from, const string to);
};

} /* namespace Salamon */

#endif /* STRINGS_H_ */
