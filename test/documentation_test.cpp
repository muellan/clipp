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

using namespace clipp;

//-------------------------------------------------------------------
void test(int lineNo, const doc_formatting& fmt,
          const group& cli, const std::string& expected)
{
    auto doc = documentation(cli,fmt).str();

//    std::cout << "\n====================================\n\n" << doc << "\n";
//    return;

    if(doc != expected) {
        std::cout << "\nEXPECTED:\n" << expected << "\nOBSERVED:\n" << doc << '\n';

        throw std::runtime_error{"failed " + std::string( __FILE__ ) +
                                 " in line " + std::to_string(lineNo)};
    }
}


//-------------------------------------------------------------------
int main()
{
    try {

    auto fmt = doc_formatting{}
        .first_column(3)
        .doc_column(45)
        .last_column(80)
        .empty_label("?")
        .param_separator("%")
        .group_separator("#")
        .alternative_param_separator("|")
        .alternative_group_separator("^")
        .flag_separator("~")
        .surround_labels("<", ">")
        .surround_optional("[", "]")
        .surround_repeat(",,,", "...")
        .surround_alternatives("§(§", "§)§")
        .surround_alternative_flags("^(^", "^)^")
        .surround_group("(", ")")
        .surround_joinable("$($", "$)$")
        .max_flags_per_param_in_doc(2)
        .max_flags_per_param_in_usage(1)
        .line_spacing(1)
        .paragraph_spacing(3)
        .alternatives_min_split_size(3)
        .merge_alternative_flags_with_common_prefix(false)
        .merge_joinable_with_common_prefix(true)
        .split_alternatives(true);


    test(__LINE__, fmt, (
            (option("-o", "--out") & value("output file")) % "output filename",
            with_prefix("-f", option("align") >> []{} | option("noalign") >> []{} ) % "control alignment"
        ),
         "   ^(^-o~--out^)^%<output file>              output filename\n"
         "\n"
         "   -falign|-fnoalign                         control alignment");


    test(__LINE__, fmt, (
            (option("-n", "--count") & value("count")) % "number of iterations",
            (option("-r", "--ratio") & value("ratio")) % "compression ratio",
            (option("-m") & opt_value("lines=5")) % "merge lines (default: 5)"
        ),
         "   ^(^-n~--count^)^%<count>                  number of iterations\n"
         "\n"
         "   ^(^-r~--ratio^)^%<ratio>                  compression ratio\n"
         "\n"
         "   -m%<lines=5>                              merge lines (default: 5)");


    test(__LINE__, fmt, (
            "input files" % values("file"),
            "compress results" % option("-c", "--compress"),
            "lines to be ignored" % repeatable( option("-i", "--ignore") & integers("line") )
        ),
         "   ,,,<file>...                              input files\n"
         "\n"
         "   -c~--compress                             compress results\n"
         "\n"
         "   ^(^-i~--ignore^)^%<line>                  lines to be ignored");


    test(__LINE__, fmt, (
            option("-r", "--recursive") % "recurse into subdirectories",
            (required("-i", "--in" ) & value("input dir")) % "sets path to input directory",
            (required("-o", "--out") & value("output dir")) % "sets path to output directory"
        ),
         "   -r~--recursive                            recurse into subdirectories\n"
         "\n"
         "   ^(^-i~--in^)^%<input dir>                 sets path to input directory\n"
         "\n"
         "   ^(^-o~--out^)^%<output dir>               sets path to output directory");


    test(__LINE__, fmt, (
            value("infile") % "input filename",
            value("outfile") % "output filename",
            option("-s", "--split") % "split files"
        ),
         "   <infile>                                  input filename\n"
         "\n"
         "   <outfile>                                 output filename\n"
         "\n"
         "   -s~--split                                split files");


    test(__LINE__, fmt, (
            option("-a") % "activates a",
            option("-b") % "activates b",
            option("-c", "--noc") % "deactivates c",
            option("--hi")([]{}) % "says hi"
        ),
         "   -a                                        activates a\n"
         "\n"
         "   -b                                        activates b\n"
         "\n"
         "   -c~--noc                                  deactivates c\n"
         "\n"
         "   --hi                                      says hi");


    test(__LINE__, fmt, (
            command("make"),
            value("file") % "name of file to make",
            option("-f", "--force") % "overwrite existing file"
        ),
         "   <file>                                    name of file to make\n"
         "\n"
         "   -f~--force                                overwrite existing file");


    test(__LINE__, fmt, (
            option("-a") % "activates a",
            option("-b") % "activates b",
            option("-c", "--noc")   % "deactivates c",
            option("--hi")([]{}) % "says hi"
        ),
         "   -a                                        activates a\n"
         "\n"
         "   -b                                        activates b\n"
         "\n"
         "   -c~--noc                                  deactivates c\n"
         "\n"
         "   --hi                                      says hi");


    test(__LINE__, fmt, (
            ( command("ship"), ( ( command("new"), values("name") ) |
                ( value("name"),
                    command("move"), value("x"), value("y"), (option("--speed=") & value("kn")) % "Speed in knots [default: 10]") |
                    ( command("shoot"), value("x"), value("y") ) ) )
                    | ( command("mine"),
                        (   command("set"   )
                            | command("remove") ),
                            value("x"), value("y"),
                            (   option("--moored"  ) % "Moored (anchored) mine."
                                | option("--drifting") % "Drifting mine.")
                    )
                    | command("-h", "--help")  % "Show this screen."
                    | command("--version")([]{}) % "Show version."
        ),
         "   --speed=%<kn>                             Speed in knots [default: 10]\n"
         "\n"
         "   --moored                                  Moored (anchored) mine.\n"
         "\n"
         "   --drifting                                Drifting mine.\n"
         "\n"
         "   -h~--help                                 Show this screen.\n"
         "\n"
         "   --version                                 Show version.");


    test(__LINE__, fmt, ( command("help")
               | ( command("build"),
                   ( command("new") | command("add") ),
                   values("file"),
                   option("-v", "--verbose") % "print detailed report",
                   option("-b", "--buffer") & opt_value(
                       "size=10")  % "sets buffer size in KiByte",
                       ( option("--init") | option("--no-init") ) % "do or don't initialize"
               )
               | ( command("query"), value("infile"),
                   required("-o", "--out") & value("outfile"),
                   option("-f", "--out-format") % "determine output format"
                   & value("format")
               )
        ),
         "   -v~--verbose                              print detailed report\n"
         "\n"
         "   <size=10>                                 sets buffer size in KiByte\n"
         "\n"
         "   --init|--no-init                          do or don't initialize\n"
         "\n"
         "   -f~--out-format                           determine output format");


    test(__LINE__, fmt, (
            value("file"),
            joinable(
                option("-r") % "open read-only",
                option("-b") % "use backup file",
                option("-s") % "use swap file"
            ),
            joinable(
                option(":vim") >> []{}, option(":st3") >> []{},
                option(":atom") >> []{}, option(":emacs") >> []{}
            ) % "editor(s) to use; multiple possible"
        ),
         "   -r                                        open read-only\n"
         "\n"
         "   -b                                        use backup file\n"
         "\n"
         "   -s                                        use swap file\n"
         "\n"
         "   :vim%:st3%:atom%:emacs                    editor(s) to use; multiple possible");


    test(__LINE__, fmt, (
            (option("x") % "sets X", option("y") % "sets Y"),
            (option("a") % "activates A", option("b") % "activates B") % "documented group 1:"       ,
            "documented group 2:" % (option("-g") % "activates G", option("-h") % "activates H"),
            "activates E or F" % (option("-e"), option("-f"))
        ),
         "   x                                         sets X\n"
         "\n"
         "   y                                         sets Y\n"
         "\n"
         "\n"
         "\n"
         "   documented group 1:\n\n"
         "       a                                     activates A\n"
         "\n"
         "       b                                     activates B\n"
         "\n"
         "\n"
         "\n"
         "   documented group 2:\n\n"
         "       -g                                    activates G\n"
         "\n"
         "       -h                                    activates H\n"
         "\n"
         "\n"
         "\n"
         "   -e%-f                                     activates E or F");


    test(__LINE__, fmt, ( ( command("make"), value("wordfile"), required("-dict") & value("dictionary"), option("--progress", "-p")) | (
            command("find"), values("infile"), required("-dict") & value("dictionary"),
            (option("-o", "--output") & value("outfile"))  % "write to file instead of stdout",
            ( option("-split"  ) | option("-nosplit"))) | command("help"),
            option("-v", "--version").call([]{}) % "show version"
        ),
         "   ^(^-o~--output^)^%<outfile>               write to file instead of stdout\n"
         "\n"
         "   -v~--version                              show version");


    test(__LINE__, fmt, (
            command("help") |
            ( command("build"),
                "build commands" %
                (   command("new") % "make new database"
                    | command("add") % "append to existing database"
                ),
                value("file")
            ) |
            ( command("query"),
                "query settings" %
                (   required("-i", "--input") & value("infile") % "input file",
                    option("-p", "--pretty-print") % "human friendly output")
            ) |
            ( command("info"),
                "database info modes" % (
                    command("space") % "detailed memory occupation analysis" |
                    (
                        command("statistics"),
                        "statistics analysis" % (
                            command("words") % "word frequency table" |
                            command("chars") % "character frequency table"
                        )
                    )
                )
            ) |
            "remove mode" % (
                command("remove"),
                "modify" % ( command("any") | command("all") ),
                value("regex") % "regular expression filter"
            ) |
            ( command("modify"),
                "modification opererations" % (
                    option("-c", "--compress") % "compress database in-memory",
                    option("-u", "--unique") % "keep only unique entries",
                    option("-m", "--memlimit") % "max. size in RAM" & value("size")
                )
            )
        ),
         "   build commands\n\n"
         "       new                                   make new database\n"
         "\n"
         "       add                                   append to existing database\n"
         "\n"
         "\n"
         "\n"
         "   query settings\n\n"
         "       <infile>                              input file\n"
         "\n"
         "       -p~--pretty-print                     human friendly output\n"
         "\n"
         "\n"
         "\n"
         "   database info modes\n\n"
         "       space                                 detailed memory occupation analysis\n"
         "\n"
         "\n"
         "\n"
         "       statistics analysis\n\n"
         "           words                             word frequency table\n"
         "\n"
         "           chars                             character frequency table\n"
         "\n"
         "\n"
         "\n"
         "   remove mode\n\n"
         "       any|all                               modify\n"
         "\n"
         "       <regex>                               regular expression filter\n"
         "\n"
         "\n"
         "\n"
         "   modification opererations\n\n"
         "       -c~--compress                         compress database in-memory\n"
         "\n"
         "       -u~--unique                           keep only unique entries\n"
         "\n"
         "       -m~--memlimit                         max. size in RAM");


    test(__LINE__, fmt, (
            command("new"),
            value("filename"),
            (option("-e", "--encoding") & value("enc")).doc("'utf8' or 'cp1252'")
        ),
        "   ^(^-e~--encoding^)^%<enc>                 'utf8' or 'cp1252'");


    test(__LINE__, fmt, (
            values("file") % "input filenames",
            (required("-s") & value("expr")) % "string to look for",
            option("any") % "report as soon as any matches" |
            option("all") % "report only if all match"
        ),
        "   ,,,<file>...                              input filenames\n"
        "\n"
        "   -s%<expr>                                 string to look for\n"
        "\n"
        "   any                                       report as soon as any matches\n"
        "\n"
        "   all                                       report only if all match");


    auto complexcli = (
        "user interface options:" % (
            option("-v", "--verbose") % "show detailed output",
            option("-i", "--interactive") % "use interactive mode"
        ),
        "copy mode:" % (
            command("copy") | command("move"),
            option("--all") % "copy all",
            option("--replace") % "replace existing files",
            option("-f", "--force") % "don't ask for confirmation"
        ) |
        "compare mode:" % (
            command("compare"),
            (command("date") | command("content")),
            option("-b", "--binary") % "compare files byte by byte",
            option("-q", "--quick") % "use heuristics for faster comparison"
        ) |
        "merge mode:" % (
            command("merge"),
            (
                command("diff") % "merge using diff"  |
                command("patch") % "merge using patch" |
                (   command("content") % "merge based on content",
                    "content based merge options:" % (
                        option("--git-style") % "emulate git's merge behavior",
                        option("-m", "--marker") & value("marker") % "merge marker symbol"
                    )
                )
            ),
            required("-o") & value("outdir") % "target directory for merge result",
            option("--show-conflicts") % "show merge conflicts during run"
        ) |
        command("list"),
        "mode-independent options:" % (
            values("files") % "input files",
            option("-r", "--recursive") % "descend into subdirectories",
            option("-h", "--help") % "show help"
        )
    );


    test(__LINE__, fmt, complexcli,
         "   user interface options:\n\n"
         "       -v~--verbose                          show detailed output\n"
         "\n"
         "       -i~--interactive                      use interactive mode\n"
         "\n"
         "\n"
         "\n"
         "   copy mode:\n\n"
         "       --all                                 copy all\n"
         "\n"
         "       --replace                             replace existing files\n"
         "\n"
         "       -f~--force                            don't ask for confirmation\n"
         "\n"
         "\n"
         "\n"
         "   compare mode:\n\n"
         "       -b~--binary                           compare files byte by byte\n"
         "\n"
         "       -q~--quick                            use heuristics for faster\n"
         "                                             comparison\n"
         "\n"
         "\n"
         "\n"
         "   merge mode:\n\n"
         "       diff                                  merge using diff\n"
         "\n"
         "       patch                                 merge using patch\n"
         "\n"
         "       content                               merge based on content\n"
         "\n"
         "\n"
         "\n"
         "       content based merge options:\n\n"
         "           --git-style                       emulate git's merge behavior\n"
         "\n"
         "           <marker>                          merge marker symbol\n"
         "\n"
         "\n"
         "\n"
         "       <outdir>                              target directory for merge result\n"
         "\n"
         "       --show-conflicts                      show merge conflicts during run\n"
         "\n"
         "\n"
         "\n"
         "   mode-independent options:\n\n"
         "       ,,,<files>...                         input files\n"
         "\n"
         "       -r~--recursive                        descend into subdirectories\n"
         "\n"
         "       -h~--help                             show help");


    fmt.first_column(7) .doc_column(20) .last_column(50);

    test(__LINE__, fmt, complexcli,
        "       user interface options:\n\n"
        "           -v~--verbose\n"
        "                    show detailed output\n\n\n\n"
        "           -i~--interactive\n"
        "                    use interactive mode\n\n\n\n"
        "       copy mode:\n\n"
        "           --all    copy all\n\n"
        "           --replace\n"
        "                    replace existing files\n\n\n\n"
        "           -f~--force\n"
        "                    don't ask for confirmation\n\n\n\n"
        "       compare mode:\n\n"
        "           -b~--binary\n"
        "                    compare files byte by byte\n\n\n\n"
        "           -q~--quick\n"
        "                    use heuristics for faster\n"
        "                    comparison\n\n\n\n"
        "       merge mode:\n\n"
        "           diff     merge using diff\n\n"
        "           patch    merge using patch\n\n"
        "           content  merge based on content\n\n\n\n"
        "           content based merge options:\n\n"
        "               --git-style\n"
        "                    emulate git's merge behavior\n\n\n\n"
        "               <marker>\n"
        "                    merge marker symbol\n\n\n\n"
        "           <outdir> target directory for merge\n"
        "                    result\n\n\n\n"
        "           --show-conflicts\n"
        "                    show merge conflicts during\n"
        "                    run\n\n\n\n"
        "       mode-independent options:\n\n"
        "           ,,,<files>...\n"
        "                    input files\n\n\n\n"
        "           -r~--recursive\n"
        "                    descend into subdirectories\n\n\n\n"
        "           -h~--help\n"
        "                    show help");

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
