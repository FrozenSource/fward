cmake_minimum_required(VERSION 3.14)

project(tests)

set(HEADERS
        ${PROJECT_SOURCE_DIR}/test.h
)
set(SOURCES
        ${PROJECT_SOURCE_DIR}/main-test.cpp
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

# Used as framework for unit testing.
set(DOCTEST_WITH_MAIN_IN_STATIC_LIB OFF)
InstallVendor(doctest https://github.com/doctest/doctest.git master) #

target_link_libraries(${PROJECT_NAME} PRIVATE fward-lib doctest::doctest)