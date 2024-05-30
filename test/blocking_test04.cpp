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

    test_active m1;
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
    test_active m2;
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
              [&]{ m1 = test_active{}; },
              [&]{ return m1 == matches; });

    run_wrapped_variants({ __FILE__, lineNo }, args, cli2,
              [&]{ m2 = test_active{}; },
              [&]{ return m2 == matches; });
}

} // namespace

//-------------------------------------------------------------------
int TEST_MAIN()
{
    try {
        test(__LINE__, {""}, test_active{});

        test(__LINE__, {"a"}, test_active{1,0,0,0,0,0});
        test(__LINE__, {"f"}, test_active{0,0,0,0,0,1});

        test(__LINE__, {"b"}, test_active{0,1,0,0,0,0});
        test(__LINE__, {"c"}, test_active{});
        test(__LINE__, {"d"}, test_active{});
        test(__LINE__, {"e"}, test_active{});

        test(__LINE__, {"b", "c"},                test_active{0,1,1,0,0,0});
        test(__LINE__, {"b", "c", "d"},           test_active{0,1,1,1,0,0});
        test(__LINE__, {"b", "c", "d", "e"},      test_active{0,1,1,1,1,0});
        test(__LINE__, {"b", "c", "d", "e", "f"}, test_active{0,1,1,1,1,1});
        test(__LINE__, {"b", "c", "d", "e", "a"}, test_active{1,1,1,1,1,0});

        test(__LINE__, {"a", "b"},                test_active{1,1,0,0,0,0});
        test(__LINE__, {"a", "b", "c"},           test_active{1,1,1,0,0,0});
        test(__LINE__, {"a", "b", "c", "d"},      test_active{1,1,1,1,0,0});
        test(__LINE__, {"a", "b", "c", "d", "e"}, test_active{1,1,1,1,1,0});

        test(__LINE__, {"f", "b"},                test_active{0,1,0,0,0,1});
        test(__LINE__, {"f", "b", "c"},           test_active{0,1,1,0,0,1});
        test(__LINE__, {"f", "b", "c", "d"},      test_active{0,1,1,1,0,1});
        test(__LINE__, {"f", "b", "c", "d", "e"}, test_active{0,1,1,1,1,1});

        test(__LINE__, {"a", "b", "c", "d", "e", "f"}, test_active{1,1,1,1,1,1});
        test(__LINE__, {"f", "b", "c", "d", "e", "a"}, test_active{1,1,1,1,1,1});
        test(__LINE__, {"a", "f", "b", "c", "d", "e"}, test_active{1,1,1,1,1,1});
        test(__LINE__, {"f", "a", "b", "c", "d", "e"}, test_active{1,1,1,1,1,1});
        test(__LINE__, {"b", "c", "d", "e", "a", "f"}, test_active{1,1,1,1,1,1});
        test(__LINE__, {"b", "c", "d", "e", "f", "a"}, test_active{1,1,1,1,1,1});

        test(__LINE__, {"b", "c", "a"}, test_active{1,1,1,0,0,0});
        test(__LINE__, {"b", "c", "f"}, test_active{0,1,1,0,0,1});
        test(__LINE__, {"b", "a", "c"}, test_active{1,1,0,0,0,0});
        test(__LINE__, {"b", "f", "c"}, test_active{0,1,0,0,0,1});

        test(__LINE__, {"b", "c", "d", "a"}, test_active{1,1,1,1,0,0});
        test(__LINE__, {"b", "c", "d", "f"}, test_active{0,1,1,1,0,1});
        test(__LINE__, {"b", "c", "a", "d"}, test_active{1,1,1,0,0,0});
        test(__LINE__, {"b", "c", "f", "d"}, test_active{0,1,1,0,0,1});
        test(__LINE__, {"b", "a", "c", "d"}, test_active{1,1,0,0,0,0});
        test(__LINE__, {"b", "f", "c", "d"}, test_active{0,1,0,0,0,1});

        test(__LINE__, {"b", "a", "c", "d", "e"}, test_active{1,1,0,0,0,0});
        test(__LINE__, {"b", "f", "c", "d", "e"}, test_active{0,1,0,0,0,1});

        test(__LINE__, {"b", "c", "d", "f", "a", "e"}, test_active{1,1,1,1,0,1});
        test(__LINE__, {"b", "c", "f", "d", "a", "e"}, test_active{1,1,1,0,0,1});
        test(__LINE__, {"b", "c", "a", "d", "f", "e"}, test_active{1,1,1,0,0,1});
        test(__LINE__, {"b", "a", "c", "f", "d", "e"}, test_active{1,1,0,0,0,1});
        test(__LINE__, {"b", "f", "c", "a", "d", "e"}, test_active{1,1,0,0,0,1});

        test(__LINE__, {"b", "c", "d", "a", "e"}, test_active{1,1,1,1,0,0});

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
