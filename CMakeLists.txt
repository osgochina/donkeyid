cmake_minimum_required(VERSION 3.5)
project(donkeyid)
INCLUDE_DIRECTORIES(
/usr/local/include
/usr/local/php54/include/php
/usr/local/php54/include/php/main
/usr/local/php54/include/php/TSRM
/usr/local/php54/include/php/Zend
)
AUX_SOURCE_DIRECTORY( /home/vic/workspace/php/Zend DIR_SRCS)


add_custom_target(makefile COMMAND phpize && ./configure && make && make install  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#
set(SOURCE_FILES donkeyid.c)
#
add_executable(donkeyid ${SOURCE_FILES})