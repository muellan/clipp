/*****************************************************************************
 *
 * CLIPP - command line interfaces for modern C++
 *
 * released under MIT license
 *
 * (c) 2017 André Müller; foss@andremueller-online.de
 *
 *****************************************************************************/

#include "testing.h"


//-------------------------------------------------------------------
/** @brief test if parameter factory call expressions compile
 */
int main(int argc, char* argv[])
{
    using namespace clipp;

    std::string a = "-a";
    std::string b = "-b";
    std::string c = "-c";

    auto cli = (
        option("-a", "-b", "-c"),
        option(a, "-b", "-c"),
        option("-a", b, "-c"),
        option("-a", "-b", c),
        option(a, b, "-c"),
        option(a, "-b", c),
        option("-a", b, c),
        option(a, b, c)
        ,
        required("-a", "-b", "-c"),
        required(a, "-b", "-c"),
        required("-a", b, "-c"),
        required("-a", "-b", c),
        required(a, b, "-c"),
        required(a, "-b", c),
        required("-a", b, c),
        required(a, b, c)
        ,
        command("-a", "-b", "-c"),
        command(a, "-b", "-c"),
        command("-a", b, "-c"),
        command("-a", "-b", c),
        command(a, b, "-c"),
        command(a, "-b", c),
        command("-a", b, c),
        command(a, b, c)
    );

    parse(argc, argv, cli);
}
