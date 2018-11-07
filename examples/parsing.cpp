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

    auto cli = (
        command("make"),
        value("file")           % "name of file to make",
        option("-f", "--force") % "overwrite existing file"
    );


    parse({"make", "out.txt"}, cli);


    auto args = std::vector<std::string> {"make", "out.txt", "-f"};
    parse(args, cli);


    auto result = parse(argc, argv, cli);

    clipp::debug::print(std::cout, result);
}
