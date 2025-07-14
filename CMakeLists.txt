cmake_minimum_required(VERSION 3.13)
project(phpconvert)
#ADD_DEFINITIONS(-std=c++11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_STANDARD 11)

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

set(Boost_INCLUDE_DIR /home/wizard/apps/boost_1_66_0)
set(Boost_LIBRARY_DIR ${Boost_INCLUDE_DIR}/stage/lib/)

include_directories(${Boost_INCLUDE_DIR})

include_directories(src)
include_directories(src/plugin)
include_directories(src/utils)

find_package(Boost 1.66.0 REQUIRED)
find_package(Boost 1.66.0 REQUIRED COMPONENTS thread system filesystem regex wave)

link_directories(${Boost_LIBRARY_DIR})


add_executable(
        phpconvert
        src/plugin/ZendParser.cpp
        src/plugin/ZendParser.h
        src/plugin/ZendParser_test.cpp
        src/utils/Bench.cpp
        src/utils/Bench.h
        src/utils/DirectoryReader.cpp
        src/utils/DirectoryReader.h
        src/utils/DirectoryReader_test.cpp
        src/utils/RegexHelper.cpp
        src/utils/RegexHelper.h
        src/utils/Regexer_test.cpp
        src/utils/StringHelper.cpp
        src/utils/StringHelper.h
        src/AppManager.cpp
        src/AppManager.h
        src/AppManager_test.cpp
        src/AppParams.cpp
        src/AppParams.h
        src/AppParams_test.cpp
        src/BaseParser.cpp
        src/BaseParser.h
        src/Parser_test.cpp
        src/ParserManager.cpp
        src/ParserManager.h
        src/ParserManager_test.cpp
        src/phpconvert.cpp
        src/SystemException.cpp
        src/SystemException.h
        src/SystemException_test.cpp
        src/plugin/Php7Parser.cpp
        src/plugin/Php7Parser.h
        src/plugin/LinuxKernelParser.cpp
        src/plugin/LinuxKernelParser.h
        #        src/utils/CString.cpp
#        src/utils/CString.h
)

target_link_libraries(phpconvert boost_thread boost_system boost_filesystem boost_regex boost_wave)
