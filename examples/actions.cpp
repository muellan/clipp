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

    bool a = false, b = false;
    int i = 1, n = 0, m = 0;
    float x = 0.0f;

    auto cli = (                           //INFORMAL description
        option("-a").set(a),               //if(found("-a")) a = true;
        option("-b") >> b,                 //if(found("-b")) b = true;
        option("--toggle").call(flip(b)),  //if(found("--toggle")) flip(b);

        //if(found("-z")) call_lambda("-z");
        option("-y").call([](const char* s) { cout << s << '\n'; }),

        //using 'operator()' instead of 'call'
        //if(found("bob")) call_lambda("bob");
        option("bob")([](std::string s) { cout << s; }),

        //for_each_occurence("-x arg", call_lambda(arg));
        option("-x") & values("X")([&](const char* s) { x = std::stof(s); }),

        //if(parsing_error_on("-z") call_lambda(get_errors())
        required("-z").if_missing([](){ cout << "-z is missing\n"; }),

        option("--all")
            >> []()              { cout << "found --all\n"; }
            >> [](const char* s) { cout << "found flag " << s << '\n'; },

        value("n").set(n),                 //n = std::atoi(arg);
        option("-i") & value("#",i),       //if(found("-i arg")) i = std::atoi(arg);
        option("-1").set(m,1),             //if(found("-1")) m = 1;
        option("-2").set(m,2)              //if(found("-2")) m = 2;
    );


    parse(argc, argv, cli);

}
