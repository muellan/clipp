/*****************************************************************************
 *
 * CLIPP - command line interfaces for modern C++
 *
 * released under MIT license
 *
 * (c) 2017 André Müller; foss@andremueller-online.de
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

    active m;

    auto cli = (
        (
            option("a").set(m.a),
            option("b").set(m.b),
            option("c").set(m.c)
        ) & (
            option("d").set(m.d),
            option("e").set(m.e),
            option("f").set(m.f)
        )
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli,
              [&]{ m = active{}; },
              [&]{ return m == matches; });
}


//-------------------------------------------------------------------
int main()
{
    try {
        test(__LINE__, {""}, active{});

        test(__LINE__, {"a"}, active{1,0,0,0,0,0});
        test(__LINE__, {"b"}, active{0,1,0,0,0,0});
        test(__LINE__, {"c"}, active{0,0,1,0,0,0});

        test(__LINE__, {"d"}, active{0,0,0,0,0,0});
        test(__LINE__, {"e"}, active{0,0,0,0,0,0});
        test(__LINE__, {"f"}, active{0,0,0,0,0,0});

        test(__LINE__, {"a", "d"}, active{1,0,0,1,0,0});
        test(__LINE__, {"a", "e"}, active{1,0,0,0,1,0});
        test(__LINE__, {"a", "f"}, active{1,0,0,0,0,1});

        test(__LINE__, {"b", "d"}, active{0,1,0,1,0,0});
        test(__LINE__, {"b", "e"}, active{0,1,0,0,1,0});
        test(__LINE__, {"b", "f"}, active{0,1,0,0,0,1});

        test(__LINE__, {"c", "d"}, active{0,0,1,1,0,0});
        test(__LINE__, {"c", "e"}, active{0,0,1,0,1,0});
        test(__LINE__, {"c", "f"}, active{0,0,1,0,0,1});

        test(__LINE__, {"c", "b", "d"}, active{0,1,1,1,0,0});
        test(__LINE__, {"b", "a", "e"}, active{1,1,0,0,1,0});
        test(__LINE__, {"c", "a", "f"}, active{1,0,1,0,0,1});

        test(__LINE__, {"c", "a", "b", "d"}, active{1,1,1,1,0,0});
        test(__LINE__, {"b", "c", "a", "e"}, active{1,1,1,0,1,0});
        test(__LINE__, {"c", "b", "a", "f"}, active{1,1,1,0,0,1});

        test(__LINE__, {"c", "a", "b", "d", "e"}, active{1,1,1,1,1,0});
        test(__LINE__, {"b", "c", "a", "e", "d"}, active{1,1,1,1,1,0});
        test(__LINE__, {"c", "b", "a", "f", "d"}, active{1,1,1,1,0,1});
        test(__LINE__, {"c", "b", "a", "d", "f"}, active{1,1,1,1,0,1});
        test(__LINE__, {"c", "b", "a", "f", "e"}, active{1,1,1,0,1,1});
        test(__LINE__, {"c", "b", "a", "e", "f"}, active{1,1,1,0,1,1});

        test(__LINE__, {"a", "b", "c", "d", "e", "f"}, active{1,1,1,1,1,1});
        test(__LINE__, {"c", "b", "a", "f", "e", "d"}, active{1,1,1,1,1,1});
        test(__LINE__, {"c", "b", "a", "f", "d", "e"}, active{1,1,1,1,1,1});
        test(__LINE__, {"c", "b", "a", "e", "d", "f"}, active{1,1,1,1,1,1});
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
