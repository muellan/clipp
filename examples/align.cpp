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
#include <cmath>

#include <clipp.h>


int main(int argc, char* argv[])
{
    using namespace clipp;

    enum class imode { file, args, stdio, random };
    enum class omode { file, stdio };
    auto input = imode::file;
    auto output = omode::stdio;
    std::int64_t minlen = 256;
    std::int64_t maxlen = 1024;
    std::string query, subject;
    std::string outfile;
    std::vector<std::string> wrong;
 
    auto cli = (
        (option("-o", "--out").set(output,omode::file) & 
         value("file", outfile)) % "write results to file"
        ,
        "read sequences from input files" % (
            command("-i", "--in"),
            value("query file", query),
            value("subject file", subject)
        ) | 
        "specify sequences on the command line" % (
            command("-a", "--args").set(input,imode::args),
            value("query string", query),
            value("subject string", subject)
        ) | 
        "generate random input sequences" % (
            command("-r", "--rand").set(input,imode::random),
            opt_integer("min len", minlen) &
            opt_integer("max len", maxlen)
        ) | ( 
        "read sequences from stdin" %
            command("-").set(input,imode::stdio)
        ),
        any_other(wrong)
    );


    if(!parse(argc,argv, cli) || !wrong.empty()) {
        if(!wrong.empty()) {
            std::cout << "Unknown command line arguments:\n";
            for(const auto& a : wrong) std::cout << "'" << a << "'\n";
            std::cout << '\n';
        }
        std::cout << make_man_page(cli, argv[0]) << '\n';
        return 0;
    }

    switch(input) {
        default:
        case imode::file:   /* ... */    break;
        case imode::args:   /* ... */    break;
        case imode::stdio:  /* ... */    break;
        case imode::random: /* ... */    break;
    }

    switch(output) {
        default:
        case omode::stdio:  /* ... */    break;          
        case omode::file:   /* ... */    break;
    }
}
