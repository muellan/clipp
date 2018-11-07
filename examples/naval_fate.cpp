/*****************************************************************************
 *
 * demo program - part of CLIPP (command line interfaces for modern C++)
 *
 * released under MIT license
 *
 * (c) 2017-2018 André Müller; foss@andremueller-online.de
 *
*
 * This command line interface example was taken from http://docopt.org/.
 *
 * EXAMPLE
 *   We want to model this command line interface:
 *
 *     Naval Fate.
 *
 *     Usage:
 *       naval_fate ship new <name>...
 *       naval_fate ship <name> move <x> <y> [--speed=<kn>]
 *       naval_fate ship shoot <x> <y>
 *       naval_fate mine (set|remove) <x> <y> [--moored|--drifting]
 *       naval_fate -h | --help
 *       naval_fate --version
 *
 *     Options:
 *       -h --help     Show this screen.
 *       --version     Show version.
 *       --speed=<kn>  Speed in knots [default: 10].
 *       --moored      Moored (anchored) mine.
 *       --drifting    Drifting mine.
 *
 *
 *****************************************************************************/

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

#include <clipp.h>


int main(int argc, char* argv[])
{
    using namespace clipp;
    using std::string;
    using std::vector;
    using std::cout;

    int x = 0, y = 0;
    float speed = 10.0f;
    bool drift = true;
    vector<string> names;

    enum class mode { none, help, shipnew, shipmove, shipshoot, mineset, minerem};
    mode selected = mode::none;

    auto coordinates = ( value("x", x), value("y", y) );

    auto shipnew  = ( command("new").set(selected,mode::shipnew),
                      values("name", names) );

    auto shipmove = (
        value("name", names),
        command("move").set(selected,mode::shipmove), coordinates,
        (option("--speed=") & value("kn",speed)) % "Speed in knots [default: 10]");

    auto shipshoot = ( command("shoot").set(selected,mode::shipshoot),
                       coordinates );

    auto mines = ( 
        command("mine"),
        (command("set"   ).set(selected,mode::mineset) | 
         command("remove").set(selected,mode::minerem) ),
        coordinates,
        (option("--moored"  ).set(drift,false) % "Moored (anchored) mine." | 
         option("--drifting").set(drift,true)  % "Drifting mine."          )
    );

    auto navalcli = (
        ( command("ship"), ( shipnew | shipmove | shipshoot ) )
        | mines
        | command("-h", "--help").set(selected,mode::help)     % "Show this screen."
        | command("--version")([]{ cout << "version 1.0\n"; }) % "Show version."
    );

    auto fmt = doc_formatting{}
        .first_column(2)
        .doc_column(20)
        .max_flags_per_param_in_usage(8);

    if(parse(argc, argv, navalcli)) {
        switch(selected) {
            default:
            case mode::none:
                break;
            case mode::help: {
                cout << "Naval Fate.\n\nUsage:\n"
                     << usage_lines(navalcli, "naval_fate", fmt)
                     << "\n\nOptions:\n"
                     << documentation(navalcli, fmt) << '\n';
                }
                break;
            case mode::shipnew:
                if(names.empty()) {
                    cout << "No new ships were made\n";
                } else {
                    cout << "Made new ship" << (names.size() > 1 ? "s " : " ");
                    for(const auto& n : names) cout << n << ' ';
                    cout << "\n";
                }
                break;
            case mode::shipmove:
                if(names.empty()) {
                    cout << "No ship was moved.\n";
                } else {
                    cout << "Moving ship " << names.front()
                         << " to x=" << x << " y=" << y << ".\n";
                }
                break;
            case mode::shipshoot:
                cout << "Ship fired shot aimed at x=" << x << " y=" << y << ".\n";
                break;
            case mode::mineset:
                cout << "Laid " << (drift ? "drifting" : "& moored")
                     << " mine at x=" << x << " y=" << y << ".\n";
                break;
            case mode::minerem:
                cout << "Removed mine at x=" << x << " y=" << y << ".\n";
                break;
        }
    }
    else {
        std::cerr << "Wrong command line arguments.\nUsage:"
                  << usage_lines(navalcli, "naval_fate", fmt) << '\n';
    }

}
