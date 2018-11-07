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

    //[a] ([b] [c] [d] [e]) [f]
    active m1;
    auto cli1 = (
        option("a").set(m1.a),
        in_sequence(
            option("b").set(m1.b),
            option("c").set(m1.c),
            option("d").set(m1.d),
            option("e").set(m1.e)
        ),
        option("f").set(m1.f)
    );

    //equivalent interface
    active m2;
    auto cli2 = (
        option("a").set(m2.a),
        (
            option("b").set(m2.b) &
            option("c").set(m2.c) &
            option("d").set(m2.d) &
            option("e").set(m2.e)
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

        test(__LINE__, {"a"},  active{1,0,0,0,0,0});
        test(__LINE__, {"b"},  active{0,1,0,0,0,0});
        test(__LINE__, {"c"},  active{0,0,0,0,0,0});
        test(__LINE__, {"d"},  active{0,0,0,0,0,0});
        test(__LINE__, {"e"},  active{0,0,0,0,0,0});
        test(__LINE__, {"f"},  active{0,0,0,0,0,1});

        test(__LINE__, {"a", "b"},  active{1,1,0,0,0,0});
        test(__LINE__, {"a", "c"},  active{1,0,0,0,0,0});
        test(__LINE__, {"a", "d"},  active{1,0,0,0,0,0});
        test(__LINE__, {"a", "e"},  active{1,0,0,0,0,0});

        test(__LINE__, {"b", "a"},  active{1,1,0,0,0,0});
        test(__LINE__, {"c", "a"},  active{1,0,0,0,0,0});
        test(__LINE__, {"d", "a"},  active{1,0,0,0,0,0});
        test(__LINE__, {"e", "a"},  active{1,0,0,0,0,0});

        test(__LINE__, {"f", "b"},  active{0,1,0,0,0,1});
        test(__LINE__, {"f", "c"},  active{0,0,0,0,0,1});
        test(__LINE__, {"f", "d"},  active{0,0,0,0,0,1});
        test(__LINE__, {"f", "e"},  active{0,0,0,0,0,1});

        test(__LINE__, {"b", "f"},  active{0,1,0,0,0,1});
        test(__LINE__, {"c", "f"},  active{0,0,0,0,0,1});
        test(__LINE__, {"d", "f"},  active{0,0,0,0,0,1});
        test(__LINE__, {"e", "f"},  active{0,0,0,0,0,1});

        test(__LINE__, {"a", "f", "b"},  active{1,1,0,0,0,1});
        test(__LINE__, {"a", "f", "c"},  active{1,0,0,0,0,1});
        test(__LINE__, {"a", "f", "d"},  active{1,0,0,0,0,1});
        test(__LINE__, {"a", "f", "e"},  active{1,0,0,0,0,1});

        test(__LINE__, {"a", "b", "f"},  active{1,1,0,0,0,1});
        test(__LINE__, {"a", "c", "f"},  active{1,0,0,0,0,1});
        test(__LINE__, {"a", "d", "f"},  active{1,0,0,0,0,1});
        test(__LINE__, {"a", "e", "f"},  active{1,0,0,0,0,1});

        test(__LINE__, {"f", "a", "b"},  active{1,1,0,0,0,1});
        test(__LINE__, {"f", "a", "c"},  active{1,0,0,0,0,1});
        test(__LINE__, {"f", "a", "d"},  active{1,0,0,0,0,1});
        test(__LINE__, {"f", "a", "e"},  active{1,0,0,0,0,1});

        test(__LINE__, {"a", "f", "b"},  active{1,1,0,0,0,1});
        test(__LINE__, {"a", "f", "c"},  active{1,0,0,0,0,1});
        test(__LINE__, {"a", "f", "d"},  active{1,0,0,0,0,1});
        test(__LINE__, {"a", "f", "e"},  active{1,0,0,0,0,1});

        test(__LINE__, {"b", "c"},           active{0,1,1,0,0,0});
        test(__LINE__, {"b", "c", "d"},      active{0,1,1,1,0,0});
        test(__LINE__, {"b", "c", "d", "e"}, active{0,1,1,1,1,0});

        test(__LINE__, {"b", "c", "a"},           active{1,1,1,0,0,0});
        test(__LINE__, {"b", "c", "d", "a"},      active{1,1,1,1,0,0});
        test(__LINE__, {"b", "c", "d", "e", "a"}, active{1,1,1,1,1,0});

        test(__LINE__, {"b", "c", "f"},           active{0,1,1,0,0,1});
        test(__LINE__, {"b", "c", "d", "f"},      active{0,1,1,1,0,1});
        test(__LINE__, {"b", "c", "d", "e", "f"}, active{0,1,1,1,1,1});

        test(__LINE__, {"b", "c", "f", "a"},           active{1,1,1,0,0,1});
        test(__LINE__, {"b", "c", "d", "f", "a"},      active{1,1,1,1,0,1});
        test(__LINE__, {"b", "c", "d", "e", "f", "a"}, active{1,1,1,1,1,1});

        test(__LINE__, {"b", "c", "a", "f"},           active{1,1,1,0,0,1});
        test(__LINE__, {"b", "c", "d", "a", "f"},      active{1,1,1,1,0,1});
        test(__LINE__, {"b", "c", "d", "e", "a", "f"}, active{1,1,1,1,1,1});

        test(__LINE__, {"f", "b", "c", "a"},           active{1,1,1,0,0,1});
        test(__LINE__, {"f", "b", "c", "d", "a"},      active{1,1,1,1,0,1});
        test(__LINE__, {"f", "b", "c", "d", "e", "a"}, active{1,1,1,1,1,1});

        test(__LINE__, {"a", "b", "c"},           active{1,1,1,0,0,0});
        test(__LINE__, {"a", "b", "c", "d"},      active{1,1,1,1,0,0});
        test(__LINE__, {"a", "b", "c", "d", "e"}, active{1,1,1,1,1,0});

        test(__LINE__, {"b", "a", "c"},           active{1,1,0,0,0,0});
        test(__LINE__, {"b", "a", "c", "d"},      active{1,1,0,0,0,0});
        test(__LINE__, {"b", "a", "c", "d", "e"}, active{1,1,0,0,0,0});

        test(__LINE__, {"b", "a", "c"},                active{1,1,0,0,0,0});
        test(__LINE__, {"b", "c", "a", "d", "f", "e"}, active{1,1,1,0,0,1});

        test(__LINE__, {"c", "b"},           active{0,1,0,0,0,0});
        test(__LINE__, {"d", "b"},           active{0,1,0,0,0,0});
        test(__LINE__, {"e", "b"},           active{0,1,0,0,0,0});
        test(__LINE__, {"c", "d", "b"},      active{0,1,0,0,0,0});
        test(__LINE__, {"b", "d", "c"},      active{0,1,1,0,0,0});
        test(__LINE__, {"b", "c", "e", "d"}, active{0,1,1,1,0,0});
        test(__LINE__, {"b", "d", "c", "e"}, active{0,1,1,0,0,0});

        test(__LINE__, {"b", "d"}, active{0,1,0,0,0,0});
        test(__LINE__, {"b", "e"}, active{0,1,0,0,0,0});

        test(__LINE__, {"c", "d"}, active{});
        test(__LINE__, {"c", "e"}, active{});
        test(__LINE__, {"d", "c"}, active{});
        test(__LINE__, {"d", "e"}, active{});
        test(__LINE__, {"e", "d"}, active{});
        test(__LINE__, {"e", "c"}, active{});
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
