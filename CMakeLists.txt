cmake_minimum_required(VERSION 3.5)
project(donkeyid)

set(PHP_INCLUDE_H /usr/local/src/php56/include)
#set(PHP_INCLUDE_H /usr/local/php54/include)
#set(PHP_INCLUDE_H /usr/local/php7/include)
INCLUDE_DIRECTORIES(
        /usr/local/include
        ${PHP_INCLUDE_H}/php
        ${PHP_INCLUDE_H}/php/main
        ${PHP_INCLUDE_H}/php/TSRM
        ${PHP_INCLUDE_H}/php/Zend

)
#AUX_SOURCE_DIRECTORY( /home/vic/clion/php-src/Zend DIR_SRCS)
#AUX_SOURCE_DIRECTORY( /home/vic/workspace/php/Zend DIR_SRCS)
AUX_SOURCE_DIRECTORY( /home/vic/workspace/php-5.6.14/Zend DIR_SRCS)


add_custom_target(makefile COMMAND phpize --clean && phpize && ./configure && make && make install  WORKING_DIRECTORY .)

set(SOURCE_FILES donkeyid/donkeyid.c donkeyid/src/donkeyid.c donkeyid/src/shm.c donkeyid/src/spinlock.c)
add_executable(donkeyid ${SOURCE_FILES})



#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB_RECURSE SRC_LIST FOLLOW_SYMLINKS donkeyid/src/*.c)
file(GLOB_RECURSE HEAD_FILES FOLLOW_SYMLINKS donkeyid/src/*.h)
message("${HEAD_FILES}")
INCLUDE_DIRECTORIES(BEFORE ./donkeyid/src)
set(DONKEYTEST_FILES donkeyid/tests/main.c)
add_executable(donkeytest ${DONKEYTEST_FILES};${SRC_LIST})