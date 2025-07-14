//
// Created by wizard on 14.07.2025.
//

#ifndef PHPCONVERT_LINUXKERNELPARSER_H
#define PHPCONVERT_LINUXKERNELPARSER_H

#include "../BaseParser.h"
#include "../utils/DirectoryReader.h"
#include "../utils/RegexHelper.h"
#include "../utils/StringHelper.h"

namespace phpconvert {

    class LinuxKernelParser : public BaseParser {
        LinuxKernelParser();
        virtual ~LinuxKernelParser();

    };

} // phpconvert

#endif //PHPCONVERT_LINUXKERNELPARSER_H
