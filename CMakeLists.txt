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
        VERSION 1.0.2
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

# Get git repo information for embed into C Code
execute_process(
    COMMAND git config --get remote.origin.url
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_REPO_URL
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

# Extract just the repository name from URL
if(GIT_REPO_URL)
    # Remove everything up to the last slash
    string(REGEX REPLACE ".*/([^/]+)$" "\\1" GIT_REPO "${GIT_REPO_URL}")
    # Remove .git extension if present
    string(REGEX REPLACE "\\.git$" "" GIT_REPO "${GIT_REPO}")
else()
    set(GIT_REPO "unknown")
endif()

# Get git commit hash
execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)
if(NOT GIT_COMMIT)
    set(GIT_COMMIT "unknown")
endif()

# Get git branch
execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)
if(NOT GIT_BRANCH)
    set(GIT_BRANCH "unknown")
endif()

# Get build date
string(TIMESTAMP BUILD_DATE "%Y-%m-%d %H:%M:%S")

# Add definitions
add_definitions(
    -DVERSION="${PROJECT_VERSION}"
    -DGIT_REPO="${GIT_REPO}"
    -DGIT_COMMIT="${GIT_COMMIT}"
    -DGIT_BRANCH="${GIT_BRANCH}"
    -DBUILD_DATE="${BUILD_DATE}"
)

message(STATUS "Build Configuration:")
message(STATUS "  Version:    ${PROJECT_VERSION}")
message(STATUS "  Repository: ${GIT_REPO}")
message(STATUS "  Commit:     ${GIT_COMMIT}")
message(STATUS "  Branch:     ${GIT_BRANCH}")
message(STATUS "  Build Date: ${BUILD_DATE}")

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
