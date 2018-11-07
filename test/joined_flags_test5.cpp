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


//-------------------------------------------------------------------
struct active {
    active() = default;
    active(bool a_, bool b_): a{a_}, b{b_} {}
    bool a = false, b = false;

    friend bool operator == (const active& x, const active& y) noexcept {
        return (x.a == y.a && x.b == y.b);
    }
};


//-------------------------------------------------------------------
void test(int lineNo,
          const std::initializer_list<const char*> args,
          const active& matches)
{
    using namespace clipp;
    active m;

    auto cli = joinable(
        option(">:a").set(m.a),
        option(">:b").set(m.b)
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli,
              [&]{ m = active{}; },
              [&]{ return m == matches; });
}


//-------------------------------------------------------------------
int main()
{
    using std::string;

    try {
        test(__LINE__, {""}, active{});

        test(__LINE__, {">:a"},       active{1,0});
        test(__LINE__, {">:b"},       active{0,1});
        test(__LINE__, {">:a",">:b"}, active{1,1});
        test(__LINE__, {">:b",">:a"}, active{1,1});
        test(__LINE__, {">:ab"},      active{1,1});
        test(__LINE__, {">:ba"},      active{1,1});
        //fail cases
        test(__LINE__, {">:-ab"}, active{});
        test(__LINE__, {"bca"},   active{});
        test(__LINE__, {"-bca"},  active{});
        test(__LINE__, {">:b-a"}, active{});
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
