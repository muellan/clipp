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
    test_active(bool n_, std::initializer_list<int> nums_,
           bool s_, const std::string& str_)
    :
           str{str_}, nums{nums_}, n{n_}, s{s_}
    {}

    std::string str;
    std::vector<int> nums;
    bool n = false, s = false;

    friend bool operator == (const test_active& x, const test_active& y) noexcept {
        return x.n == y.n && x.s == y.s && x.str == y.str &&
               std::equal(x.nums.begin(), x.nums.end(), y.nums.begin());
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
        option("-n", "--num").set(m.n) & integers("nums", m.nums),
        option("-s", "--str").set(m.s) & value("str", m.str)
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli,
              [&]{ m = test_active{}; },
              [&]{ return m == matches; });
}

} // namespace

//-------------------------------------------------------------------
int TEST_MAIN()
{
    try {
        test(__LINE__, {""}, test_active{});


        test(__LINE__, {"-n"},                 test_active{true, {}, false, ""});
        test(__LINE__, {"-n", "1"},            test_active{true, {1}, false, ""});
        test(__LINE__, {"-n", "1", "5"},       test_active{true, {1,5}, false, ""});
        test(__LINE__, {"-n", "1", "-3", "4"}, test_active{true, {1,-3,4}, false, ""});

        test(__LINE__, {"-s"},        test_active{false, {}, true, ""});
        test(__LINE__, {"-s", "xyz"}, test_active{false, {}, true, "xyz"});

        test(__LINE__, {"-n", "1",       "ab"}, test_active{true, {1}, false, ""});
        test(__LINE__, {"-n", "1", "2",  "ab"}, test_active{true, {1,2}, false, ""});
        test(__LINE__, {"-n", "1", "-3", "ab"}, test_active{true, {1,-3}, false, ""});

        test(__LINE__, {"-n", "1",       "-s", "ab"}, test_active{true, {1}, true, "ab"});
        test(__LINE__, {"-n", "1", "2",  "-s", "ab"}, test_active{true, {1,2}, true, "ab"});
        test(__LINE__, {"-n", "1", "-3", "-s", "ab"}, test_active{true, {1,-3}, true, "ab"});

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
