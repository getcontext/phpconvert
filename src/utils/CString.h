//
// Created by wizard on 18.12.2019.
//

#ifndef PHPCONVERT_CSTRING_H
#define PHPCONVERT_CSTRING_H

//process cstring type char*
class CString {
public:
    static const char *COMMENTS[];
    static const char *WHITE_CHARS[];

    CString(const char *source);

    const char *findBetween(char start, char stop);

    const char *stripComments();

    const char *iterate();

    bool isWhite(const char someChar);

private:
    const char *source;

};


#endif //PHPCONVERT_CSTRING_H
