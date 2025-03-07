/*
 * AppParams.cpp
 *
 *  Created on: 17 May 2014
 *      Author: Andrzej phpconvert <andrzej.salamon@gmail.com>
 */

#include "AppParams.h"

namespace phpconvert {

    AppParams *AppParams::createFromRequest(int argc, char *argv[]) {
        string current;
        AppParams *obj = new AppParams();

        obj->setValid(false);
        obj->setRecursive(false);

        for (int i = 1; i < argc; i += 1) {
            current = argv[i];
            if (current == "-r") {
                obj->recursive = true;
            } else if (current == "-d") {
                obj->path = argv[i + 1];
                obj->setValid(true);
                i++;
            } else if (current == "-o") {
                obj->outputPath = argv[i + 1];
                i++;
            }
        }

        return obj;
    }

    void AppParams::setRecursive(const bool recursive) {
        this->recursive = recursive;
    }

    bool AppParams::isValid() const {
        return valid;
    }

    void AppParams::setValid(const bool valid) {
        this->valid = valid;
    }

} /* namespace phpconvert */
