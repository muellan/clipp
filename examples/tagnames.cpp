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
#include <set>
#include <string>

#include <clipp.h>


int main(int argc, char* argv[])
{
    using namespace clipp;
    using std::cout;
    using std::string;

    auto tag_name = [] (const string& arg) {
        if(arg.size() < 3) return subrange{}; //too short
        auto i = arg.find("<");
        if(i == string::npos) return subrange{}; //no tag start found
        auto j = arg.find(">", i+1);
        if(j == string::npos) return subrange{}; //didn't find end of tag
        return subrange{i,j-i+1}; //partial match {start, length}
    };


    std::set<string> tags;
    auto cli = joinable(
        values(tag_name, "string",
               [&](const string& arg){ if(arg[1] != '/') tags.insert(arg); })
    );

    if(parse(argc, argv, cli)) {
        cout << "tag names:\n";
        for(const auto& t : tags) cout << t << '\n';
    } else {
        cout << "Usage:\n" << usage_lines(cli, argv[0]) << '\n';
    }
}
