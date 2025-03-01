/*
 * must be immutable !
 * AppParams.h
 *
 *  Created on: 17 May 2014
 *      Author: Andrzej phpconvert <andrzej.salamon@gmail.com>
 */

#ifndef APPPARAMS_H_
#define APPPARAMS_H_

#include <string>

using namespace std;

namespace phpconvert {

    class AppParams {
    public:
        string path;
        bool recursive;
        string outputPath;

        AppParams *createFromRequest(int argc, char *argv[]);

        bool isValid() const;

    private:
        bool valid;

        void setRecursive(const bool recursive);

        void setValid(const bool valid);
    };

} /* namespace phpconvert */

#endif /* APPPARAMS_H_ */
