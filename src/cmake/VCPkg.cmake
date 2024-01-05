include(FetchContent)
set(VCPKG_TOOLCHAIN_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/vcpkg")
FetchContent_Declare(
        vcpkg
        GIT_REPOSITORY https://github.com/microsoft/vcpkg
        GIT_TAG 2022.08.15
        SOURCE_DIR ${VCPKG_TOOLCHAIN_DIRECTORY}
)
FetchContent_MakeAvailable(vcpkg)

set(CMAKE_TOOLCHAIN_FILE ${VCPKG_TOOLCHAIN_DIRECTORY}/scripts/buildsystems/vcpkg.cmake)

message(STATUS "Found: VCPkg, Toolchain: ${CMAKE_TOOLCHAIN_FILE}")

if (NOT EXISTS "${VCPKG_TOOLCHAIN_DIRECTORY}/vcpkg")
    if (WIN32)
        execute_process(
                COMMAND ${VCPKG_TOOLCHAIN_DIRECTORY}/bootstrap-vcpkg.bat -disableMetrics
                WORKING_DIRECTORY ${VCPKG_TOOLCHAIN_DIRECTORY}
                RESULT_VARIABLE VCPKG_BOOTSTRAP_RESULT
                OUTPUT_QUIET
        )
    else ()
        execute_process(
                COMMAND chmod +x ${VCPKG_TOOLCHAIN_DIRECTORY}/bootstrap-vcpkg.sh
                WORKING_DIRECTORY ${VCPKG_TOOLCHAIN_DIRECTORY}
                RESULT_VARIABLE VCPKG_OWN_RESULT
                OUTPUT_QUIET
        )
        if (NOT VCPKG_OWN_RESULT EQUAL "0")
            message(FATAL_ERROR "VCPkg could own bootstrap, output: ${VCPKG_OWN_RESULT}")
        endif ()
        execute_process(
                COMMAND ${VCPKG_TOOLCHAIN_DIRECTORY}/bootstrap-vcpkg.sh -disableMetrics
                WORKING_DIRECTORY ${VCPKG_TOOLCHAIN_DIRECTORY}
                RESULT_VARIABLE VCPKG_BOOTSTRAP_RESULT
                OUTPUT_QUIET
        )
    endif ()
    if (NOT VCPKG_BOOTSTRAP_RESULT EQUAL "0")
        message(FATAL_ERROR "VCPkg could not bootstrap, output: ${VCPKG_BOOTSTRAP_RESULT}")
    endif ()
endif ()

function(vcpkg_install package_name recursive)
    if (recursive)
        set(__recusrive "--recurse")
    else ()
        set(__recusrive "")
    endif ()
    execute_process(
            COMMAND ${VCPKG_TOOLCHAIN_DIRECTORY}/vcpkg install ${package_name} ${__recusrive}
            WORKING_DIRECTORY ${VCPKG_TOOLCHAIN_DIRECTORY}
            RESULT_VARIABLE VCPKG_INSTALL_RESULT
    )
    if (NOT VCPKG_INSTALL_RESULT EQUAL "0")
        message(FATAL_ERROR "VCPkg could not install package: '${package_name}', output: ${VCPKG_INSTALL_RESULT}")
    endif ()
endfunction()