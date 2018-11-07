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

    std::string file;
    bool readonly = false;
    bool usebackup = false;
    bool useswap = false;
    enum class editor {vim, sublime3, atom, emacs};
    std::vector<editor> editors;
    auto add = [&](editor e){ return [&]{ editors.push_back(e); }; };

    auto cli = (
        value("file", file),
        joinable(
            option("-r").set(readonly)  % "open read-only",
            option("-b").set(usebackup) % "use backup file",
            option("-s").set(useswap)   % "use swap file"
        ),
        joinable(
            option(":vim")   >> add(editor::vim),
            option(":st3")   >> add(editor::sublime3),
            option(":atom")  >> add(editor::atom),
            option(":emacs") >> add(editor::emacs)
        ) % "editor(s) to use; multiple possible"
    );


    if(parse(argc, argv, cli)) {
        cout << "open " << file
             << (readonly  ? " read-only" : "")
             << (usebackup ? " using backup" : "")
             << (useswap   ? " using swap" : "");

        if(!editors.empty()) {
            bool first = true;
            cout << "\nwith editor(s): ";
            for(auto e : editors) {
                if(first) first = false; else cout << ", ";
                switch(e) {
                    case editor::vim:      cout << "vim"; break;
                    case editor::sublime3: cout << "Sublime Text 3"; break;
                    case editor::atom:     cout << "Atom"; break;
                    case editor::emacs:    cout << "GNU Emacs"; break;
                }
            }
        }
        cout << '\n';
    }
    else {
        cout << make_man_page(cli, argv[0]) << '\n';
    }
}
