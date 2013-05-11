project(InheritanceTest)
cmake_minimum_required(VERSION 2.8)

set( ${PROJECT_NAME}_lib_sources
    "diamond.h"
    "dumbpointer.h"
    "missiles.h"
    "nestedclasses.h"
    "vehicletree.h"
)

set( ${PROJECT_NAME}_exe_sources
    "main.cpp"
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_lib_sources} ${${PROJECT_NAME}_exe_sources} )


