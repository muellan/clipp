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

    std::vector<std::string> files;
    std::vector<int> lines;
    bool zip = false;

    auto cli = (
        values("directory", files)                         % "input files",
        option("-c", "--compress").set(zip)                % "compress results",
        (option("-i", "--ignore") & values("line", lines)) % "lines to be ignored"
    );

    if(parse(argc, argv, cli)) {
        cout << "input files:";
        for(const auto& f : files) cout << ' ' << f;
        cout << "\n";
        cout << (zip ? "" : "don't ") << "compress results\n";
        cout << "ignored lines:";
        for(const auto& l : lines) cout << ' ' << l;
        cout << "\n";
    }
    else {
        cout << make_man_page(cli, argv[0]) << '\n';
    }


}
