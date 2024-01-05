#
# Created by Bram Nijenkamp on 05-01-2024.
#

cmake_minimum_required(VERSION 3.14)

# Version Info - Determine DateTime
string(TIMESTAMP YEAR_TODAY "%Y" UTC)
string(TIMESTAMP YEAR_TODAY_SHORT "%y" UTC)
string(TIMESTAMP MONTH_TODAY "%m" UTC)
string(TIMESTAMP DAY_TODAY "%d" UTC)
string(TIMESTAMP HOUR_TODAY "%H" UTC)
string(TIMESTAMP MINUTE_TODAY "%m" UTC)

set(VER_YEAR ${YEAR_TODAY})
math(EXPR VER_YEAR_NO_LEAD ${VER_YEAR})
set(VER_YEAR_SHORT ${YEAR_TODAY_SHORT})
math(EXPR VER_YEAR_SHORT_NO_LEAD ${VER_YEAR_SHORT})
set(VER_MONTH ${MONTH_TODAY})
math(EXPR VER_MONTH_NO_LEAD ${VER_MONTH})
set(VER_DAY ${DAY_TODAY})
math(EXPR VER_DAY_NO_LEAD ${VER_DAY})
set(VER_HOUR ${HOUR_TODAY})
math(EXPR VER_HOUR_NO_LEAD ${VER_HOUR})
set(VER_MINUTE ${MINUTE_TODAY})
math(EXPR VER_MINUTE_NO_LEAD ${VER_MINUTE})
set(VER_COPYRIGHT_FROM_YEAR 2005)
math(EXPR VER_COPYRIGHT_FROM_YEAR_NO_LEAD ${VER_COPYRIGHT_FROM_YEAR})

# Version Info - Determine Git Rev
execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        RESULT_VARIABLE _VER_GIT_REF
        OUTPUT_STRIP_TRAILING_WHITESPACE
        OUTPUT_VARIABLE _VER_GIT_REF_STD)
if (NOT _VER_GIT_REF) # e.g. ret == 0
    set(VER_GIT_REF ${_VER_GIT_REF_STD})
else ()
    set(VER_GIT_REF "")
endif ()

# Version Info
set(VER_MAJOR 1)
set(VER_REVISION 0)
set(VER_MINOR ${VER_YEAR_SHORT_NO_LEAD} ${VER_MONTH} ${VER_DAY})
set(VER_BUILD ${VER_HOUR_NO_LEAD} ${VER_MINUTE})
set(VER_STAGE "Alpha")
set(VER_STAGE_NR 1)

# Tooling options
option(SANITIZE_ADDRESS "Build-in bound checking." OFF)
option(STATIC_ANALYSIS "Build-in static code analysis." OFF)
# Testing.
option(BUILD_TESTS "Builds the unit tests." ON)
option(BUILD_COVERAGE "Turns coverage on/off." OFF)
# Documentation and library versioning.
option(LOCAL_VENDOR "Tells the build system to use the local Vendor directory." OFF)
option(BUILD_DOCS "Sets whether to build the documentation." OFF)

# Add additional CMake externals (./CMake)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(fward)

# Setup the standard compiler commands.
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Install Vendor dependencies
# They are installed in the Vendor directory and should be committed to keep previous builds.
# A build server as such should always configure CMake with LOCAL_VENDOR to use that commit.
include(GitVendor)

include(System) # Detects the environment exporting (COMP_ENV64, COMP_ENV32, and the platform (WINDOWS, LINUX, MACOS, PLATFORM_WASM))
include(Tooling) # Adds compiler options for the designated options at the top.
include(Linker) # Adds the possibility of using the mold linker.
include(Format)

configure_file(${PROJECT_SOURCE_DIR}/include/version.template ${PROJECT_SOURCE_DIR}/include/version.h)

set(HEADERS
        ${PROJECT_SOURCE_DIR}/include/version.h
        ${PROJECT_SOURCE_DIR}/include/system.h
        ${PROJECT_SOURCE_DIR}/include/utils/gettimeofday.h
        ${PROJECT_SOURCE_DIR}/include/utils/stl_case_insensitive.h
        ${PROJECT_SOURCE_DIR}/include/utils/temporary.h
)
set(SOURCES
        ${PROJECT_SOURCE_DIR}/src/system.cpp
        ${PROJECT_SOURCE_DIR}/src/utils/gettimeofday.cpp
)
add_library(${PROJECT_NAME}-lib SHARED ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME}-lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-lib)

if (BUILD_TESTS)
    add_subdirectory(${PROJECT_SOURCE_DIR}/test)
endif ()

if (BUILD_DOCS)
    include(Documenting)
endif ()