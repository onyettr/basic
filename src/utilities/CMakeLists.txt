cmake_minimum_required(VERSION 3.28)

set(BASIC_LIB_TARGET utilities)

project(${BASIC_LIB_TARGET}
    DESCRIPTION     "basic utilities library"
    LANGUAGES       C ASM)

add_library(${BASIC_LIB_TARGET} STATIC)

target_include_directories(${BASIC_LIB_TARGET}
           PRIVATE
           ${PROJECT_SOURCE_DIR}/src/utilities
           )

target_sources(${BASIC_LIB_TARGET}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/utilities/symboltable.c
    ${CMAKE_SOURCE_DIR}/src/utilities/binarytree.c
    ${CMAKE_SOURCE_DIR}/src/utilities/utilities.c
    )

#message(STATUS "****************************************************")
message(STATUS "Library                 : " ${BASIC_LIB_TARGET})
#message(STATUS "CMAKE_SYSTEM_PROCESSOR  : " ${CMAKE_SYSTEM_PROCESSOR})
#message(STATUS "CMAKE_SYSTEM_ARCH       : " ${CMAKE_SYSTEM_ARCH})
#message(STATUS "****************************************************")
