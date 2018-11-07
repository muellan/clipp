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

    std::vector<std::string> wrong;

    auto istarget = match::prefix_not("-");

    auto cli = (
        value("file")
            .if_missing([]{ cout << "You need to provide a source filename!\n"; } )
            .if_repeated([](int idx){ cout << "Only one source file allowed! (index " << idx << ")\n"; } )
        ,
        required("-t") & values(istarget, "target")
            .if_missing([]{ cout << "You need to provide at least one target filename!\n"; } )
            .if_blocked([]{ cout << "Target names must not be given before the file command and the source file name!\n"; })
        ,
        option("--http") | option("--ftp")
            .if_conflicted([]{ cout << "You can only use one protocol at a time!\n"; } )
        ,
        any_other(wrong)
    );

    if(parse(argc, argv, cli) && wrong.empty()) {
        cout << "OK\n";
    }
    else {
        for(const auto& arg : wrong) {
            cout << "'" << arg << "' is not a valid command line argument\n";
        }
        cout << "Usage:\n" << usage_lines(cli,argv[0]) << '\n';
    }
}
