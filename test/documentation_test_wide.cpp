/*****************************************************************************
 *
 * CLIPP - wcommand line interfaces for modern C++
 *
 * released under MIT license
 *
 * (c) 2017-2018 André Müller; foss@andremueller-online.de
 *
 *****************************************************************************/

#include "testing.h"

using namespace clipp;

namespace {

//-------------------------------------------------------------------
void test(int lineNo, const wdoc_formatting& fmt,
          const wgroup& cli, const std::wstring& expected)
{
    auto const doc{wdocumentation(cli,fmt).str()};

    if(doc != expected) {
        std::wcout << L"\nEXPECTED:\n" << expected << L"\nOBSERVED:\n" << doc << '\n';

        test_location const loc{std::string{__FILE__}, lineNo};
        loc.debug_out("FAILED");

        throw std::runtime_error{"failed " + loc.file + 
                                 " in line " + std::to_string(loc.line)};
    }
}

} // namespace

//-------------------------------------------------------------------
int TEST_MAIN()
{
    try {

    auto fmt = wdoc_formatting{}
        .first_column(3)
        .doc_column(45)
        .last_column(80)
        .empty_label(L"?")
        .param_separator(L"%")
        .group_separator(L"#")
        .alternative_param_separator(L"|")
        .alternative_group_separator(L"^")
        .flag_separator(L"~")
        .surround_labels(L"<", L">")
        .surround_optional(L"[", L"]")
        .surround_repeat(L",,,", L"...")
        .surround_alternatives(L"§(§L", L"§)§")
        .surround_alternative_flags(L"^(^", L"^)^")
        .surround_group(L"(", L")")
        .surround_joinable(L"$($", L"$)$")
        .max_flags_per_param_in_doc(2)
        .max_flags_per_param_in_usage(1)
        .line_spacing(1)
        .paragraph_spacing(3)
        .alternatives_min_split_size(3)
        .merge_alternative_flags_with_common_prefix(false)
        .merge_joinable_with_common_prefix(true)
        .split_alternatives(true);


    test(__LINE__, fmt, (
            (woption(L"-o", L"--out") & wvalue(L"output file")) % L"output filename",
            wwith_prefix(L"-f", woption(L"align") >> []{} | woption(L"noalign") >> []{} ) % L"control alignment"
        ),
         L"   ^(^-o~--out^)^%<output file>              output filename\n"
         L"\n"
         L"   -falign|-fnoalign                         control alignment");


    test(__LINE__, fmt, (
            (woption(L"-n", L"--count") & wvalue(L"count")) % L"number of iterations",
            (woption(L"-r", L"--ratio") & wvalue(L"ratio")) % L"compression ratio",
            (woption(L"-m") & wopt_value(L"lines=5")) % L"merge lines (default: 5)"
        ),
         L"   ^(^-n~--count^)^%<count>                  number of iterations\n"
         L"\n"
         L"   ^(^-r~--ratio^)^%<ratio>                  compression ratio\n"
         L"\n"
         L"   -m%<lines=5>                              merge lines (default: 5)");


    test(__LINE__, fmt, (
            L"input files" % wvalues(L"file"),
            L"compress results" % woption(L"-c", L"--compress"),
            L"lines to be ignored" % wrepeatable( woption(L"-i", L"--ignore") & wintegers(L"line") )
        ),
         L"   ,,,<file>...                              input files\n"
         L"\n"
         L"   -c~--compress                             compress results\n"
         L"\n"
         L"   ^(^-i~--ignore^)^%<line>                  lines to be ignored");


    test(__LINE__, fmt, (
            woption(L"-r", L"--recursive") % L"recurse into subdirectories",
            (wrequired(L"-i", L"--in" ) & wvalue(L"input dir")) % L"sets path to input directory",
            (wrequired(L"-o", L"--out") & wvalue(L"output dir")) % L"sets path to output directory"
        ),
         L"   -r~--recursive                            recurse into subdirectories\n"
         L"\n"
         L"   ^(^-i~--in^)^%<input dir>                 sets path to input directory\n"
         L"\n"
         L"   ^(^-o~--out^)^%<output dir>               sets path to output directory");


    test(__LINE__, fmt, (
            wvalue(L"infile") % L"input filename",
            wvalue(L"outfile") % L"output filename",
            woption(L"-s", L"--split") % L"split files"
        ),
         L"   <infile>                                  input filename\n"
         L"\n"
         L"   <outfile>                                 output filename\n"
         L"\n"
         L"   -s~--split                                split files");


    test(__LINE__, fmt, (
            woption(L"-a") % L"activates a",
            woption(L"-b") % L"activates b",
            woption(L"-c", L"--noc") % L"deactivates c",
            woption(L"--hi")([]{}) % L"says hi"
        ),
         L"   -a                                        activates a\n"
         L"\n"
         L"   -b                                        activates b\n"
         L"\n"
         L"   -c~--noc                                  deactivates c\n"
         L"\n"
         L"   --hi                                      says hi");


    test(__LINE__, fmt, (
            wcommand(L"make"),
            wvalue(L"file") % L"name of file to make",
            woption(L"-f", L"--force") % L"overwrite existing file"
        ),
         L"   <file>                                    name of file to make\n"
         L"\n"
         L"   -f~--force                                overwrite existing file");


    test(__LINE__, fmt, (
            woption(L"-a") % L"activates a",
            woption(L"-b") % L"activates b",
            woption(L"-c", L"--noc")   % L"deactivates c",
            woption(L"--hi")([]{}) % L"says hi"
        ),
         L"   -a                                        activates a\n"
         L"\n"
         L"   -b                                        activates b\n"
         L"\n"
         L"   -c~--noc                                  deactivates c\n"
         L"\n"
         L"   --hi                                      says hi");


    test(__LINE__, fmt, (
            ( wcommand(L"ship"), ( ( wcommand(L"new"), wvalues(L"name") ) |
                ( wvalue(L"name"),
                    wcommand(L"move"), wvalue(L"x"), wvalue(L"y"), (woption(L"--speed=") & wvalue(L"kn")) % L"Speed in knots [default: 10]") |
                    ( wcommand(L"shoot"), wvalue(L"x"), wvalue(L"y") ) ) )
                    | ( wcommand(L"mine"),
                        (   wcommand(L"set"   )
                            | wcommand(L"remove") ),
                            wvalue(L"x"), wvalue(L"y"),
                            (   woption(L"--moored"  ) % L"Moored (anchored) mine."
                                | woption(L"--drifting") % L"Drifting mine.")
                    )
                    | wcommand(L"-h", L"--help")  % L"Show this screen."
                    | wcommand(L"--version")([]{}) % L"Show version."
        ),
         L"   --speed=%<kn>                             Speed in knots [default: 10]\n"
         L"\n"
         L"   --moored                                  Moored (anchored) mine.\n"
         L"\n"
         L"   --drifting                                Drifting mine.\n"
         L"\n"
         L"   -h~--help                                 Show this screen.\n"
         L"\n"
         L"   --version                                 Show version.");


    test(__LINE__, fmt, ( wcommand(L"help")
               | ( wcommand(L"build"),
                   ( wcommand(L"new") | wcommand(L"add") ),
                   wvalues(L"file"),
                   woption(L"-v", L"--verbose") % L"print detailed report",
                   woption(L"-b", L"--buffer") & wopt_value(
                       L"size=10")  % L"sets buffer size in KiByte",
                       ( woption(L"--init") | woption(L"--no-init") ) % L"do or don't initialize"
               )
               | ( wcommand(L"query"), wvalue(L"infile"),
                   wrequired(L"-o", L"--out") & wvalue(L"outfile"),
                   woption(L"-f", L"--out-format") % L"determine output format"
                   & wvalue(L"format")
               )
        ),
         L"   -v~--verbose                              print detailed report\n"
         L"\n"
         L"   <size=10>                                 sets buffer size in KiByte\n"
         L"\n"
         L"   --init|--no-init                          do or don't initialize\n"
         L"\n"
         L"   -f~--out-format                           determine output format");


    test(__LINE__, fmt, (
            wvalue(L"file"),
            wjoinable(
                woption(L"-r") % L"open read-only",
                woption(L"-b") % L"use backup file",
                woption(L"-s") % L"use swap file"
            ),
            wjoinable(
                woption(L":vim") >> []{}, woption(L":st3") >> []{},
                woption(L":atom") >> []{}, woption(L":emacs") >> []{}
            ) % L"editor(s) to use; multiple possible"
        ),
         L"   -r                                        open read-only\n"
         L"\n"
         L"   -b                                        use backup file\n"
         L"\n"
         L"   -s                                        use swap file\n"
         L"\n"
         L"   :vim%:st3%:atom%:emacs                    editor(s) to use; multiple possible");


    test(__LINE__, fmt, (
            (woption(L"x") % L"sets X", woption(L"y") % L"sets Y"),
            (woption(L"a") % L"activates A", woption(L"b") % L"activates B") % L"documented group 1:",
            L"documented group 2:" % (woption(L"-g") % L"activates G", woption(L"-h") % L"activates H"),
            L"activates E or F" % (woption(L"-e"), woption(L"-f"))
        ),
         L"   x                                         sets X\n"
         L"\n"
         L"   y                                         sets Y\n"
         L"\n"
         L"\n"
         L"\n"
         L"   documented group 1:\n\n"
         L"       a                                     activates A\n"
         L"\n"
         L"       b                                     activates B\n"
         L"\n"
         L"\n"
         L"\n"
         L"   documented group 2:\n\n"
         L"       -g                                    activates G\n"
         L"\n"
         L"       -h                                    activates H\n"
         L"\n"
         L"\n"
         L"\n"
         L"   -e%-f                                     activates E or F");


    test(__LINE__, fmt, ( ( wcommand(L"make"), wvalue(L"wordfile"), wrequired(L"-dict") & wvalue(L"dictionary"), woption(L"--progress", L"-p")) | (
            wcommand(L"find"), wvalues(L"infile"), wrequired(L"-dict") & wvalue(L"dictionary"),
            (woption(L"-o", L"--output") & wvalue(L"outfile"))  % L"write to file instead of stdout",
            ( woption(L"-split"  ) | woption(L"-nosplit"))) | wcommand(L"help"),
            woption(L"-v", L"--version").call([]{}) % L"show version"
        ),
         L"   ^(^-o~--output^)^%<outfile>               write to file instead of stdout\n"
         L"\n"
         L"   -v~--version                              show version");


    test(__LINE__, fmt, (
            wcommand(L"help") |
            ( wcommand(L"build"),
                L"build commands" %
                (   wcommand(L"new") % L"make new database"
                    | wcommand(L"add") % L"append to existing database"
                ),
                wvalue(L"file")
            ) |
            ( wcommand(L"query"),
                L"query settings" %
                (   wrequired(L"-i", L"--input") & wvalue(L"infile") % L"input file",
                    woption(L"-p", L"--pretty-print") % L"human friendly output")
            ) |
            ( wcommand(L"info"),
                L"database info modes" % (
                    wcommand(L"space") % L"detailed memory occupation analysis" |
                    (
                        wcommand(L"statistics"),
                        L"statistics analysis" % (
                            wcommand(L"words") % L"word frequency table" |
                            wcommand(L"chars") % L"character frequency table"
                        )
                    )
                )
            ) |
            L"remove mode" % (
                wcommand(L"remove"),
                L"modify" % ( wcommand(L"any") | wcommand(L"all") ),
                wvalue(L"regex") % L"regular expression filter"
            ) |
            ( wcommand(L"modify"),
                L"modification opererations" % (
                    woption(L"-c", L"--compress") % L"compress database in-memory",
                    woption(L"-u", L"--unique") % L"keep only unique entries",
                    woption(L"-m", L"--memlimit") % L"max. size in RAM" & wvalue(L"size")
                )
            )
        ),
         L"   build commands\n\n"
         L"       new                                   make new database\n"
         L"\n"
         L"       add                                   append to existing database\n"
         L"\n"
         L"\n"
         L"\n"
         L"   query settings\n\n"
         L"       <infile>                              input file\n"
         L"\n"
         L"       -p~--pretty-print                     human friendly output\n"
         L"\n"
         L"\n"
         L"\n"
         L"   database info modes\n\n"
         L"       space                                 detailed memory occupation analysis\n"
         L"\n"
         L"\n"
         L"\n"
         L"       statistics analysis\n\n"
         L"           words                             word frequency table\n"
         L"\n"
         L"           chars                             character frequency table\n"
         L"\n"
         L"\n"
         L"\n"
         L"   remove mode\n\n"
         L"       any|all                               modify\n"
         L"\n"
         L"       <regex>                               regular expression filter\n"
         L"\n"
         L"\n"
         L"\n"
         L"   modification opererations\n\n"
         L"       -c~--compress                         compress database in-memory\n"
         L"\n"
         L"       -u~--unique                           keep only unique entries\n"
         L"\n"
         L"       -m~--memlimit                         max. size in RAM");


    test(__LINE__, fmt, (
            wcommand(L"new"),
            wvalue(L"filename"),
            (woption(L"-e", L"--encoding") & wvalue(L"enc")).doc(L"'utf8' or 'cp1251'")
        ),
        L"   ^(^-e~--encoding^)^%<enc>                 'utf8' or 'cp1251'");


    test(__LINE__, fmt, (
            wvalues(L"file") % L"input filenames",
            (wrequired(L"-s") & wvalue(L"expr")) % L"string to look for",
            woption(L"any") % L"report as soon as any matches" |
            woption(L"all") % L"report only if all match"
        ),
        L"   ,,,<file>...                              input filenames\n"
        L"\n"
        L"   -s%<expr>                                 string to look for\n"
        L"\n"
        L"   any                                       report as soon as any matches\n"
        L"\n"
        L"   all                                       report only if all match");


    auto const complexcli = (
        L"user interface options:" % (
            woption(L"-v", L"--verbose") % L"show detailed output",
            woption(L"-i", L"--interactive") % L"use interactive mode"
        ),
        L"copy mode:" % (
            wcommand(L"copy") | wcommand(L"move"),
            woption(L"--all") % L"copy all",
            woption(L"--replace") % L"replace existing files",
            woption(L"-f", L"--force") % L"don't ask for confirmation"
        ) |
        L"compare mode:" % (
            wcommand(L"compare"),
            (wcommand(L"date") | wcommand(L"content")),
            woption(L"-b", L"--binary") % L"compare files byte by byte",
            woption(L"-q", L"--quick") % L"use heuristics for faster comparison"
        ) |
        L"merge mode:" % (
            wcommand(L"merge"),
            (
                wcommand(L"diff") % L"merge using diff"  |
                wcommand(L"patch") % L"merge using patch" |
                (   wcommand(L"content") % L"merge based on content",
                    L"content based merge options:" % (
                        woption(L"--git-style") % L"emulate git's merge behavior",
                        woption(L"-mL", L"--marker") & wvalue(L"marker") % L"merge marker symbol"
                    )
                )
            ),
            wrequired(L"-o") & wvalue(L"outdir") % L"target directory for merge result",
            woption(L"--show-conflicts") % L"show merge conflicts during run"
        ) |
        wcommand(L"list"),
        L"mode-independent options:" % (
            wvalues(L"files") % L"input files",
            woption(L"-r", L"--recursive") % L"descend into subdirectories",
            woption(L"-h", L"--help") % L"show help"
        )
    );


    test(__LINE__, fmt, complexcli,
         L"   user interface options:\n\n"
         L"       -v~--verbose                          show detailed output\n"
         L"\n"
         L"       -i~--interactive                      use interactive mode\n"
         L"\n"
         L"\n"
         L"\n"
         L"   copy mode:\n\n"
         L"       --all                                 copy all\n"
         L"\n"
         L"       --replace                             replace existing files\n"
         L"\n"
         L"       -f~--force                            don't ask for confirmation\n"
         L"\n"
         L"\n"
         L"\n"
         L"   compare mode:\n\n"
         L"       -b~--binary                           compare files byte by byte\n"
         L"\n"
         L"       -q~--quick                            use heuristics for faster\n"
         L"                                             comparison\n"
         L"\n"
         L"\n"
         L"\n"
         L"   merge mode:\n\n"
         L"       diff                                  merge using diff\n"
         L"\n"
         L"       patch                                 merge using patch\n"
         L"\n"
         L"       content                               merge based on content\n"
         L"\n"
         L"\n"
         L"\n"
         L"       content based merge options:\n\n"
         L"           --git-style                       emulate git's merge behavior\n"
         L"\n"
         L"           <marker>                          merge marker symbol\n"
         L"\n"
         L"\n"
         L"\n"
         L"       <outdir>                              target directory for merge result\n"
         L"\n"
         L"       --show-conflicts                      show merge conflicts during run\n"
         L"\n"
         L"\n"
         L"\n"
         L"   mode-independent options:\n\n"
         L"       ,,,<files>...                         input files\n"
         L"\n"
         L"       -r~--recursive                        descend into subdirectories\n"
         L"\n"
         L"       -h~--help                             show help");


    fmt.first_column(7) .doc_column(20) .last_column(50);

    test(__LINE__, fmt, complexcli,
        L"       user interface options:\n\n"
        L"           -v~--verbose\n"
        L"                    show detailed output\n\n\n\n"
        L"           -i~--interactive\n"
        L"                    use interactive mode\n\n\n\n"
        L"       copy mode:\n\n"
        L"           --all    copy all\n\n"
        L"           --replace\n"
        L"                    replace existing files\n\n\n\n"
        L"           -f~--force\n"
        L"                    don't ask for confirmation\n\n\n\n"
        L"       compare mode:\n\n"
        L"           -b~--binary\n"
        L"                    compare files byte by byte\n\n\n\n"
        L"           -q~--quick\n"
        L"                    use heuristics for faster\n"
        L"                    comparison\n\n\n\n"
        L"       merge mode:\n\n"
        L"           diff     merge using diff\n\n"
        L"           patch    merge using patch\n\n"
        L"           content  merge based on content\n\n\n\n"
        L"           content based merge options:\n\n"
        L"               --git-style\n"
        L"                    emulate git's merge behavior\n\n\n\n"
        L"               <marker>\n"
        L"                    merge marker symbol\n\n\n\n"
        L"           <outdir> target directory for merge\n"
        L"                    result\n\n\n\n"
        L"           --show-conflicts\n"
        L"                    show merge conflicts during\n"
        L"                    run\n\n\n\n"
        L"       mode-independent options:\n\n"
        L"           ,,,<files>...\n"
        L"                    input files\n\n\n\n"
        L"           -r~--recursive\n"
        L"                    descend into subdirectories\n\n\n\n"
        L"           -h~--help\n"
        L"                    show help");

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
