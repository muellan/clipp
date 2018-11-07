/*****************************************************************************
 *
 * demo program - part of CLIPP (command line interfaces for modern C++)
 *
 * released under MIT license
 *
 * (c) 2017-2018 André Müller; foss@andremueller-online.de
 *
 *****************************************************************************/

#include <iostream>
#include <string>

#include <clipp.h>


int main(int argc, char* argv[])
{
    using namespace clipp;
    using std::cout;

    bool recurse = false;
    std::string inpath, outpath;

    auto cli = (
        option("-r", "--recursive").set(recurse)                 % "recurse into subdirectories",
        (required("-i", "--in" ) & value("input dir", inpath))   % "sets path to input directory",
        (required("-o", "--out") & value("output dir", outpath)) % "sets path to output directory"
    );

    if(parse(argc, argv, cli)) {
        cout << "read from " << inpath
             << (recurse ? " (recursively)" : " (non-recursively)")
             << " and write to " << outpath << '\n';
    }
    else {
        cout << make_man_page(cli, argv[0]) << '\n';
    }
}
