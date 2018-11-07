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
    auto us = usage_lines(cli,fmt).str();

//    std::cout << "\n======================================\n\n" << us << "\n";
//    return;

    if(us != expected) {
        std::cout << "\nEXPECTED:\n" << expected << "\nOBSERVED:\n" << us << '\n';

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
        .doc_column(9)
        .last_column(80)
        .empty_label("?")
        .param_separator("%")
        .group_separator("#")
        .alternative_param_separator("|")
        .alternative_group_separator("^")
        .flag_separator("_")
        .surround_labels("<", ">")
        .surround_optional("[", "]")
        .surround_repeat(",,,", "...")
        .surround_alternatives("§(§", "§)§")
        .surround_group("(", ")")
        .surround_joinable("$($", "$)$")
        .max_flags_per_param_in_doc(2)
        .max_flags_per_param_in_usage(1)
        .line_spacing(0)
        .paragraph_spacing(3)
        .alternatives_min_split_size(3)
        .merge_alternative_flags_with_common_prefix(true)
        .merge_joinable_with_common_prefix(true)
        .split_alternatives(true);

    test(__LINE__, fmt, group( option("-a") ), "   [-a]");
    test(__LINE__, fmt, group( option("-a", "--all") ), "   [-a]");
    test(__LINE__, fmt, group( option("-a").repeatable(true) ), "   ,,,[-a]...");

    test(__LINE__, fmt, group( required("-a") ), "   -a");
    test(__LINE__, fmt, group( required("-a", "--all") ), "   -a");
    test(__LINE__, fmt, group( required("-a").repeatable(true) ), "   ,,,-a...");

    test(__LINE__, fmt, group( command("-a") ), "   -a");
    test(__LINE__, fmt, group( command("-a", "--all") ), "   -a");
    test(__LINE__, fmt, group( command("-a").repeatable(true) ), "   ,,,-a...");


    test(__LINE__, fmt, (
            value("geometry file"),
            option("-translate") & value("x") & value("y") & value("z"),
            option("-rotate") & value("azimuth") & value("polar")),
         "   <geometry file>#[-translate%<x>%<y>%<z>]#[-rotate%<azimuth>%<polar>]");


    test(__LINE__, fmt, joinable( values("string")), "   ,,,<string>...");


    test(__LINE__, fmt, (
            (option("-o", "--out") & value("output file")) % "output filename",
            with_prefix("-f", option("align") >> []{} | option("noalign") >> []{} ) % "control alignment"),
         "   [-o%<output file>]#[-f§(§align|noalign§)§]");


    test(__LINE__, fmt, (
            (option("-n", "--count") & value("count")) % "number of iterations",
            (option("-r", "--ratio") & value("ratio")) % "compression ratio",
            (option("-m") & opt_value("lines=5")) % "merge lines (default: 5)"),
         "   [-n%<count>]#[-r%<ratio>]#[-m%[<lines=5>]]");


    test(__LINE__, fmt, (
            "input files" % values("file"),
            "compress results" % option("-c", "--compress"),
            "lines to be ignored" % repeatable( option("-i", "--ignore") & integers("line") )),
         "   ,,,<file>...#[-c]#,,,[-i%,,,<line>...]...");


    test(__LINE__, fmt, (
            value("file").if_missing([]{}).if_repeated([](int){}),
            required("-t") & values("target").if_missing([]{}).if_blocked([]{}),
            option("--http") | option("--ftp").if_conflicted([]{}),
            any_other()),
         "   <file>#-t%,,,<target>...#[--§(§http|ftp§)§]#,,,[<?>]...");


    test(__LINE__, fmt, (
            option("-r", "--recursive") % "recurse into subdirectories",
            (required("-i", "--in" ) & value("input dir")) % "sets path to input directory",
            (required("-o", "--out") & value("output dir")) % "sets path to output directory"),
         "   [-r]#-i%<input dir>#-o%<output dir>");


    test(__LINE__, fmt, (
            values("directory") % "input files",
            option("-c", "--compress") % "compress results",
            (option("-i", "--ignore") & values("line")) % "lines to be ignored"),
         "   ,,,<directory>...#[-c]#[-i%,,,<line>...]");


    test(__LINE__, fmt, (
            value("infile") % "input filename",
            value("outfile") % "output filename",
            option("-s", "--split") % "split files" ),
         "   <infile>%<outfile>%[-s]");


    test(__LINE__, fmt, (
            option("-a") % "activates a",
            option("-b") % "activates b",
            option("-c", "--noc") % "deactivates c",
            option("--hi")([]{}) % "says hi"),
         "   [-a]%[-b]%[-c]%[--hi]");


    test(__LINE__, fmt, (
            command("make"),
            value("file") % "name of file to make",
            option("-f", "--force") % "overwrite existing file"),
         "   make%<file>%[-f]");


    test(__LINE__, fmt, (
            option("-a") % "activates a",
            option("-b") % "activates b",
            option("-c", "--noc")   % "deactivates c",
            option("--hi")([]{}) % "says hi"),
         "   [-a]%[-b]%[-c]%[--hi]");


    test(__LINE__, fmt,
         joinable(repeatable( option(",") , opt_number("number"))),
         "   ,,,([,]%[<number>])...");


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
                    | command("--version")([]{}) % "Show version."),
         "   ship#new%,,,<name>...\n"
         "   ship#<name>#move#<x>#<y>#[--speed=%<kn>]\n"
         "   ship#shoot%<x>%<y>\n"
         "   mine#§(§set|remove§)§#<x>#<y>#[--§(§moored|drifting§)§]\n"
         "   -h\n"
         "   --version");


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
               )),
       "   help\n"
       "   build#§(§new|add§)§#,,,<file>...#[-v]#[-b%[<size=10>]]#[--§(§init|no-init§)§]\n"
       "   query#<infile>#-o%<outfile>#[-f%<format>]");


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
            ) % "editor(s) to use; multiple possible"),
         "   <file>#[-rbs]#$($[:vim]%[:st3]%[:atom]%[:emacs]$)$");


    test(__LINE__, fmt, (
            (option("x") % "sets X", option("y") % "sets Y"),
            (option("a") % "activates A", option("b") % "activates B") % "documented group 1:"       ,
            "documented group 2:" % (option("-g") % "activates G", option("-h") % "activates H"),
            "activates E or F" % (option("-e"), option("-f")))
         ,
         "   [x]#[y]#[a]%[b]#[-g]%[-h]#[-e]%[-f]");


    test(__LINE__, fmt, ( ( command("make"), value("wordfile"), required("-dict") & value("dictionary"), option("--progress", "-p")) | (
            command("find"), values("infile"), required("-dict") & value("dictionary"),
            (option("-o", "--output") & value("outfile"))  % "write to file instead of stdout",
            ( option("-split"  ) | option("-nosplit"))) | command("help"),
            option("-v", "--version").call([]{}) % "show version")
         ,
       "   make#<wordfile>#-dict%<dictionary>#[--progress]#[-v]\n"
       "   find#,,,<infile>...#-dict%<dictionary>#[-o%<outfile>]#[-§(§split|nosplit§)§]#[-v]\n"
       "   help#[-v]");


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
        "   help\n"
        "   build#§(§new|add§)§#<file>\n"
        "   query#-i%<infile>#[-p]\n"
        "   info#space\n"
        "   info#statistics#§(§words|chars§)§\n"
        "   remove#a§(§ny|ll§)§#<regex>\n"
        "   modify#[-c]#[-u]#[-m%<size>]");


    test(__LINE__, fmt, (
            value("input file"),
            option("-r", "--recursive"),
            option("-o") & value("output format"),
            option("-utf16")),
         "   <input file>#[-r]#[-o%<output format>]#[-utf16]");


    test(__LINE__, fmt,
         joinable( repeatable(option("a")([]{})|option("b")([]{})) ),
         "   ,,,[a|b]...");


    test(__LINE__, fmt, (
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
        ),
         "   [-v]%[-i]#§(§copy|move§)§#[--all]#[--replace]#[-f]#,,,<files>...%[-r]%[-h]\n"
         "   [-v]%[-i]#compare#§(§date|content§)§#[-b]#[-q]#,,,<files>...%[-r]%[-h]\n"
         "   [-v]%[-i]#merge#§(§diff|patch§)§#-o%<outdir>#[--show-conflicts]#,,,<files>...%[-r]%[-h]\n"
         "   [-v]%[-i]#merge#content#[--git-style]#[-m%<marker>]#-o%<outdir>#[--show-conflicts]#,,,<files>...%[-r]%[-h]\n"
         "   [-v]%[-i]#list#,,,<files>...%[-r]%[-h]");


    test(__LINE__, fmt, (
            command("new"),
            value("filename"),
            (option("-e", "--encoding") & value("enc")).doc("'utf8' or 'cp1252'")
        ), "   new#<filename>#[-e%<enc>]");


    test(__LINE__, fmt, ( command("auto") | ( command("label"), value("character") )),
         "   §(§auto^(label%<character>)§)§");


    test(__LINE__, fmt, (
            values("file") % "input filenames",
            (required("-s") & value("expr")) % "string to look for",
            option("any") % "report as soon as any matches" |
            option("all") % "report only if all match"
        ),
        "   ,,,<file>...#-s%<expr>#[a§(§ny|ll§)§]");


    test(__LINE__, fmt, ( value(""), option("-x"), option("-y") ),
         "   <?>%[-x]%[-y]");

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
