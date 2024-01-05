//
// Created by Bram Nijenkamp on 05-01-2024.
//

#pragma once
#include "system.h"

#define DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES
#define DOCTEST_CONFIG_NO_EXCEPTIONS
#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#define DOCTEST_BREAK_INTO_DEBUGGER() ((void) 0)

#if defined(_WIN32) || defined(_WIN64)
    #pragma warning(push)
    #pragma warning(disable : 4805)
#endif
#include <doctest/doctest.h>
#if defined(_WIN32) || defined(_WIN64)
    #pragma warning(pop)
#endif