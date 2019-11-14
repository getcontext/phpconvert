//
// Created by wizard on 14.11.2019.
//

#ifndef PHPCONVERT_PREPAREDTYPE_H
#define PHPCONVERT_PREPAREDTYPE_H

#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <exception>

using namespace std;

namespace phpconvert {
    class PreparedType {
    public:
        bool operator==(PreparedType &compared) const {
            return typeLower.compare(compared.typeLower) == 0;
        }

        bool operator<(PreparedType const &compared) const {
            return typeLower.size() > compared.typeLower.size();
        }

        const string &getId() const;

        void setId(const string &id);

        const string &getType() const;

        void setType(const string &type);

        const string &getTypeLower() const;

        void setTypeLower(const string &typeLower);

        const string &getUsage() const;

        void setUsage(const string &usage);

        const string &getAlias() const;

        void setAlias(const string &alias);

        const string &getRaw() const;

        void setRaw(const string &raw);

        const string &getReplace() const;

        void setReplace(const string &replace);

        bool isMain1() const;

        void setIsMain(bool isMain);

        const string &getExtends() const;

        void setExtends(const string &extends);

        const vector<string> &getImplements() const;

        void setImplements(const vector<string> &implements);

    private:
        string id; //@todo new introduced field, fully qualified namespace with c/i name
        string type;
        string typeLower;
        string usage;
        string alias;
        string raw;
        string replace;
        bool isMain = false;
        string extends;
        vector<string> implements;

    };
}


#endif //PHPCONVERT_PREPAREDTYPE_H
