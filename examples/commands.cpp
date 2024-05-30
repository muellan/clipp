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


int EXAMPLE_MAIN(int argc, char* argv[])
{
    using namespace clipp;
    using std::cout;

    std::string fname;
    std::string enc = "utf8";

    auto cli = (
        command("new"),
        value("filename", fname),
        (option("-e", "--encoding") & value("enc", enc)).doc("'utf8' or 'cp1252', default is " + enc)

    );

    if(parse(argc, argv, cli)) {
        cout << "making file " << fname << " with encoding " << enc << '\n';
    }
    else {
        cout << "Usage:\n" << usage_lines(cli,argv[0]) << '\n';
    }
    return 0;
}
