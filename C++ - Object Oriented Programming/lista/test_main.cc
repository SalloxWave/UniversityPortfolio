//
// Created by eric on 8/25/16.
//
// This file exists to speed up compilation
// of the catch library.
//
// You only need to compile this file once,
// and then later on include the resulting
// object file (test_main.o) in your compilation.
//
// Compile catch:
// $ g++ -c test_main.cc
//
// And then when compiling your testcases:
// $ g++ <your files> test_main.o
//
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

int main(int argc, char* argv[])
{
    return Catch::Session().run(argc, argv);
}
