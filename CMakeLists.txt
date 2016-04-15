cmake_minimum_required(VERSION 3.5)
project(donkeyid)

set(PHP_INCLUDE_H /usr/local/src/php56/include)
INCLUDE_DIRECTORIES(
        /usr/local/include
        ${PHP_INCLUDE_H}/php
        ${PHP_INCLUDE_H}/php/main
        ${PHP_INCLUDE_H}/php/TSRM
        ${PHP_INCLUDE_H}/php/Zend
)
#AUX_SOURCE_DIRECTORY( /home/vic/workspace/php/Zend DIR_SRCS)
AUX_SOURCE_DIRECTORY( /home/vic/workspace/php-5.6.14/Zend DIR_SRCS)

add_custom_target(makefile COMMAND phpize --clean && phpize && ./configure && make && make install  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

set(SOURCE_FILES donkeyid.c donkey_id.c)
add_executable(donkeyid ${SOURCE_FILES})



#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#
set(DONKEYTEST_FILES ./src/donkeyid.c ./src/shm.c ./src/spinlock.c)
add_executable(donkeytest ${DONKEYTEST_FILES})