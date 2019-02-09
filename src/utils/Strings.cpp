/*
 * Strings.cpp
 *
 *  Created on: Sep 18, 2014
 *      Author: andy
 */

#include "Strings.h"

namespace phpconvert {

Strings::Strings() {
	// TODO Auto-generated constructor stub

}

Strings::~Strings() {
	// TODO Auto-generated destructor stub
}

void Strings::split(vector<string>& out, const string& delim, const string& source) {
	out.clear();
	if (source.find(delim) != string::npos)
		boost::split(out, source, is_any_of(delim), token_compress_on);
	else
		out.push_back(source);
}
string Strings::join(vector<string>& in, string& delim) {
	return boost::algorithm::join(in, delim);
}

void Strings::replace(string& target, string& from, string& to) {
	boost::replace_all(target, from, to);
}

void Strings::regexReplace(string& target, string& from, string& to) {
	boost::regex re(from);
	target = boost::regex_replace(target, re, to,
			boost::match_default | boost::format_all);
}

} /* namespace phpconvert */
