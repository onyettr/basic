cmake_minimum_required(VERSION 3.28)

set(BASIC_LIB_TARGET dtss)

project(${BASIC_LIB_TARGET}
    DESCRIPTION     "basic dtss library"
    LANGUAGES       C ASM)

add_library(${BASIC_LIB_TARGET} STATIC)
    
target_include_directories(${BASIC_LIB_TARGET}
           PRIVATE
           ${PROJECT_SOURCE_DIR}/src/dtss
           )

target_sources(${BASIC_LIB_TARGET}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/dtss/dtss.c
    )

#message(STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})
#message(STATUS "****************************************************")
message(STATUS "Library                 : " ${BASIC_LIB_TARGET})
#message(STATUS "CMAKE_SYSTEM_PROCESSOR  : " ${CMAKE_SYSTEM_PROCESSOR})
#message(STATUS "CMAKE_SYSTEM_ARCH       : " ${CMAKE_SYSTEM_ARCH})
#message(STATUS "****************************************************")
