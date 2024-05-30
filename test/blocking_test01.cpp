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
    test_active(bool a_, bool b_, bool c_, bool d_, bool e_, bool f_, int i_ = 0) :
        a{a_}, b{b_}, c{c_}, d{d_}, e{e_}, f{f_}, i{i_}
    {}
    bool a = false, b = false, c = false, d = false, e = false, f = false;
    int i = 0;

    friend bool operator == (const test_active& x, const test_active& y) noexcept {
        return (x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d &&
                x.e == y.e && x.f == y.f && x.i == y.i);
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
        option("-a").set(m.a),

        command("-b").set(m.b),
        option("-c").set(m.c),
        option("-i") & value("i", m.i),
        option("-d").set(m.d),

        command("-e").set(m.e),
        option("-f").set(m.f)
    );

    run_test({ __FILE__, lineNo }, args, cli, [&]{ return m == matches; });
}

} // namespace

//-------------------------------------------------------------------
int TEST_MAIN()
{
    try {
        test(__LINE__, {""}, test_active{});

        test(__LINE__, {"-a"}, test_active{1,0,0,0,0,0, 0});

        test(__LINE__, {"-a", "-c"}, test_active{1,0,0,0,0,0, 0});
        test(__LINE__, {"-a", "-d"}, test_active{1,0,0,0,0,0, 0});
        test(__LINE__, {"-a", "-e"}, test_active{1,0,0,0,0,0, 0});
        test(__LINE__, {"-a", "-f"}, test_active{1,0,0,0,0,0, 0});
        test(__LINE__, {"-a", "-i", "123"}, test_active{1,0,0,0,0,0, 0});

        test(__LINE__, {"-b"}, test_active{0,1,0,0,0,0, 0});
        test(__LINE__, {"-b", "-a"}, test_active{0,1,0,0,0,0, 0});
        test(__LINE__, {"-a", "-b"}, test_active{1,1,0,0,0,0, 0});

        test(__LINE__, {"-b", "-c"}, test_active{0,1,1,0,0,0, 0});
        test(__LINE__, {"-a", "-b", "-c"}, test_active{1,1,1,0,0,0, 0});
        test(__LINE__, {"-a", "-b", "-c", "-i"}, test_active{1,1,1,0,0,0, 0});
        test(__LINE__, {"-a", "-b", "-c", "-i", "123"}, test_active{1,1,1,0,0,0, 123});

        test(__LINE__, {"-b", "-i", "123"},             test_active{0,1,0,0,0,0, 123});
        test(__LINE__, {"-a", "-b", "-i", "123"},       test_active{1,1,0,0,0,0, 123});
        test(__LINE__, {"-a", "-b", "-c", "-i", "123"}, test_active{1,1,1,0,0,0, 123});
        test(__LINE__, {"-a", "-b", "-d", "-i", "123"}, test_active{1,1,0,1,0,0, 123});
        test(__LINE__, {"-a", "-b", "-i", "123", "-c"}, test_active{1,1,1,0,0,0, 123});
        test(__LINE__, {"-a", "-b", "-i", "123", "-d"}, test_active{1,1,0,1,0,0, 123});

        test(__LINE__, {"-b", "-i", "123", "-a"}, test_active{0,1,0,0,0,0, 123});
        test(__LINE__, {"-b", "-i", "123", "-a"}, test_active{0,1,0,0,0,0, 123});

        test(__LINE__, {"-b", "-i", "123", "-d", "-c"}, test_active{0,1,1,1,0,0, 123});
        test(__LINE__, {"-b", "-i", "123", "-c", "-d"}, test_active{0,1,1,1,0,0, 123});
        test(__LINE__, {"-b", "-c", "-i", "123", "-d"}, test_active{0,1,1,1,0,0, 123});
        test(__LINE__, {"-b", "-c", "-d", "-i", "123"}, test_active{0,1,1,1,0,0, 123});
        test(__LINE__, {"-b", "-d", "-c", "-i", "123"}, test_active{0,1,1,1,0,0, 123});
        test(__LINE__, {"-b", "-d", "-i", "123", "-c"}, test_active{0,1,1,1,0,0, 123});

        test(__LINE__, {"-a", "-b", "-i", "123", "-d", "-c"}, test_active{1,1,1,1,0,0, 123});
        test(__LINE__, {"-a", "-b", "-i", "123", "-c", "-d"}, test_active{1,1,1,1,0,0, 123});
        test(__LINE__, {"-a", "-b", "-c", "-i", "123", "-d"}, test_active{1,1,1,1,0,0, 123});
        test(__LINE__, {"-a", "-b", "-c", "-d", "-i", "123"}, test_active{1,1,1,1,0,0, 123});
        test(__LINE__, {"-a", "-b", "-d", "-c", "-i", "123"}, test_active{1,1,1,1,0,0, 123});
        test(__LINE__, {"-a", "-b", "-d", "-i", "123", "-c"}, test_active{1,1,1,1,0,0, 123});

        test(__LINE__, {"-b", "-c", "-d"},             test_active{0,1,1,1,0,0, 0});
        test(__LINE__, {"-b", "-c", "-d", "-e"},       test_active{0,1,1,1,1,0, 0});
        test(__LINE__, {"-b", "-c", "-d", "-e", "-f"}, test_active{0,1,1,1,1,1, 0});

        test(__LINE__, {"-a", "-b", "-c", "-d"},             test_active{1,1,1,1,0,0, 0});
        test(__LINE__, {"-a", "-b", "-c", "-d", "-e"},       test_active{1,1,1,1,1,0, 0});
        test(__LINE__, {"-a", "-b", "-c", "-d", "-e", "-f"}, test_active{1,1,1,1,1,1, 0});

        test(__LINE__, {"-b", "-f"},       test_active{0,1,0,0,0,0, 0});
        test(__LINE__, {"-b", "-e", "-f"}, test_active{0,1,0,0,1,1, 0});

        test(__LINE__, {"-a", "-b", "-f"},       test_active{1,1,0,0,0,0, 0});
        test(__LINE__, {"-a", "-b", "-e", "-f"}, test_active{1,1,0,0,1,1, 0});

        test(__LINE__, {"-b", "-e", "-c"},        test_active{0,1,0,0,1,0, 0});
        test(__LINE__, {"-b", "-e", "-d"},        test_active{0,1,0,0,1,0, 0});
        test(__LINE__, {"-b", "-e", "-i", "123"}, test_active{0,1,0,0,1,0, 0});

        test(__LINE__, {"-a", "-b", "-e", "-c"},        test_active{1,1,0,0,1,0, 0});
        test(__LINE__, {"-a", "-b", "-e", "-d"},        test_active{1,1,0,0,1,0, 0});
        test(__LINE__, {"-a", "-b", "-e", "-i", "123"}, test_active{1,1,0,0,1,0, 0});

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
