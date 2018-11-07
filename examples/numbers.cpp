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
#include <vector>
#include <cmath>

#include <clipp.h>


int main(int argc, char* argv[])
{
    using namespace clipp;
    using std::cout;

    std::vector<double> nums;

    auto cli = joinable( repeatable(
                   option(",") , opt_number("number", nums)
               ) );

    if(parse(argc, argv, cli)) {
        cout << "numbers:\n";
        for(auto n : nums) cout << n << '\n';
    } else {
        cout << "Usage:\n" << usage_lines(cli, argv[0]) << '\n';
    }
}
