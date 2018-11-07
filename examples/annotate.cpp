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

#include <clipp.h>


int main(int argc, char* argv[])
{
    using namespace clipp;
    using std::cout;

    auto is_char = [](const std::string& arg) {
        return arg.size() == 1 && std::isalpha(arg[0]);
    };

    char lbl = ' ';
    auto cli = (
        command("auto").set(lbl, '_') |
        ( command("label"), value(is_char, "character", lbl) )
    );

    if(parse(argc, argv, cli)) {
        cout << "Label: " << lbl << '\n';
    }
    else {
        cout << "Usage:\n" << usage_lines(cli,argv[0]) << '\n';
    }
}
