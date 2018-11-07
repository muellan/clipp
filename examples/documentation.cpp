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


int main()
{
    using namespace clipp;
    using std::cout;

    auto cli = (
        command("help") |
        ( command("build"),
            "build commands" %
            (   command("new")  % "make new database"
              | command("add")  % "append to existing database"
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
                option("-u", "--unique")   % "keep only unique entries",
                option("-m", "--memlimit") % "max. size in RAM" & value("size")
            )
        )
    );

    auto fmt = doc_formatting{} .first_column(4) .doc_column(28);

    cout << make_man_page(cli, "worddb", fmt)
        .prepend_section("DESCRIPTION", "    Builds a database of words from text files.")
        .append_section("LICENSE", "    GPL3") << '\n';
}
