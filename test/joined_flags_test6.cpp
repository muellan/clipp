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
    active(bool a_, bool b_, bool c_, bool d_):
        a{a_}, b{b_}, c{c_}, d{d_}
    {}
    bool a = false, b = false, c = false, d = false;

    friend bool operator == (const active& x, const active& y) noexcept {
        return (x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d);
    }
};


//-------------------------------------------------------------------
void test(int lineNo,
          const std::initializer_list<const char*> args,
          const active& matches)
{
    using namespace clipp;
    active m;

    auto cli =  (
        joinable(option("a").set(m.a),
                 option("b").set(m.b)),
        joinable(option("-a").set(m.c),
                 option("-b").set(m.d))  );

    parse(args, cli);

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

        test(__LINE__, {"a"},     active{1,0,0,0});
        test(__LINE__, {"b"},     active{0,1,0,0});
        test(__LINE__, {"a","b"}, active{1,1,0,0});
        test(__LINE__, {"b","a"}, active{1,1,0,0});
        test(__LINE__, {"ab"},    active{1,1,0,0});
        test(__LINE__, {"ba"},    active{1,1,0,0});
        //second group
        test(__LINE__, {"-a"},      active{0,0,1,0});
        test(__LINE__, {"-b"},      active{0,0,0,1});
        test(__LINE__, {"-a","-b"}, active{0,0,1,1});
        test(__LINE__, {"-b","-a"}, active{0,0,1,1});
        test(__LINE__, {"-ab"},     active{0,0,1,1});
        test(__LINE__, {"-ba"},     active{0,0,1,1});
        test(__LINE__, {"-b-a"},    active{0,0,1,1});
        test(__LINE__, {"-a-b"},    active{0,0,1,1});
        //fail cases
        test(__LINE__, {"--ab"}, active{});
        test(__LINE__, {"ab-"},  active{});
        test(__LINE__, {"ba-"},  active{});
        test(__LINE__, {"a-b"},  active{});
        test(__LINE__, {"b-a"},  active{});
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
