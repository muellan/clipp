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

#include <clipp.h>


int main(int argc, char* argv[])
{
    using namespace clipp;
    using std::cout;

    bool a = false, b = false, c = true; //target variables

    auto cli = (
        option("-a").set(a)                  % "activates a",
        option("-b").set(b)                  % "activates b",
        option("-c", "--noc").set(c,false)   % "deactivates c",
        option("--hi")([]{cout << "hi!\n";}) % "says hi");

    if(parse(argc, argv, cli)) {
        cout << std::boolalpha
             << "a=" << a << "\nb=" << b << "\nc=" << c << '\n';
    } else {
        cout << make_man_page(cli, argv[0]) << '\n';
    }


// alternative style 1: member functions instead of operators
//    auto cli = (
//        option("-b").set(b).doc("activates b"),
//        option("-c", "--noc").set(c,false).doc("deactivates c"),
//        option("--hi").call([]{cout << "hi!\n";}).doc("says hi") );


// alternative style 2: even more operators
//    auto cli = (
//        option("-b")          % "activates b"   >> b,
//        option("-c", "--noc") % "deactivates c" >> set(c,false),
//        option("--hi")        % "says hi"       >> []{cout << "hi!\n";} );

//    auto cli = (
//        option("-b")          % "activates b"   >> b,
//        option("-c", "--noc") % "deactivates c" >> set(c,false),
//        option("--hi")        % "says hi"       >> []{cout << "hi!\n";} );

//    auto cli = (
//        b                    << option("-b")          % "activates b",
//        set(c,false)         << option("-c", "--noc") % "deactivates c",
//        []{cout << "hi!\n";} << option("--hi")        % "says hi" );

//    auto cli = (
//        "activates b"   % option("-b")          >> b,
//        "deactivates c" % option("-c", "--noc") >> set(c,false),
//        "says hi"       % option("--hi")        >> []{cout << "hi!\n";} );

}
