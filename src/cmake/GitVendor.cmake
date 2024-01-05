# Function for installing third-party applications.

# If LOCAL_VENDOR is true then the Vendor directory will be checked first.
# If not found, it will fetch from a git repository.
if (LOCAL_VENDOR)
    if (EXISTS "${PROJECT_SOURCE_DIR}/Vendor")
        file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/Vendor")
    endif ()
endif ()

if (NOT InstallVendor_defined)
    include(FetchContent)
    function(InstallVendor project_name git_repo branch)
        if (NOT ${project_name}_included)
            # If LOCAL_VENDOR first check for a CMakefile.
            # If not found, download it to the Vendor directory.
            if (LOCAL_VENDOR)
                if (EXISTS "${PROJECT_SOURCE_DIR}/Vendor/${project_name}/CMakeLists.txt")
                    add_subdirectory(${PROJECT_SOURCE_DIR}/Vendor/${project_name} ${CMAKE_CURRENT_BINARY_DIR}/${project_name})
                else ()
                    FetchContent_Declare(
                            ${project_name}
                            GIT_REPOSITORY ${git_repo}
                            GIT_TAG ${branch}
                            GIT_PROGRESS TRUE
                            GIT_SHALLOW TRUE
                            SOURCE_DIR "${PROJECT_SOURCE_DIR}/Vendor/${project_name}"
                            BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/${project_name}"
                    )
                    FetchContent_MakeAvailable(${project_name})
                    file(REMOVE_RECURSE "${PROJECT_SOURCE_DIR}/Vendor/${project_name}/.git")
                endif ()
            else ()
                # Otherwise we fetch to the binary directory.
                FetchContent_Declare(
                        ${project_name}
                        GIT_REPOSITORY ${git_repo}
                        GIT_TAG ${branch}
                        SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/${project_name}"
                        BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/${project_name}"
                )
                FetchContent_MakeAvailable(${project_name})
            endif ()
            set(${project_name}_included CACHE INTERNAL "")
            message(STATUS "Found ${project_name} @ ${branch}")
        endif ()
    endfunction()
    set(InstallVendor_defined CACHE INTERNAL "")
endif ()