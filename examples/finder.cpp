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
    using std::string;

    enum class mode {make, find, help};
    mode selected = mode::help;
    std::vector<string> input;
    string dict, out;
    bool split = false, progr = false;

    auto makeMode = (
        command("make").set(selected,mode::make),
        value("wordfile", input),
        required("-dict") & value("dictionary", dict),
        option("--progress", "-p").set(progr)           % "show progress" );

    auto findMode = (
        command("find").set(selected,mode::find),
        values("infile", input),
        required("-dict") & value("dictionary", dict),
        (option("-o", "--output") & value("outfile", out))  % "write to file instead of stdout",
        ( option("-split"  ).set(split,true) |
          option("-nosplit").set(split,false) )  % "(do not) split output" );

    auto cli = (
        (makeMode | findMode | command("help").set(selected,mode::help) ),
        option("-v", "--version").call([]{cout << "version 1.0\n\n"; }) % "show version" );

    string execname = argv[0];

    if(parse(argc, argv, cli)) {
        switch(selected) {
            case mode::make:
                cout << "make new dictionary " << dict << " from wordfile(s) ";
                for(const auto& s : input) { cout << s << ' '; } cout << '\n';
                break;
            case mode::find:
                cout << "find words from dictionary " <<  dict << " in files ";
                for(const auto& s : input) { cout << s << ' '; } cout << '\n';
                cout << "output: ";
                if(split) cout << "splitted ";
                cout << "to ";
                if(!out.empty()) cout << "file " << out; else cout << "stdin";
                cout << '\n';
                if(progr) cout << "Progres: [                      ] 0%\n";
                break;
            case mode::help:
                cout << make_man_page(cli, execname) << '\n';
                break;
        }
    } else {
        cout << usage_lines(cli, execname) << '\n';
    }
}
