/*
 * AppParams.cpp
 *
 *  Created on: 17 May 2014
 *      Author: Andrzej Salamon <andrzej.salamon@gmail.com>
 */

#include "AppParams.h"

namespace phpconvert {

    AppParams *AppParams::createFromRequest(int argc, char *argv[]) {
        string current;
        AppParams *obj = new AppParams(); //I hate 'auto'

        obj->setValid(false);
        obj->setRecursive(false);

        for (int i = 1; i < argc; i += 1) {
            current = argv[i];
            if (current.compare("-r") == 0) {
                obj->setRecursive(true);
            } else if (current.compare("-d") == 0) {
                obj->setPath(argv[i + 1]); //will boom according to autogeneration :D
                obj->setValid(true);
                i++;
            } else if (current.compare("-o") == 0) {
                obj->setOutputPath(argv[i + 1]); //same here buuuum ! :D
                i++;
            }
        }

        return obj;
    }

    void AppParams::setRecursive(const bool &recursive) {
        this->recursive = recursive;
    }

    void AppParams::setValid(const bool &valid) {
        this->valid = valid;
    }

    const bool &AppParams::isValid() {
        return valid;
    }

    const string &AppParams::getPath() const {
        return path;
    }

    const bool &AppParams::isRecursive() const {
        return recursive;
    }

    const string &AppParams::getOutputPath() const {
        return outputPath;
    }

    //yeah autogeneration partially correct...  will blow when explictly passed. must be copyy :D
    void AppParams::setPath(const string &path) {
        this->path = path;
    }

    void AppParams::setOutputPath(const string &outputPath) {
        this->outputPath = outputPath;
    }

} /* namespace phpconvert */
