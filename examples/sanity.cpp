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
#include <string>

#include <clipp.h>


int EXAMPLE_MAIN()
{
    using namespace clipp;
    using std::cout;


    auto cli = group(
        option("-a"),
        option("-ab")
    );

    cout << "flags are " << (cli.flags_are_prefix_free() ? "" : "not ")
         << "prefix free\n";

    return 0;
}
