/*
 * SystemException.h
 *
 *  Created on: 18 May 2014
 *      Author: Andrzej phpconvert <andrzej.salamon@gmail.com>
 */

#ifndef SYSTEMEXCEPTION_H_
#define SYSTEMEXCEPTION_H_

#include <string>
#include <exception>

using namespace std;

namespace phpconvert {

    class SystemException : public exception {
    public:
        enum ExceptionType {
            INVALID_PARAMETERS,
            CANT_OPEN_DIRECTORY,
            OS_NOT_FOUND,
            FATAL
        };

        SystemException(ExceptionType t) throw();

        virtual ~SystemException();

        virtual const char *what() const throw();

        ExceptionType getCode();

        void setMessage(string &message);

    private:
        ExceptionType code;
        const char* charMessage;
        string message;
    };

} /* namespace phpconvert */

#endif /* SYSTEMEXCEPTION_H_ */
