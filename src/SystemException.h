/*
 * SystemException.h
 *
 *  Created on: 18 May 2014
 *      Author: Andrzej phpconvert <andrzej.salamon@gmail.com>
 */

#ifndef SYSTEMEXCEPTION_H_
#define SYSTEMEXCEPTION_H_

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
    private:
        ExceptionType code;
    public:
        SystemException(ExceptionType t);

        virtual ~SystemException() throw();

        const char *what() const throw();

        ExceptionType getCode();
    };

} /* namespace phpconvert */

#endif /* SYSTEMEXCEPTION_H_ */
