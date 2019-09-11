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
        AppParams *createFromRequest(int argc, char *argv[]);

        const bool &isValid();

        const string &getPath() const;

        const bool &isRecursive() const;

        void setPath(const string &path);

        void setOutputPath(const string &outputPath);

        const string &getOutputPath() const;

    private:
        //@todo add private constructor, avoid copy constructor
        string path;
        bool recursive;
        string outputPath;
        bool valid;

        void setRecursive(const bool recursive);

        void setValid(const bool valid);
    };

} /* namespace phpconvert */

#endif /* APPPARAMS_H_ */
