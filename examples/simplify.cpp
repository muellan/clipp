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
#include <vector>

#include <clipp.h>


int main(int argc, char* argv[])
{
    using namespace clipp;
    using std::cout;


    std::vector<std::string> files;
    std::vector<int> lines;
    bool zip = false;

    auto cli = (
        "input files" %
            values("file", files),
        "compress results" %
            option("-c", "--compress").set(zip),
        "lines to be ignored" %
            repeatable( option("-i", "--ignore") & integers("line", lines) )
    );

    if(parse(argc, argv, cli)) {
        cout << "files:\n";
        for(const auto& f : files) cout << "    " << f << '\n';
        cout << "lines to be ignored: ";
        for(const auto& l : lines) cout << l << ' ';
        cout << "\nusing" << (zip ? "" : " no") << " compression\n";
    }
    else {
        cout << make_man_page(cli, argv[0]) << '\n';
    }

}
