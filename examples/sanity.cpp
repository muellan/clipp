/*****************************************************************************
 *
 * demo program - part of CLIPP (command line parsing for modern C++)
 *
 * released under MIT license
 *
 * (c) 2017 André Müller; foss@andremueller-online.de
 *
 *****************************************************************************/

#include <iostream>
#include <string>

#include <clipp.h>


int main(int argc, char* argv[])
{
    using namespace clipp;
    using std::cout;


    auto cli = group( );

    if(parse(argc, argv, cli)) {

    }
    else {
        cout << make_man_page(cli, argv[0]) << '\n';
    }
}
