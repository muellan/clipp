/*****************************************************************************
 *
 * CLIPP - command line interfaces for modern C++
 *
 * released under MIT license
 *
 * (c) 2017-2019 André Müller; foss@andremueller-online.de
 *
 *****************************************************************************/

#include "testing.h"

namespace {

//-------------------------------------------------------------------
struct test_active {
    test_active() = default;
    test_active(bool a_, int i_): a{a_}, i{i_} {}
    bool a = false;
    int i = 0;

    friend bool operator == (const test_active& x, const test_active& y) noexcept {
        return (x.a == y.a && x.i == y.i);
    }
};


//-------------------------------------------------------------------
static void
test(int lineNo,
     const std::initializer_list<const char*> args,
     const test_active& matches )
{
    using namespace clipp;
    test_active m;

    auto cli = (
        option("-a").set(m.a),
        option("-ab", "-a-b", "-a-b=") & value("i", m.i)
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli,
              [&]{ m = test_active{}; },
              [&]{ return m == matches; });
}

} // namespace

//-------------------------------------------------------------------
int TEST_MAIN()
{
    using std::string;

    try {
        test(__LINE__, {""},      test_active{0,0});
        test(__LINE__, {"-a"},    test_active{1,0});

        test(__LINE__, {"-ab"},   test_active{0,0});
        test(__LINE__, {"-a-b"},  test_active{0,0});
        test(__LINE__, {"-a-b="}, test_active{0,0});

        test(__LINE__, {"-ab",   "2"}, test_active{0,2});
        test(__LINE__, {"-a-b",  "3"}, test_active{0,3});
        test(__LINE__, {"-a-b=", "4"}, test_active{0,4});

        test(__LINE__, {"-ab2"  }, test_active{0,2});
        test(__LINE__, {"-a-b3" }, test_active{0,3});
        test(__LINE__, {"-a-b=4"}, test_active{0,4});

        test(__LINE__, {"-a", "-ab",   "2"}, test_active{1,2});
        test(__LINE__, {"-a", "-a-b",  "3"}, test_active{1,3});
        test(__LINE__, {"-a", "-a-b=", "4"}, test_active{1,4});

        test(__LINE__, {"-a", "-ab2"  }, test_active{1,2});
        test(__LINE__, {"-a", "-a-b3" }, test_active{1,3});
        test(__LINE__, {"-a", "-a-b=4"}, test_active{1,4});

        test(__LINE__, {"-ab",   "2", "-a"}, test_active{1,2});
        test(__LINE__, {"-a-b",  "3", "-a"}, test_active{1,3});
        test(__LINE__, {"-a-b=", "4", "-a"}, test_active{1,4});

        test(__LINE__, {"-a", "-ab"  }, test_active{1,0});
        test(__LINE__, {"-a", "-a-b" }, test_active{1,0});
        test(__LINE__, {"-a", "-a-b="}, test_active{1,0});

        test(__LINE__, {"-ab",   "-a"}, test_active{1,0});
        test(__LINE__, {"-a-b",  "-a"}, test_active{1,0});
        test(__LINE__, {"-a-b=", "-a"}, test_active{1,0});
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
