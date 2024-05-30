/*****************************************************************************
 *
 * CLIPP - command line interfaces for modern C++
 *
 * released under MIT license
 *
 * (c) 2017-2018 André Müller; foss@andremueller-online.de
 *
 *****************************************************************************/

//don't include anything else before the test subject
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

    auto cli = (
        option("a").set(m.a),
        (
            option("b").set(m.b)  |
            command("c").set(m.c) |
            command("d").set(m.d) |
            option("e").set(m.e)
        ),
        option("f").set(m.f)
    );

    run_test({ __FILE__, lineNo }, args, cli, [&]{ return m == matches; });
}

} // namespace

//-------------------------------------------------------------------
int TEST_MAIN()
{
    try {
        test(__LINE__, {""}, test_active{});

        test(__LINE__, {"a"}, test_active{1, 0,0,0,0, 0});
        test(__LINE__, {"b"}, test_active{0, 1,0,0,0, 0});
        test(__LINE__, {"c"}, test_active{0, 0,1,0,0, 0});
        test(__LINE__, {"d"}, test_active{0, 0,0,1,0, 0});
        test(__LINE__, {"e"}, test_active{0, 0,0,0,1, 0});
        test(__LINE__, {"f"}, test_active{0, 0,0,0,0, 1});

        test(__LINE__, {"a", "b"}, test_active{1, 1,0,0,0, 0});
        test(__LINE__, {"a", "c"}, test_active{1, 0,1,0,0, 0});
        test(__LINE__, {"a", "d"}, test_active{1, 0,0,1,0, 0});
        test(__LINE__, {"a", "e"}, test_active{1, 0,0,0,1, 0});
        test(__LINE__, {"a", "f"}, test_active{1, 0,0,0,0, 1});

        test(__LINE__, {"a", "b", "f"}, test_active{1, 1,0,0,0, 1});
        test(__LINE__, {"a", "c", "f"}, test_active{1, 0,1,0,0, 1});
        test(__LINE__, {"a", "d", "f"}, test_active{1, 0,0,1,0, 1});
        test(__LINE__, {"a", "e", "f"}, test_active{1, 0,0,0,1, 1});

        test(__LINE__, {"f", "b"}, test_active{0, 1,0,0,0, 1});
        test(__LINE__, {"f", "c"}, test_active{0, 0,1,0,0, 1});
        test(__LINE__, {"f", "d"}, test_active{0, 0,0,1,0, 1});
        test(__LINE__, {"f", "e"}, test_active{0, 0,0,0,1, 1});

        test(__LINE__, {"b", "f"}, test_active{0, 1,0,0,0, 1});
        test(__LINE__, {"c", "f"}, test_active{0, 0,1,0,0, 1});
        test(__LINE__, {"d", "f"}, test_active{0, 0,0,1,0, 1});
        test(__LINE__, {"e", "f"}, test_active{0, 0,0,0,1, 1});

        test(__LINE__, {"b", "f", "a"}, test_active{1, 1,0,0,0, 1});
        test(__LINE__, {"c", "f", "a"}, test_active{0, 0,1,0,0, 1});
        test(__LINE__, {"d", "f", "a"}, test_active{0, 0,0,1,0, 1});
        test(__LINE__, {"e", "f", "a"}, test_active{1, 0,0,0,1, 1});
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
