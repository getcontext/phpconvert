/*
 * Strings.h
 *
 *  Created on: Sep 18, 2014
 *      Author: andy
 */

#ifndef STRINGS_H_
#define STRINGS_H_

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/regex.hpp>

using namespace std;
using namespace boost;

namespace phpconvert {

class Strings {
public:
	Strings();
	virtual ~Strings();
	void split(vector<string>& out, const string& delim, const string& target);
	string join(vector<string>& in, string& delim);
	void replace(string& target, string& from, string& to);
	void regexReplace(string& target, string& from, string& to);
	void regexReplace(string* target, string& from, string& to);
};

} /* namespace phpconvert */

#endif /* STRINGS_H_ */
