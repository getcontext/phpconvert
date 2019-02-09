/*
 * Regexer.h
 *
 *  Created on: 29 May 2014
 *      Author: winmaster
 */

#ifndef REGEXER_H_
#define REGEXER_H_

#include <iostream>
#include <string>
#include <boost/regex.hpp>

using namespace std;

namespace phpconvert {

class Regexer {
public:
	Regexer();
	virtual ~Regexer();
	string find(const string& target,const string pattern, int index);
	vector<string> findAll(const string& target,const string pattern, int index);
	void findAll(vector<string>& out, const string& target,const string pattern, int index);
	void test(string target, string pattern);
};

} /* namespace phpconvert */

#endif /* REGEXER_H_ */
