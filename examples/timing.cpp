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
    using std::string;
    using std::cout;

    int n = 1;
    bool errStop = false;
    string exe;
    std::vector<string> args;

    auto cli = (
        option("-n", "--repeat") & value("times", n) % "execute multiple times",
        option("-s", "--stop-on-error").set(errStop) % "stop on error",
        value("executable", exe)                     % "client program",
        option("--") & values("args", args)          % "client arguments"
    );

    if(!parse(argc, argv, cli)) {
        cout << make_man_page(cli, argv[0]) << '\n';
    }

    cout << "call: " << exe;
    for(const auto& a : args) cout << ' ' << a;
    cout << '\n';
    cout << n << " times\n";
    if(errStop) cout << "execution will be stopped on any error\n";
}
