/*
 * SystemException.cpp
 *
 *  Created on: 18 May 2014
 *      Author: Andrzej phpconvert <andrzej.salamon@gmail.com>
 */

#include "SystemException.h"
#include <sstream>
#include <iostream>
#include <cstring>

using namespace std;


namespace phpconvert {

    SystemException::SystemException(SystemException::ExType t) {
        code = t;
    }

    SystemException::~SystemException() noexcept {
    }

    const char *SystemException::what() const noexcept {
        stringstream msg;

        switch (code) {
            case INVALID_PARAMETERS:
                msg << "Invalid parameters\n";
                msg << "Usage: phpconvert(.exe) <option(s)>\n";
                msg << "Options:\n";
                msg << "\t-d,\tSpecify the source directory\n";
                msg << "\t-o,\tSpecify the output directory\n";
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
         const char *out = new char[strlen(str.c_str())+1];
//        out = str.c_str(); //@todo seems to be pointer as well, make it field
        std::strcpy(const_cast<char *>(out), str.c_str());
        return out;
    }

    SystemException::ExType SystemException::getCode() {
        return code;
    }
} /* namespace phpconvert */

