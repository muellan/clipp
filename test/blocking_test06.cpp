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
        return (x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d &&
                x.e == y.e && x.f == y.f);
    }
};


//-------------------------------------------------------------------
void test(int lineNo,
          const std::initializer_list<const char*> args,
          const test_active& matches)
{
    using namespace clipp;

    test_active m;

    auto cli1 = (
        (
            option("a").set(m.a),
            option("b").set(m.b),
            option("c").set(m.c)
        ).doc("group abc")
        & (
            option("d").set(m.d),
            option("e").set(m.e),
            option("f").set(m.f)
        ).doc("group def")
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli1,
              [&]{ m = test_active{}; },
              [&]{ return m == matches; });


    auto cli2 = (
        (
            option("x"),
            (
                option("a").set(m.a),
                option("b").set(m.b),
                option("c").set(m.c)
            ).doc("group abc")
        ).doc("group x(abc)")
        & (
            option("d").set(m.d),
            option("e").set(m.e),
            option("f").set(m.f)
        ).doc("group def")
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli2,
              [&]{ m = test_active{}; },
              [&]{ return m == matches; });


    auto cli3 = (
        (
            (   option("a").set(m.a),
                option("b").set(m.b)
            ).doc("group ab")
            ,
            option("c").set(m.c)
        ).doc("group (ab)c")
        & (
            (   option("d").set(m.d),
                option("e").set(m.e)
            ).doc("group de")
            ,
            option("f").set(m.f)
        ).doc("group (de)f")
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli3,
              [&]{ m = test_active{}; },
              [&]{ return m == matches; });


    auto cli4 = (
        (
            (
                (   option("a").set(m.a),
                    option("b").set(m.b)
                ).doc("group ab")
                ,
                option("c").set(m.c)
            ).doc("group (ab)c"),
            option("x")
        ).doc("group ((ab)c)x")
        & (
            (
                (   option("d").set(m.d),
                    option("e").set(m.e)
                ).doc("group de")
                ,
                option("f").set(m.f)
            ).doc("group (de)f"),
            option("y")
        ).doc("group ((de)f)y")
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli4,
              [&]{ m = test_active{}; },
              [&]{ return m == matches; });

}

} // namespace

//-------------------------------------------------------------------
int TEST_MAIN()
{
    try {
        test(__LINE__, {""}, test_active{});

        test(__LINE__, {"a"}, test_active{1,0,0,0,0,0});
        test(__LINE__, {"b"}, test_active{0,1,0,0,0,0});
        test(__LINE__, {"c"}, test_active{0,0,1,0,0,0});

        test(__LINE__, {"d"}, test_active{0,0,0,0,0,0});
        test(__LINE__, {"e"}, test_active{0,0,0,0,0,0});
        test(__LINE__, {"f"}, test_active{0,0,0,0,0,0});

        test(__LINE__, {"a", "d"}, test_active{1,0,0,1,0,0});
        test(__LINE__, {"a", "e"}, test_active{1,0,0,0,1,0});
        test(__LINE__, {"a", "f"}, test_active{1,0,0,0,0,1});

        test(__LINE__, {"b", "d"}, test_active{0,1,0,1,0,0});
        test(__LINE__, {"b", "e"}, test_active{0,1,0,0,1,0});
        test(__LINE__, {"b", "f"}, test_active{0,1,0,0,0,1});

        test(__LINE__, {"c", "d"}, test_active{0,0,1,1,0,0});
        test(__LINE__, {"c", "e"}, test_active{0,0,1,0,1,0});
        test(__LINE__, {"c", "f"}, test_active{0,0,1,0,0,1});

        test(__LINE__, {"c", "b", "d"}, test_active{0,1,1,1,0,0});
        test(__LINE__, {"b", "a", "e"}, test_active{1,1,0,0,1,0});
        test(__LINE__, {"c", "a", "f"}, test_active{1,0,1,0,0,1});

        test(__LINE__, {"c", "a", "b", "d"}, test_active{1,1,1,1,0,0});
        test(__LINE__, {"b", "c", "a", "e"}, test_active{1,1,1,0,1,0});
        test(__LINE__, {"c", "b", "a", "f"}, test_active{1,1,1,0,0,1});

        test(__LINE__, {"c", "a", "b", "d", "e"}, test_active{1,1,1,1,1,0});
        test(__LINE__, {"b", "c", "a", "e", "d"}, test_active{1,1,1,1,1,0});
        test(__LINE__, {"c", "b", "a", "f", "d"}, test_active{1,1,1,1,0,1});
        test(__LINE__, {"c", "b", "a", "d", "f"}, test_active{1,1,1,1,0,1});
        test(__LINE__, {"c", "b", "a", "f", "e"}, test_active{1,1,1,0,1,1});
        test(__LINE__, {"c", "b", "a", "e", "f"}, test_active{1,1,1,0,1,1});

        test(__LINE__, {"a", "b", "c", "d", "e", "f"}, test_active{1,1,1,1,1,1});
        test(__LINE__, {"c", "b", "a", "f", "e", "d"}, test_active{1,1,1,1,1,1});
        test(__LINE__, {"c", "b", "a", "f", "d", "e"}, test_active{1,1,1,1,1,1});
        test(__LINE__, {"c", "b", "a", "e", "d", "f"}, test_active{1,1,1,1,1,1});
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
