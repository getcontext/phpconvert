/*
 * StringHelper.cpp
 *
 *  Created on: Sep 18, 2014
 *      Author: andy
 */

#include "StringHelper.h"

namespace phpconvert {

StringHelper::StringHelper() {
	// TODO Auto-generated constructor stub

}

StringHelper::~StringHelper() {
	// TODO Auto-generated destructor stub
}

void StringHelper::split(vector<string>& out, const string& delim, const string& source) {
	out.clear();
	if (source.find(delim) != string::npos)
		boost::split(out, source, is_any_of(delim), token_compress_on);
	else
		out.push_back(source);
}
string StringHelper::join(vector<string>& in, string& delim) {
	return boost::algorithm::join(in, delim);
}

void StringHelper::replace(string& target, string& from, string& to) {
	boost::replace_all(target, from, to);
}

void StringHelper::regexReplace(string& target, string& from, string& to) {
	boost::regex re(from);
	target = boost::regex_replace(target, re, to,
			boost::match_default | boost::format_all);
}

} /* namespace phpconvert */
