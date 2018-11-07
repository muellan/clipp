/*****************************************************************************
 *
 * CLIPP - command line interfaces for modern C++
 *
 * released under MIT license
 *
 * (c) 2017-2018 André Müller; foss@andremueller-online.de
 *
 *****************************************************************************/

#include "testing.h"


//-------------------------------------------------------------------
void test(int lineNo, bool expected, const clipp::group& cli)
{
    if(cli.flags_are_prefix_free() != expected) {

        std::cout << "prefix '" << cli.common_flag_prefix() << "'" << std::endl;

        throw std::runtime_error{"failed test " + std::string(__FILE__) +
            " in line " + std::to_string(lineNo)};
    }
}


//-------------------------------------------------------------------
clipp::group
make_cli_1()
{
    using namespace clipp;

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
        (   command("inside") % "merge based on content",
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

    return (
        firstOpt,
        copyMode | compareMode | mergeMode | command("list"),
        lastOpt);
}

//-------------------------------------------------------------------
int main()
{
    using namespace clipp;

    try {
        test( __LINE__ , true, group{} );

        test( __LINE__ , true, ( option("-a"), option("-b") ));

        test( __LINE__ , true, ( option(""), option("-b") ));
        test( __LINE__ , true, ( option("-a"), option("") ));

        test( __LINE__ , true, ( option("-a"), option("-b"), option("--a") ));
        test( __LINE__ , true, ( option(""), option("-b"), option("--a") ));
        test( __LINE__ , true, ( option("-a"), option(""), option("--a") ));
        test( __LINE__ , true, ( option("-a"), option("-b"), option("") ));

        test( __LINE__ , true, (
            option("-a"), option("--a"), option("-b"), option("--b")
        ));

        test( __LINE__ , true, (
            option("-a"), option("--a-a"), option("-b"), option("--b-b")
        ));

        test( __LINE__ , true, (
            (option("-a"), option("--a-a")) % "a",
            (option("-b"), option("--b-b")) % "b"
        ));

        test( __LINE__ , true, ( value("-a"), option("-a"), option("-b") ));
        test( __LINE__ , true, ( option("-a"), value("-a"), option("-b") ));
        test( __LINE__ , true, ( option("-a"), option("-b"), value("-b") ));

        test( __LINE__ , true, ( option("-a"), option("-b"), value("") ));

        test( __LINE__ , true, (
            option("-a"), option("--a"), option("-b"), value("-b"), option("--b")
        ));

        test( __LINE__ , true, (
            option("-a"), option("--a-a"), option("-b"), option("--b-b")
        ));

        test( __LINE__ , true, (
            (option("-a"), option("--a-a")) % "a",
            (option("-b"), option("--b-b")) % "b"
        ));

        //same flags are not an error - TODO is this OK?
        test( __LINE__ , true, (
            (option("-a"), option("-a")) % "a",
            (option("-b"), option("-b")) % "b"
        ));

        test( __LINE__ , true, (
            (option("-a") & value("-a"), option("--a-a")) % "a",
            (option("-b"), option("--b-b") & values("-a")) % "b"
        ));


        test( __LINE__ , false, ( option("-a"), option("-ab") ));

        test( __LINE__ , false, ( option("-ab"), option("-a") ));

        test( __LINE__ , false, (
            option("-a"), option("-aa"), option("-b"), option("--b-b")
        ));

        test( __LINE__ , false, (
            (option("-a"), option("--a")) % "a",
            (option("-b"), option("-abba")) % "b"
        ));

        test( __LINE__ , false, (
            (option("-a") & value("a"), option("--a")) % "a",
            (option("-b"), option("-abba") & values("-b")) % "b"
        ));

        test( __LINE__, true, make_cli_1() );

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
