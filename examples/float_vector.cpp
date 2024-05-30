/*****************************************************************************
 *
 * demo program - part of CLIPP (command line interfaces for modern C++)
 *
 * released under MIT license
 *
 * (c) 2017-2018 André Müller; foss@andremueller-online.de
 *
 *****************************************************************************/

#include "pch.h"
#include "example-def.h" // EXAMPLE_MAIN

#include <iostream>
#include <vector>
#include <cmath>

#include <clipp.h>


int EXAMPLE_MAIN(int argc, char* argv[])
{
    using namespace clipp;
    using std::cout;

    std::vector<float> nums;

    auto cli = required("--data") & numbers("number", nums);

    if(parse(argc, argv, cli)) {
        cout << "numbers:\n";
        for(auto n : nums) cout << n << '\n';
    } else {
        cout << "Usage:\n" << usage_lines(cli, argv[0]) << '\n';
    }
    return 0;
}
