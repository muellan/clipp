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

    int n = 0;
    bool domerge = false;
    long m = 5;
    auto print_ratio = [](const char* r) { cout << "using ratio of " << r << '\n'; };

    auto cli = (
        (option("-n", "--count") & value("count", n))           % "number of iterations",
        (option("-r", "--ratio") & value("ratio", print_ratio)) % "compression ratio",
        (option("-m") & opt_value("lines=5", m).set(domerge))   % "merge lines (default: 5)"
    );

    if(parse(argc, argv, cli)) {
        cout << "performing "  << n << " iterations\n";
        if(domerge) cout << "merge " << m << " lines\n";
    }
    else {
        cout << make_man_page(cli, argv[0]) << '\n';
    }
}
