/*
 * DirectoryReader.cpp
 *
 *  Created on: 24 May 2014
 *      Author: Andrzej phpconvert <andrzej.salamon@gmail.com>
 */
#include "DirectoryReader.h"
#include <iostream>

namespace phpconvert {

/*
 * windows based systems directory separator
 */
    const string DirectoryReader::WIN_DS = "\\";
/*
 * unix/linux based systems directory separator
 */
    const string DirectoryReader::NIX_DS = "/";

    void DirectoryReader::read() {
        DIR *dir;
        dirent *ent;
        string rootPath(path);

        if ((dir = opendir(path)) == NULL) {
            throw SystemException(SystemException::CANT_OPEN_DIRECTORY);
        }

        while ((ent = readdir(dir)) != NULL) {
            string name(ent->d_name);

            if (name == "." || name == "..")
                continue;

            OS os = getOS();
            string separator;

            if (os == DirectoryReader::Win) {
                separator = DirectoryReader::NIX_DS;
            } else if (os == DirectoryReader::Nix) {
                separator = DirectoryReader::NIX_DS;
            }

            string fullPath = getFullPath(rootPath, name, separator);

            struct stat st;
            stat(fullPath.c_str(), &st);

            Item item; //can I return reference of local and catch it to external pointer ? when ref to fn Item creation block
            item.isFile = S_ISDIR(st.st_mode) ? false : true;
            item.name = name;
            item.fullPath = fullPath;

            result->push_back(item);
        }
        closedir(dir);
    }

    string DirectoryReader::getFullPath(const string &rootPath, const string &name, const string &separator) const {
        string fullPath = rootPath;
        fullPath += separator;
        fullPath += name;
        return fullPath;
    }

    void DirectoryReader::read(const char *dirName, string baseDir) {
        DIR *dir;
        dirent *ent;

        if ((dir = opendir(dirName)) == NULL) {
            throw SystemException(SystemException::CANT_OPEN_DIRECTORY);
        }

        while ((ent = readdir(dir)) != NULL) {
            string fileName(ent->d_name);
            if (fileName.compare(".") == 0 || fileName.compare("..") == 0)
                continue;

            OS os = getOS();
            string separator;

            if (os == DirectoryReader::Win) {
                separator = DirectoryReader::NIX_DS;
            } else if (os == DirectoryReader::Nix) {
                separator = DirectoryReader::NIX_DS;
            }

            string fullPath = getFullPath(dirName, fileName, separator);

            struct stat st;
            stat(fullPath.c_str(), &st);

            Item item;
            item.isFile = S_ISDIR(st.st_mode) ? false : true;
            item.name = fileName;
            item.fullPath = fullPath;
            item.dir = baseDir;

            result->push_back(item);

            if (!item.isFile) {
                this->read(fullPath.c_str(), getFullPath(baseDir, fileName, separator));
            }
        }

        closedir(dir);
    }

    DirectoryReader::DirectoryReader() {
        result = new vector<DirectoryReader::Item>();
    }

    DirectoryReader::DirectoryReader(const char *dir) :
            path(dir) {
        result = new vector<DirectoryReader::Item>();
    }

    DirectoryReader::~DirectoryReader() {
    }

    vector<DirectoryReader::Item> *DirectoryReader::getResults() {
        return result;
    }

    void DirectoryReader::setPath(const char *p) {
        path = p;
    }

    const char *DirectoryReader::getPath() {
        return path;
    }

    DirectoryReader::OS DirectoryReader::getOS() {
#ifdef _WIN32
        const char *OS_NAME = "win";
#elif _WIN64
        const char *OS_NAME = "win";
#elif __unix
        const char *OS_NAME = "nix";
#elif __linux
        const char *OS_NAME= "nix";
#endif

        DirectoryReader::OS os;
        string operatingSystemName(OS_NAME);

        if (operatingSystemName == "win") {
            os = DirectoryReader::Win;
        } else if (operatingSystemName == "nix") {
            os = DirectoryReader::Nix;
        } else {
            throw new SystemException(SystemException::OS_NOT_FOUND);
        }

        return os;
    }

    bool DirectoryReader::writeTextFile(const string path, const string &content) {
        ofstream myfile;
        myfile.open(path.c_str());
        if (!myfile.is_open()) {
            throw std::invalid_argument("Can't write file :" + path);
        }
        myfile << content;
        myfile.close();
        return myfile.is_open();
    }

    string DirectoryReader::readTextFile(const string path) {
        string line;
        string out;
        ifstream myfile(path.c_str());
        if (myfile.is_open()) {
            while (getline(myfile, line)) {
                out += line + "\n";
            }
        }
        myfile.close();
        return out;
    }

    void DirectoryReader::createDir(const string path) {
        boost::filesystem::create_directories(path);
    }

    void DirectoryReader::removeDir(const string path) {
        boost::filesystem::remove_all(path);
    }

}

/* namespace phpconvert */
