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

    bool verbose = false;
    bool init = true;
    int bsize = 1024;
    enum class mode {help, build, query};
    bool add = false;
    enum class fmt  {plain, json, xml};
    mode selected;
    std::vector<std::string> infiles;
    std::string outfile;
    std::string outfmt;

    auto cli = (
        command("help")
        | (   command("build").set(selected,mode::help),
            (   command("new").set(selected,mode::build).set(add,false)
              | command("add").set(selected,mode::build).set(add,true) ),
            values("file", infiles),
            option("-v", "--verbose").set(verbose)     % "print detailed report",
            option("-b", "--buffer") & opt_value(
                "size="+std::to_string(bsize), bsize)  % "sets buffer size in KiByte",
            (   option("--init").set(init)
              | option("--no-init").set(init,false) )  % "do or don't initialize"
        )
        | ( command("query").set(selected,mode::query),
            value("infile", infiles),
            required("-o", "--out") & value("outfile", outfile),
            option("-f", "--out-format")               % "determine output format"
                 & value("format",outfmt)
        )
    );

    if(parse(argc, argv, cli)) {
        switch(selected) {
            default:
            case mode::help:
                cout << make_man_page(cli, argv[0]) << '\n'; return 0;
            case mode::build:
                if(add)
                    cout << "adding to database\n";
                else
                    cout << "building new database\n";

                cout << "buffer size is " << bsize << '\n';
                if(init) cout << "showing detailed information\n";
                break;
            case mode::query:
                cout << "query database\n";
                cout << "output to " << outfile << " in format " << outfmt << '\n';
                break;
        }
        cout << "input files:";
        for(const auto& f : infiles) cout << ' ' << f;
        cout << "\n";
        if(verbose) cout << "showing detailed information\n";
    }
    else {
        cout << usage_lines(cli, argv[0]) << '\n';
    }
}
