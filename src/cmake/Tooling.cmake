if (STATIC_ANALYSIS)
    set(CMAKE_C_CLANG_TIDY
            clang-tidy;
            -header-filter=./Include;
            -checks=*;)
    set(CMAKE_CXX_CLANG_TIDY
            clang-tidy;
            -header-filter=./Include;
            -checks=*;)

    find_program(CMAKE_CXX_INCLUDE_WHAT_YOU_USE NAMES include-what-you-use iwyu)
    if (CMAKE_CXX_INCLUDE_WHAT_YOU_USE)
        message(STATUS "iwyu: Enabled")
    else ()
        message(STATUS "iwyu: Not available")
    endif ()

    set(CMAKE_LINK_WHAT_YOU_USE TRUE)
endif ()

if (SANITIZE_ADDRESS)
    if (MSVC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /fsanitize=address /Zi")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fsanitize=address /Zi")
        set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} /fsanitize=address /Zi")
    elseif (PLATFORM_LINUX OR (PLATFORM_UNIX AND NOT PLATFORM_MACOS))
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize=address,pointer-compare,pointer-subtract,shift,leak -fsanitize-address-use-after-scope")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address,pointer-compare,pointer-subtract,shift,leak -fsanitize-address-use-after-scope")
        set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address,pointer-compare,pointer-subtract,shift,leak -fsanitize-address-use-after-scope")
    elseif (PLATFORM_WASM)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
        set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=undefined")
    else ()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize=address,pointer-compare,pointer-subtract,shift,nullability -fsanitize-address-use-after-scope")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address,pointer-compare,pointer-subtract,shift,nullability -fsanitize-address-use-after-scope")
        set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address,pointer-compare,pointer-subtract,shift,nullability -fsanitize-address-use-after-scope")
    endif ()
endif ()

if (USE_VALGRIND)
    find_program(VALGRIND_BINPATH valgrind)
    if (VALGRIND_BINPATH)
        set(MEMORYCHECK_COMMAND ${VALGRIND_BINPATH})
        set(MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full")
        message(STATUS "valgrind: Enabled")
    else ()
        message(STATUS "valgrind: Not available")
    endif ()
endif ()

find_program(INCLUDE_WHAT_YOU_USE_BINPATH include-what-you-use iwyu)
if (INCLUDE_WHAT_YOU_USE_BINPATH)
#    set(CMAKE_C_INCLUDE_WHAT_YOU_USE ${INCLUDE_WHAT_YOU_USE_BINPATH})
#    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${INCLUDE_WHAT_YOU_USE_BINPATH})
    message(STATUS "include-what-you-use: Enabled")
else ()
    message(STATUS "include-what-you-use: Not available")
endif ()