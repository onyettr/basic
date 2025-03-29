# Project: Basic interpretor
cmake_minimum_required(VERSION 3.28)

# Cut down the chatter from CMake
set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")
set(CMAKE_VERBOSE_MAKEFILE  0)

set(CMAKE_C_STANDARD                99)
set(CMAKE_C_COMPILER                gcc)
set(CMAKE_ASM_COMPILER              gcc)
set(CMAKE_CXX_COMPILER              g++)
set(CMAKE_LINKER                    ld)
set(CMAKE_CROSSCOMPILING            true)
set(CMAKE_SYSTEM_NAME               Generic)

find_program(CMAKE_C_COMPILER       gcc)
find_program(CMAKE_CXX_COMPILER     g++)

# Adding MATH library support
find_library(MATH_LIBRARY m PATHS /usr/lib64/)

# Generate Docs
OPTION(BUILD_WITH_DOCS "Generate Docs" ON)
IF(TASKSCH_BUILD_WITH_DOCS)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/docs)
ENDIF()

set(PROJECT_NAME "basic")
project(${PROJECT_NAME}
        VERSION 1.0
        LANGUAGES C CXX)
if(MATH_LIBRARY)
  message("[INFO] Math library found")
  LINK_LIBRARIES(${MATH_LIBRARY})
else()
  message("MATH library not found")
endif( MATH_LIBRARY )

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    )

add_executable(${PROJECT_NAME} src/main.c)

target_sources(${PROJECT_NAME} PRIVATE "src/main.c")

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}")

add_subdirectory("src/misc")
add_subdirectory("src/dtss")
add_subdirectory("src/lister")
add_subdirectory("src/utilities")
add_subdirectory("src/tokenizer")
add_subdirectory("docs")

target_link_libraries(${PROJECT_NAME} PRIVATE
                      lister
                      misc
                      tokenizer
                      dtss
                      utilities
                    )
target_link_libraries(${PROJECT_NAME} PUBLIC m)
install(TARGETS misc basic)
