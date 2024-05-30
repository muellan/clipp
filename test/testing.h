#pragma once

//don't include anything else before the test subject
#include "../include/clipp.h"

#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <type_traits>
#include <limits>

#define __STR_CAT(A, B) A##B
#define _STR_CAT(A, B)  __STR_CAT(A, B)

#ifdef ALL_TESTS_AS_SINGLE_PRJ
#  ifndef THIS_TEST_FNAME
#    error macro THIS_TEST_FNAME must be defined!
#  endif // !THIS_TEST_FNAME
#  define TEST_MAIN _STR_CAT(THIS_TEST_FNAME, _main)
#else
#  define TEST_MAIN main
#endif

/*************************************************************************//**
 *
 * @brief stores the location of a test definition
 *
 *****************************************************************************/
struct test_location
{
    test_location(std::string file_, int line_):
        file{std::move(file_)}, line{line_}
    {}

    std::string file;
    int line;

#ifdef _XTRA_FUNCTIONAL
    void debug_out(char const* message) const
    {
        std::ostringstream stm;
        stm << file.c_str() << "(" << line << "): " << message << std::endl;
#if defined(_WIN32)
        OutputDebugStringA(stm.str().c_str());
#endif
        std::cout << stm.str().c_str() << std::endl;
    }
#else // !_XTRA_FUNCTIONAL
    void debug_out(char const*) const {}
#endif
};



/*************************************************************************//**
 *
 * @brief returns wrapped/nested variants of the input command line interface
 *
 *****************************************************************************/
template<class CLI>
inline std::vector<CLI>
wrapped_variants(const CLI& cli)
{
    using namespace clipp;
    return {
        /* 1*/ cli
        ,
        /* 2*/ group{cli}
        ,
        /* 3*/ group{group{cli}}
        ,
        /* 4*/ group{option("?a?"), cli}
        ,
        /* 5*/ group{cli, option("?a?")}
        ,
        /* 6*/ group{option("?b?"), cli, option("?a?")}
        ,
        /* 7*/ group{group{option("?a?")}, cli}
        ,
        /* 8*/ group{cli, group{option("?a?")}}
        ,
        /* 9*/ group{option("?a?"), group{cli}}
        ,
        /*10*/ group{group{cli}, option("?a?")}
        ,
        /*11*/ group{option("?b?"), group{cli}, option("?a?")}
    };
}



/*************************************************************************//**
 *
 * @brief runs CLI validity tests based on differently wrapped variants of
 *        the original input CLI
 *
 *****************************************************************************/
template<class CLI>
inline void run_wrapped_variants(
    const test_location& info,
    const std::initializer_list<const char*>& args,
    const CLI& cli,
    std::function<void()> initialize,
    std::function<bool()> valid)
{
    using std::cout;
    using std::to_string;

    int variant = 0;
    for(const auto& wrappedCli : wrapped_variants(cli)) {
        ++variant;

        initialize();

        parse(args, wrappedCli);

        //cout << "==============================================\n"
        //     << " in file " << info.file << " in line " << info.line
        //     << " with variant " << variant << '\n'
        //     << "==============================================\n";
        //clipp::debug::print(cout, wrappedCli);
        //cout << "args = { ";
        //for(const auto& a : args) cout << '"' << a << "\" ";
        //cout << "}\n";
        //auto res = clipp::parse(args, wrappedCli);
        //cout << "----------------------------------------------\n";
        //clipp::debug::print(cout, res);

        if(!valid()) {
            info.debug_out("TEST FAILED");
            throw std::runtime_error{"failed test " + info.file +
                                     " in line " + to_string(info.line) +
                                     " with variant " + to_string(variant) };
        }
    }
}



/*************************************************************************//**
 *
 * @brief runs CLI validity test
 *
 *****************************************************************************/
template<class CLI>
inline void run_test(
    const test_location& info,
    const std::initializer_list<const char*>& args,
    const CLI& cli,
    std::function<bool()> valid)
{
    using std::cout;
    using std::to_string;

    parse(args, cli);

    //cout << "==============================================\n"
    //     << " in file " << info.file << " in line " << info.line << '\n'
    //     << "==============================================\n";
    //clipp::debug::print(cout, cli);
    //cout << "args = { ";
    //for(const auto& a : args) cout << '"' << a << "\" ";
    //cout << "}\n";
    //auto res = clipp::parse(args, cli);
    //cout << "----------------------------------------------\n";
    //clipp::debug::print(cout, res);

    if(!valid()) {
        info.debug_out("TEST FAILED");
        throw std::runtime_error{"failed test " + info.file +
                                 " in line " + to_string(info.line) };
    }
}
