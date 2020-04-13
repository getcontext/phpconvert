//
// Created by wizard on 14.11.2019.
//

#ifndef PHPCONVERT_FILE_H
#define PHPCONVERT_FILE_H

namespace phpconvert {
    class File {
    public:
        const string &getName() const;;

        void setName(const string &name);

        const string &getFullPath() const;

        void setFullPath(const string &fullPath);

        const string &getRootPath() const;

        void setRootPath(const string &rootPath);

        const string &getNamespaceName() const;

        void setNamespaceName(const string &namespaceName);

        bool isValid1() const;

        void setIsValid(bool isValid);

        const string &getMainType() const;

        void setMainType(const string &mainType);

        const string &getFirstMainTypeFull() const;

        void setFirstMainTypeFull(const string &firstMainTypeFull);

        const string &getContent() const;

        void setContent(const string &content);

        set<string> *getMainTypes() const;

        void setMainTypes(set<string> *mainTypes);

        const vector<string> &getTypes() const;

        void setTypes(const vector<string> &types);

        const vector<PreparedType> &getPrepTypes() const;

        void setPrepTypes(const vector<PreparedType> &prepTypes);

        const vector<PreparedType> &getPrepTypesMain() const;

        void setPrepTypesMain(const vector<PreparedType> &prepTypesMain);

    private:
        string name;
        string fullPath;
        string rootPath;
        string namespaceName;
        bool isValid;
        string mainType;
        string firstMainTypeFull;
        string content;

        set<string> *mainTypes;
        vector<string> types;
        vector<PreparedType> prepTypes;
        vector<PreparedType> prepTypesMain;
    };


#endif //PHPCONVERT_FILE_H
