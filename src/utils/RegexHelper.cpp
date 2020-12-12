/*
 * RegexHelper.cpp
 *
 *  Created on: 29 May 2014
 *      Author: winmaster
 */

#include "RegexHelper.h"

#include <iostream>
#include <string>

using namespace std;

namespace phpconvert {

RegexHelper::RegexHelper() {
	// TODO Auto-generated constructor stub

}

RegexHelper::~RegexHelper() {
	// TODO Auto-generated destructor stub
}

string RegexHelper::find(const string& target, const string pattern, int index) {
	string out;
	boost::regex expression(pattern);
	std::string::const_iterator start, end;
	start = target.begin();
	end = target.end();
	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;
	if (boost::regex_search(start, end, what, expression, flags)) {
		out = what[index];
	}

	return out;

}
void RegexHelper::findAll(vector<string>& out, const string& target,
                          const string pattern, int index) {
	out.clear();
	boost::regex expression(pattern);
	std::string::const_iterator start, end;
	start = target.begin();
	end = target.end();
	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;
	string result;
	while (boost::regex_search(start, end, what, expression, flags)) {
		if (index < 0) {
			for (unsigned i = 0; i < what.size(); ++i) {
				result = what[i];
				out.push_back(result);
			}
		} else {
			result = what[index];
			out.push_back(result);
		}
		// update search position:
		start = what[0].second;
		// update flags:
		flags |= boost::match_prev_avail;
		flags |= boost::match_not_bob;
	}

//	for (string v : out) {
//		cout << v << "\n";
//	}

}

vector<string> RegexHelper::findAll(const string& target, const string pattern,
                                    int index) {
	vector<string> out;
	boost::regex expression(pattern);
	std::string::const_iterator start, end;
	start = target.begin();
	end = target.end();
	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;

	while (boost::regex_search(start, end, what, expression, flags)) {
		if (index < 0) {
			for (unsigned i = 0; i < what.size(); ++i) {
				out.push_back((string) what[i]);
			}
		} else {
			out.push_back((string) what[index]);
		}
		// update search position:
		start = what[0].second;
		// update flags:
		flags |= boost::match_prev_avail;
		flags |= boost::match_not_bob;
	}

	return out;
}

void RegexHelper::test(string target, string pattern) {
	boost::regex expression(pattern);
	std::string::const_iterator start, end;
	const string file = target;
	start = file.begin();
	end = file.end();
	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;
	while (boost::regex_search(start, end, what, expression, flags)) {
		for (unsigned i = 0; i < what.size(); ++i) {
			std::cout << "[" << what[i] << "] ";
		}
		// update search position:
		start = what[0].second;
		// update flags:
		flags |= boost::match_prev_avail;
		flags |= boost::match_not_bob;
	}
}

} /* namespace phpconvert */

