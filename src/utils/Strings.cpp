/*
 * Strings.cpp
 *
 *  Created on: Sep 18, 2014
 *      Author: andy
 */

#include "Strings.h"

namespace Salamon {

Strings::Strings() {
	// TODO Auto-generated constructor stub

}

Strings::~Strings() {
	// TODO Auto-generated destructor stub
}

vector<string> Strings::split(const string delim, const string source) {
	vector<string> out; // #2: Search for tokens
	boost::split(out, source, is_any_of(delim), token_compress_on);
	return out;
}

string Strings::replace(const string target, const string from,
		const string to) {
	string out = target;
	boost::replace_all(out, from, to);
	return out;
}

} /* namespace Salamon */
