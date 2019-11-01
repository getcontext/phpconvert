/*
 * SystemException.cpp
 *
 *  Created on: 18 May 2014
 *      Author: Andrzej phpconvert <andrzej.salamon@gmail.com>
 */

#include "SystemException.h"
#include <sstream>

using namespace std;


namespace phpconvert {

    SystemException::SystemException(SystemException::ExceptionType t) throw() {
        code = t;

        stringstream msg;

        switch (code) {
            case INVALID_PARAMETERS:
                msg << "Invalid parameters\n";
                msg << "Usage: zf2ns(.exe) <option(s)>\n";
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

        message = msg.str();
    }

    SystemException::~SystemException() throw() {
    }

    const char *SystemException::what() const throw() {
        return message.c_str();
    }

    SystemException::ExceptionType SystemException::getCode() {
        return code;
    }

    void SystemException::setMessage(string &message) {
        this->message = message;
    }
} /* namespace phpconvert */

