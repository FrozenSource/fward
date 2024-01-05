if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build Type" FORCE)
endif ()

if (NOT "${CMAKE_BUILD_TYPE}"
        MATCHES "^(Debug|Release|RelWithDebInfo|MinSizeRel)\$")
    message(FATAL_ERROR
            "Unknown keyword for CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}\n"
            "Acceptable keywords: Debug,Release,RelWithDebInfo,MinSizeRel")
endif ()

# Detect the cpu platform.
if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
    set(COMP_ENV64 ON)
    set(COMP_ENV32 OFF)
    message(STATUS "Architecture: 64-bit @ ${CMAKE_BUILD_TYPE}")
elseif ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
    set(COMP_ENV32 ON)
    set(COMP_ENV64 OFF)
    message(STATUS "Architecture: 32-bit @ ${CMAKE_BUILD_TYPE}")
else ()
    set(COMP_ENV32 OFF)
    set(COMP_ENV64 OFF)
    message(WARN "Architecture: Unknown @ ${CMAKE_BUILD_TYPE}")
endif ()

# Detect what system we are building for.
if (EMSCRIPTEN)
    set(PLATFORM_WASM ON)
    message(STATUS "Platform: WASM")
elseif (BUILD_WASI)
    set(PLATFORM_WASI ON)
    message(STATUS "Platform: WASI")
    add_compile_options(
            -DHAVE_DECL__ISNAN
            -DHAVE_DECL_ISINF
            -DHAVE_SNPRINTF
            -D_WASI_EMULATED_SIGNAL
            --target=wasm32-unknown-wasi
    )
    add_link_options(
            -lwasi-emulated-signal
            --target=wasm32-unknown-wasi
    )
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(PLATFORM_WINDOWS ON)
    message(STATUS "Platform: Windows")
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(PLATFORM_MACOS ON)
    message(STATUS "Platform: MacOS")
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(PLATFORM_LINUX ON)
    message(STATUS "Platform: Linux")
elseif (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
    set(PLATFORM_UNIX ON)
    message(STATUS "Platform: Unix")
else ()
    message(ERROR "Platform: Unknown")
endif ()