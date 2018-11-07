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

    std::string ifile, ofile;
    bool split = false;

    auto cli = (
        value("infile", ifile)             % "input filename",
        value("outfile", ofile)            % "output filename",
        option("-s", "--split").set(split) % "split files" );

    if(parse(argc, argv, cli)) {
        cout << "input file:  " << ifile << '\n'
             << "output file: " << ofile << '\n'
             << "Files will "   << (split ? "" : "not") << " be split.";
    }
    else {
        cout << make_man_page(cli, argv[0]) << '\n';
    }

// alternative style:
//    auto cli = (
//        value("infile")         % "input filename"  >> ifile,
//        value("outfile")        % "output filename" >> ofile,
//        option("-s", "--split") % "split files"     >> split  );

}
