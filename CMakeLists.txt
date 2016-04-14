cmake_minimum_required(VERSION 3.5)
project(donkeyid)

INCLUDE_DIRECTORIES(
        /usr/local/include
        /usr/local/src/php56/include/php
        /usr/local/src/php56/include/php/main
        /usr/local/src/php56/include/php/TSRM
        /usr/local/src/php56/include/php/Zend
)
AUX_SOURCE_DIRECTORY( /home/vic/workspace/php-5.6.14/Zend DIR_SRCS)

add_custom_target(makefile COMMAND phpize --clean && phpize && ./configure && make && make install  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

set(SOURCE_FILES donkeyid.c donkey_id.c)
add_executable(donkeyid ${SOURCE_FILES})



#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#
set(DONKEYTEST_FILES ./src/donkeyid.c ./src/shm.c )
add_executable(donkeytest ${DONKEYTEST_FILES})