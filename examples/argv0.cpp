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
#include <vector>
#include <cmath>

#include <clipp.h>


int main(int argc, char* argv[])
{
    using namespace clipp;
    using std::cout;

    std::string progcall;
    bool x = false;
    bool y = false;

    auto cli = (
            value("", progcall),
            option("-x").set(x),
            option("-y").set(y)
        );

    //parse all args including argv[0]
    if(parse(argv, argv+argc, cli)) {
        cout << "program call: " << progcall << '\n';
        if(x) cout << "x\n";
        if(y) cout << "y\n";
    } else {
        cout << "Usage:\n" << usage_lines(cli,argv[0]) << '\n';
    }
}
