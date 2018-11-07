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
#include <sstream>

// #include <clipp.h>
#include "../include/clipp.h"


int main(int argc, char* argv[])
{
    using namespace clipp;
    using std::cout;
    using std::string;
    using std::to_string;

    int firstCol = 0;
    int docCol = 30;
    int lastCol = 80;
    auto fmtcli = (
        value("first column", firstCol),
        value("docstr column", docCol),
        value("last column", lastCol)
    );

    if(!parse(argc, argv, fmtcli)) {
        std::cout
        << "This program shows how clipp handles documentation formatting.\n"
        << "You can use the following command line interface to re-format\n"
        << "the documentation text of a more complex command line interface\n"
        << "Usage:\n" << usage_lines(fmtcli, argv[0]) << '\n';
        return 0;
    }
    if(docCol < firstCol) docCol = firstCol;
    if(lastCol <= docCol) lastCol = docCol+1;

    std::cout
        << "first column  (>) " << firstCol << '\n'
        << "docstr column (|) " << docCol << '\n'
        << "last column   (<) " << lastCol << '\n'
        << std::string(firstCol, ' ') << ">"
        << std::string(docCol-firstCol-1, ' ') << "|"
        << std::string(lastCol-docCol-1, ' ') << "<\n";

    auto copyMode = "copy mode:" % (
        command("copy") | command("move"),
        option("--all")         % "copy all",
        option("--replace")     % "replace existing files",
        option("-f", "--force") % "don't ask for confirmation"
    );

    auto compareMode = "compare mode:" % (
        command("compare"),
        (command("date") | command("content")),
        option("-b", "--binary") % "compare files byte by byte",
        option("-q", "--quick")  % "use heuristics for faster comparison"
    );

    auto mergeAlgo = (
        command("diff")  % "merge using diff"  |
        command("patch") % "merge using patch" |
        (   command("content") % "merge based on content",
            "content based merge options:" % (
              option("--git-style") % "emulate git's merge behavior",
              option("-m", "--marker") & value("marker") % "merge marker symbol"
            )
        )
    );

    auto mergeMode = "merge mode:" % (
        command("merge"),
        mergeAlgo,
        required("-o") & value("outdir") % "target directory for merge result",
        option("--show-conflicts")       % "show merge conflicts during run"
    );

    auto firstOpt = "user interface options:" % (
        option("-v", "--verbose")     % "show detailed output",
        option("-i", "--interactive") % "use interactive mode"
    );
    auto lastOpt = "mode-independent options:" % (
        values("files")             % "input files",
        option("-r", "--recursive") % "descend into subdirectories",
        option("-h", "--help")      % "show help"
    );

    auto cli = (
        firstOpt,
        copyMode | compareMode | mergeMode | command("list"),
        lastOpt
    );

    parse(argc, argv, fmtcli);

    auto fmt = doc_formatting{}
        .line_spacing(0)
        .paragraph_spacing(1)
        .first_column(firstCol)
        .doc_column(docCol)
        .last_column(lastCol);

    cout << make_man_page(cli, argv[0], fmt) << '\n';
//    cout << documentation(cli, fmt) << '\n';
//    cout << usage_lines(cli, argv[0], fmt) << '\n';
}
