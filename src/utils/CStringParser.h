//
// Created by wizard on 18.12.2019.
//

#ifndef PHPCONVERT_CSTRINGPARSER_H
#define PHPCONVERT_CSTRINGPARSER_H

//process cstring type char*
class CStringParser {
public:
    CStringParser(const char *source);
    const char* findBetween(char start, char stop);
private:
    const char* source;

};


#endif //PHPCONVERT_CSTRINGPARSER_H
