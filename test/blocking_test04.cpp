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
    explicit
    active(bool a_, bool b_, bool c_, bool d_, bool e_, bool f_) :
        a{a_}, b{b_}, c{c_}, d{d_}, e{e_}, f{f_}
    {}
    bool a = false, b = false, c = false, d = false, e = false, f = false;

    friend bool operator == (const active& x, const active& y) noexcept {
        return (x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d &&
                x.e == y.e && x.f == y.f);
    }
};


//-------------------------------------------------------------------
void test(int lineNo,
          const std::initializer_list<const char*> args,
          const active& matches)
{
    using namespace clipp;

    active m1;
    auto cli1 = (
        option("a").set(m1.a),
        group(
            command("b").set(m1.b),
            command("c").set(m1.c),
            command("d").set(m1.d),
            command("e").set(m1.e)
        ),
        option("f").set(m1.f)
    );

    //equivalent interface
    active m2;
    auto cli2 = (
        option("a").set(m2.a),
        (
            required("b").set(m2.b) &
            required("c").set(m2.c) &
            required("d").set(m2.d) &
            required("e").set(m2.e)
        ),
        option("f").set(m2.f)
    );


    run_wrapped_variants({ __FILE__, lineNo }, args, cli1,
              [&]{ m1 = active{}; },
              [&]{ return m1 == matches; });

    run_wrapped_variants({ __FILE__, lineNo }, args, cli2,
              [&]{ m2 = active{}; },
              [&]{ return m2 == matches; });
}


//-------------------------------------------------------------------
int main()
{
    try {
        test(__LINE__, {""}, active{});

        test(__LINE__, {"a"}, active{1,0,0,0,0,0});
        test(__LINE__, {"f"}, active{0,0,0,0,0,1});

        test(__LINE__, {"b"}, active{0,1,0,0,0,0});
        test(__LINE__, {"c"}, active{});
        test(__LINE__, {"d"}, active{});
        test(__LINE__, {"e"}, active{});

        test(__LINE__, {"b", "c"},                active{0,1,1,0,0,0});
        test(__LINE__, {"b", "c", "d"},           active{0,1,1,1,0,0});
        test(__LINE__, {"b", "c", "d", "e"},      active{0,1,1,1,1,0});
        test(__LINE__, {"b", "c", "d", "e", "f"}, active{0,1,1,1,1,1});
        test(__LINE__, {"b", "c", "d", "e", "a"}, active{1,1,1,1,1,0});

        test(__LINE__, {"a", "b"},                active{1,1,0,0,0,0});
        test(__LINE__, {"a", "b", "c"},           active{1,1,1,0,0,0});
        test(__LINE__, {"a", "b", "c", "d"},      active{1,1,1,1,0,0});
        test(__LINE__, {"a", "b", "c", "d", "e"}, active{1,1,1,1,1,0});

        test(__LINE__, {"f", "b"},                active{0,1,0,0,0,1});
        test(__LINE__, {"f", "b", "c"},           active{0,1,1,0,0,1});
        test(__LINE__, {"f", "b", "c", "d"},      active{0,1,1,1,0,1});
        test(__LINE__, {"f", "b", "c", "d", "e"}, active{0,1,1,1,1,1});

        test(__LINE__, {"a", "b", "c", "d", "e", "f"}, active{1,1,1,1,1,1});
        test(__LINE__, {"f", "b", "c", "d", "e", "a"}, active{1,1,1,1,1,1});
        test(__LINE__, {"a", "f", "b", "c", "d", "e"}, active{1,1,1,1,1,1});
        test(__LINE__, {"f", "a", "b", "c", "d", "e"}, active{1,1,1,1,1,1});
        test(__LINE__, {"b", "c", "d", "e", "a", "f"}, active{1,1,1,1,1,1});
        test(__LINE__, {"b", "c", "d", "e", "f", "a"}, active{1,1,1,1,1,1});

        test(__LINE__, {"b", "c", "a"}, active{1,1,1,0,0,0});
        test(__LINE__, {"b", "c", "f"}, active{0,1,1,0,0,1});
        test(__LINE__, {"b", "a", "c"}, active{1,1,0,0,0,0});
        test(__LINE__, {"b", "f", "c"}, active{0,1,0,0,0,1});

        test(__LINE__, {"b", "c", "d", "a"}, active{1,1,1,1,0,0});
        test(__LINE__, {"b", "c", "d", "f"}, active{0,1,1,1,0,1});
        test(__LINE__, {"b", "c", "a", "d"}, active{1,1,1,0,0,0});
        test(__LINE__, {"b", "c", "f", "d"}, active{0,1,1,0,0,1});
        test(__LINE__, {"b", "a", "c", "d"}, active{1,1,0,0,0,0});
        test(__LINE__, {"b", "f", "c", "d"}, active{0,1,0,0,0,1});

        test(__LINE__, {"b", "a", "c", "d", "e"}, active{1,1,0,0,0,0});
        test(__LINE__, {"b", "f", "c", "d", "e"}, active{0,1,0,0,0,1});

        test(__LINE__, {"b", "c", "d", "f", "a", "e"}, active{1,1,1,1,0,1});
        test(__LINE__, {"b", "c", "f", "d", "a", "e"}, active{1,1,1,0,0,1});
        test(__LINE__, {"b", "c", "a", "d", "f", "e"}, active{1,1,1,0,0,1});
        test(__LINE__, {"b", "a", "c", "f", "d", "e"}, active{1,1,0,0,0,1});
        test(__LINE__, {"b", "f", "c", "a", "d", "e"}, active{1,1,0,0,0,1});

        test(__LINE__, {"b", "c", "d", "a", "e"}, active{1,1,1,1,0,0});

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
