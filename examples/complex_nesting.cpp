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

    if(parse(argc, argv, cli)) {
        // program logic...
    } else {
        auto fmt = doc_formatting{}.doc_column(31).last_column(80);
        cout << make_man_page(cli, argv[0], fmt) << '\n';
    }
}
