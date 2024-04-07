cmake_minimum_required(VERSION 3.28)

# Cut down the chatter from CMake
set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")
set(CMAKE_VERBOSE_MAKEFILE  0)

set(CMAKE_C_COMPILER                gcc)
set(CMAKE_ASM_COMPILER              gcc)
set(CMAKE_CXX_COMPILER              g++)
set(CMAKE_LINKER                    ld)
set(CMAKE_CROSSCOMPILING            true)
set(CMAKE_SYSTEM_NAME               Generic)

find_program(CMAKE_C_COMPILER       gcc)
find_program(CMAKE_CXX_COMPILER     g++)

set(PROJECT_NAME "basic")
project(${PROJECT_NAME} 
        VERSION 1.0 
        LANGUAGES C CXX)

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
target_link_libraries(${PROJECT_NAME} PRIVATE 
                      lister
                      misc
                      tokenizer
                      dtss
                      utilities
                      )
 
install(TARGETS misc basic)