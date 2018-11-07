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
    active(bool a_, bool b_, bool c_, bool d_, bool e_, bool f_, int i_ = 0) :
        a{a_}, b{b_}, c{c_}, d{d_}, e{e_}, f{f_}, i{i_}
    {}
    bool a = false, b = false, c = false, d = false, e = false, f = false;
    int i = 0;

    friend bool operator == (const active& x, const active& y) noexcept {
        return (x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d &&
                x.e == y.e && x.f == y.f && x.i == y.i);
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
        command("a").set(m1.a),
        command("b").set(m1.b),
        command("c").set(m1.c),
        command("d").set(m1.d),
        opt_value("i").set(m1.i),
        option("-e").set(m1.e),
        option("-f").set(m1.f)
    );

    //equivalent interface
    active m2;
    auto cli2 = (
        required("a").set(m2.a) &
        required("b").set(m2.b) &
        required("c").set(m2.c) &
        required("d").set(m2.d) &
        (
            opt_value("i").set(m2.i),
            option("-e").set(m2.e),
            option("-f").set(m2.f)
        )
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

        test(__LINE__, {"a"},  active{1,0,0,0,0,0, 0});
        test(__LINE__, {"b"},  active{});
        test(__LINE__, {"c"},  active{});
        test(__LINE__, {"d"},  active{});
        test(__LINE__, {"12"}, active{});
        test(__LINE__, {"-e"}, active{});
        test(__LINE__, {"-f"}, active{});

        test(__LINE__, {"a", "b"},  active{1,1,0,0,0,0, 0});
        test(__LINE__, {"a", "c"},  active{1,0,0,0,0,0, 0});
        test(__LINE__, {"a", "d"},  active{1,0,0,0,0,0, 0});
        test(__LINE__, {"a", "12"}, active{1,0,0,0,0,0, 0});
        test(__LINE__, {"a", "-e"}, active{1,0,0,0,0,0, 0});
        test(__LINE__, {"a", "-f"}, active{1,0,0,0,0,0, 0});

        test(__LINE__, {"a", "b", "c"},  active{1,1,1,0,0,0, 0});
        test(__LINE__, {"a", "b", "d"},  active{1,1,0,0,0,0, 0});
        test(__LINE__, {"a", "b", "12"}, active{1,1,0,0,0,0, 0});
        test(__LINE__, {"a", "b", "-e"}, active{1,1,0,0,0,0, 0});
        test(__LINE__, {"a", "b", "-f"}, active{1,1,0,0,0,0, 0});

        test(__LINE__, {"a", "b", "c", "d"},  active{1,1,1,1,0,0, 0});
        test(__LINE__, {"a", "b", "c", "12"}, active{1,1,1,0,0,0, 0});
        test(__LINE__, {"a", "b", "c", "-e"}, active{1,1,1,0,0,0, 0});
        test(__LINE__, {"a", "b", "c", "-f"}, active{1,1,1,0,0,0, 0});

        test(__LINE__, {"a", "b", "c", "d", "-e"},  active{1,1,1,1,1,0, 0});
        test(__LINE__, {"a", "b", "c", "d", "-f"},  active{1,1,1,1,0,1, 0});
        test(__LINE__, {"a", "b", "c", "d", "12"},  active{1,1,1,1,0,0, 12});
        test(__LINE__, {"a", "b", "c", "d", "-3"},  active{1,1,1,1,0,0, -3});

        test(__LINE__, {"a", "b", "c", "d", "12", "-e"},  active{1,1,1,1,1,0, 12});
        test(__LINE__, {"a", "b", "c", "d", "12", "-f"},  active{1,1,1,1,0,1, 12});

        test(__LINE__, {"a", "b", "c", "d", "-e", "12"},  active{1,1,1,1,1,0, 12});
        test(__LINE__, {"a", "b", "c", "d", "-f", "12"},  active{1,1,1,1,0,1, 12});

        test(__LINE__, {"a", "b", "c", "d", "-e", "-f"},  active{1,1,1,1,1,1, 0});
        test(__LINE__, {"a", "b", "c", "d", "-f", "-e"},  active{1,1,1,1,1,1, 0});

        test(__LINE__, {"a", "b", "c", "d", "12", "-e", "-f"},  active{1,1,1,1,1,1, 12});
        test(__LINE__, {"a", "b", "c", "d", "-e", "12", "-f"},  active{1,1,1,1,1,1, 12});
        test(__LINE__, {"a", "b", "c", "d", "-e", "-f", "12"},  active{1,1,1,1,1,1, 12});

        test(__LINE__, {"a", "b", "c", "d", "12", "-f", "-e"},  active{1,1,1,1,1,1, 12});
        test(__LINE__, {"a", "b", "c", "d", "-f", "12", "-e"},  active{1,1,1,1,1,1, 12});
        test(__LINE__, {"a", "b", "c", "d", "-f", "-e", "12"},  active{1,1,1,1,1,1, 12});

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
