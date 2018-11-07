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

    std::string outfile = "a.out";
    bool align = false;

    auto cli = (
        (option("-o", "--out") & value("output file", outfile)) % "output filename",
        with_prefix("-f", option("align")   >> [&]{ align = true; } |
                          option("noalign") >> [&]{ align = false; } ) % "control alignment"
    );

    if(parse(argc, argv, cli)) {
        cout << "write to " << outfile
             << " with" << (align ? "" : "out") << " alignment.\n";
    }
    else {
        auto fmt = doc_formatting{}
            .merge_alternative_flags_with_common_prefix(true);

        cout << make_man_page(cli, argv[0], fmt) << '\n';
    }
}
