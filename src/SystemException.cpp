/*
 * SystemException.cpp
 *
 *  Created on: 18 May 2014
 *      Author: Andrzej Salamon <andrzej.salamon@gmail.com>
 */

#include "SystemException.h"
#include <sstream>

using namespace std;


namespace Salamon {

SystemException::SystemException(SystemException::ExType t) {
	code = t;
}

SystemException::~SystemException() throw () {
}

const char* SystemException::what() const throw () {
	stringstream msg;

	switch (code) {
	case INVALID_PARAMETERS:
		msg << "Invalid parameters\n";
		msg << "Usage: zf2ns(.exe) <option(s)>\n";
		msg << "Options:\n";
		msg << "\t-d,\tSpecify the source directory\n";
		msg << "\t-r,\tProcess directories recursive";
		break;
	case CANT_OPEN_DIRECTORY:
		msg << "Cant't open directory or directory not found.\n";
		break;
	case FATAL:
		msg << "Application has crashed !";
		break;
	default:
		msg << "Unknonw error";
		break;
	}
	string str = msg.str();
	const char* out = str.c_str();
	return out;
}

SystemException::ExType SystemException::getCode() {
	return code;
}
} /* namespace Salamon */

