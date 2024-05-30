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

namespace {

//-------------------------------------------------------------------
struct test_active {
    test_active() = default;
    explicit
    test_active(bool a_, bool b_, bool c_, bool d_, bool e_, bool f_) :
        a{a_}, b{b_}, c{c_}, d{d_}, e{e_}, f{f_}
    {}
    bool a = false, b = false, c = false, d = false, e = false, f = false;

    friend bool operator == (const test_active& x, const test_active& y) noexcept {
        return (x.a == y.a && x.b == y.b && x.c == y.c &&
                x.d == y.d && x.e == y.e && x.f == y.f );
    }
};


//-------------------------------------------------------------------
void test(int lineNo,
          const std::initializer_list<const char*> args,
          const test_active& matches, const test_active& missing)
{
    using namespace clipp;

    test_active v;
    test_active m;

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
              [&]{ m = test_active{}; v = test_active{}; },
              [&]{ return v == matches && m == missing; });

}

} // namespace

//-------------------------------------------------------------------
int TEST_MAIN()
{
    try {
        test(__LINE__, {""}, test_active{0,0,0,0,0,0}, test_active{0,0,0,1,1,1});

        test(__LINE__, {"a"}, test_active{1,0,0,0,0,0}, test_active{0,0,0,1,1,1});
        test(__LINE__, {"b"}, test_active{0,1,0,0,0,0}, test_active{0,0,0,1,1,1});
        test(__LINE__, {"c"}, test_active{0,0,1,0,0,0}, test_active{0,0,0,1,1,1});
        test(__LINE__, {"d"}, test_active{0,0,0,1,0,0}, test_active{0,0,0,0,0,1});
        test(__LINE__, {"e"}, test_active{0,0,0,0,1,0}, test_active{0,0,0,0,0,1});
        test(__LINE__, {"f"}, test_active{0,0,0,0,0,1}, test_active{0,0,0,1,1,0});

        test(__LINE__, {"d", "f"}, test_active{0,0,0,1,0,1}, test_active{0,0,0,0,0,0});
        test(__LINE__, {"e", "f"}, test_active{0,0,0,0,1,1}, test_active{0,0,0,0,0,0});

        test(__LINE__, {"b", "f"}, test_active{0,1,0,0,0,1}, test_active{0,0,0,1,1,0});
        test(__LINE__, {"c", "f"}, test_active{0,0,1,0,0,1}, test_active{0,0,0,1,1,0});

        test(__LINE__, {"b", "d", "f"}, test_active{0,1,0,1,0,1}, test_active{0,0,0,0,0,0});
        test(__LINE__, {"c", "d", "f"}, test_active{0,0,1,1,0,1}, test_active{0,0,0,0,0,0});

        test(__LINE__, {"b", "e", "f"}, test_active{0,1,0,0,1,1}, test_active{0,0,0,0,0,0});
        test(__LINE__, {"c", "e", "f"}, test_active{0,0,1,0,1,1}, test_active{0,0,0,0,0,0});
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
