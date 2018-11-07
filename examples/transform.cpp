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

    std::string infile;
    bool tr = false, rot = false;
    double x = 0, y = 0, z = 0;
    double phi = 0, theta = 0;

    auto cli = (
        value("geometry file", infile),
        option("-translate").set(tr) & value("x", x) & value("y", y) & value("z", z),
        option("-rotate").set(rot) & value("azimuth", phi) & value("polar", theta)
    );

    if(parse(argc, argv, cli)) {
        if(!tr && !rot) {
            cout << "nothing will be done with file '" << infile << "'\n";
        }
        else {
            cout << "transforming content of file '" << infile << "'\n";
            if(tr) {
                cout << "translating objects by vector ("
                     << x << ", " << y << ", " << z << ")\n";
            }
            if(rot) {
                cout << "rotating objects about azimuth = " << phi
                     << " and polar angle = " << theta << '\n';
            }
        }
    }
    else {
        cout << "Usage:\n" << usage_lines(cli, argv[0]) << '\n';
    }
}
