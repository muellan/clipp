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
        return (x.a == y.a && x.b == y.b && x.c == y.c &&
                x.d == y.d && x.e == y.e && x.f == y.f );
    }
};


//-------------------------------------------------------------------
void test(int lineNo,
          const std::initializer_list<const char*> args,
          const active& matches, const active& missing)
{
    using namespace clipp;

    active v;
    active m;

    auto cli = (
        option("a").set(v.a).if_missing(set(m.a)),
        (
            option("b").set(v.b).if_missing(set(m.b)) |
            required("c").set(v.c).if_missing(set(m.c))
        ),
        (
            required("d").set(v.d).if_missing(set(m.d)) |
            required("e").set(v.e).if_missing(set(m.e))
        ),
        required("f").set(v.f).if_missing(set(m.f))
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli,
              [&]{ m = active{}; v = active{}; },
              [&]{ return v == matches && m == missing; });

}


//-------------------------------------------------------------------
int main()
{
    try {
        test(__LINE__, {""}, active{0,0,0,0,0,0}, active{0,0,0,1,1,1});

        test(__LINE__, {"a"}, active{1,0,0,0,0,0}, active{0,0,0,1,1,1});
        test(__LINE__, {"b"}, active{0,1,0,0,0,0}, active{0,0,0,1,1,1});
        test(__LINE__, {"c"}, active{0,0,1,0,0,0}, active{0,0,0,1,1,1});
        test(__LINE__, {"d"}, active{0,0,0,1,0,0}, active{0,0,0,0,0,1});
        test(__LINE__, {"e"}, active{0,0,0,0,1,0}, active{0,0,0,0,0,1});
        test(__LINE__, {"f"}, active{0,0,0,0,0,1}, active{0,0,0,1,1,0});

        test(__LINE__, {"d", "f"}, active{0,0,0,1,0,1}, active{0,0,0,0,0,0});
        test(__LINE__, {"e", "f"}, active{0,0,0,0,1,1}, active{0,0,0,0,0,0});

        test(__LINE__, {"b", "f"}, active{0,1,0,0,0,1}, active{0,0,0,1,1,0});
        test(__LINE__, {"c", "f"}, active{0,0,1,0,0,1}, active{0,0,0,1,1,0});

        test(__LINE__, {"b", "d", "f"}, active{0,1,0,1,0,1}, active{0,0,0,0,0,0});
        test(__LINE__, {"c", "d", "f"}, active{0,0,1,1,0,1}, active{0,0,0,0,0,0});

        test(__LINE__, {"b", "e", "f"}, active{0,1,0,0,1,1}, active{0,0,0,0,0,0});
        test(__LINE__, {"c", "e", "f"}, active{0,0,1,0,1,1}, active{0,0,0,0,0,0});
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
