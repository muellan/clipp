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
    std::string expr;
    bool ifany = false, ifall = false;

    auto cli = (
        values("file", files)                  % "input filenames",
        (required("-s") & value("expr", expr)) % "string to look for",
        option("any").set(ifany)               % "report as soon as any matches" |
        option("all").set(ifall)               % "report only if all match"
    );

    if(parse(argc, argv, cli)) {
        cout << "find '" << expr << "' in files: ";
        for(const auto& f : files) { cout << "'" << f << "' "; } cout << '\n';
        if(ifany) cout << "using 'any' mode\n";
        if(ifall) cout << "using 'all' mode\n";
    }
    else {
        cout << make_man_page(cli, argv[0]) << '\n';
    }

}
