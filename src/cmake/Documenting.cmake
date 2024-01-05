# This CMake file looks up doxygen which is a documentation tool
# It reads back all function signatures and the comments above them.
# Based on this it creates HTML and other types of visual documentation.
find_package(Doxygen
        REQUIRED dot)
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/Doxyfile)
set(DOXYGEN_OUT ${PROJECT_SOURCE_DIR}/Doxyfile)
doxygen_add_docs(${PROJECT_NAME}-docs
        ${PROJECT_SOURCE_DIR}/Include
        ${PROJECT_SOURCE_DIR}/Src
        COMMENT "Generating documentation..."
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
