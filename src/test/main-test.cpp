//
// Created by Bram Nijenkamp on 05-01-2024.
//

#define DOCTEST_CONFIG_IMPLEMENT
#include "test.h"

int32_t main(int argc, char** argv) {
    auto ctx = doctest::Context(argc, argv);
    return ctx.run();
}