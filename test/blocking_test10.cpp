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
    active(bool n_, std::initializer_list<int> nums_,
           bool s_, const std::string& str_)
    :
           str{str_}, nums{nums_}, n{n_}, s{s_}
    {}

    std::string str;
    std::vector<int> nums;
    bool n = false, s = false;

    friend bool operator == (const active& x, const active& y) noexcept {
        return x.n == y.n && x.s == y.s && x.str == y.str &&
               std::equal(x.nums.begin(), x.nums.end(), y.nums.begin());
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
        option("-n", "--num").set(m.n) & integers("nums", m.nums),
        option("-s", "--str").set(m.s) & value("str", m.str)
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


        test(__LINE__, {"-n"},                 active{true, {}, false, ""});
        test(__LINE__, {"-n", "1"},            active{true, {1}, false, ""});
        test(__LINE__, {"-n", "1", "5"},       active{true, {1,5}, false, ""});
        test(__LINE__, {"-n", "1", "-3", "4"}, active{true, {1,-3,4}, false, ""});

        test(__LINE__, {"-s"},        active{false, {}, true, ""});
        test(__LINE__, {"-s", "xyz"}, active{false, {}, true, "xyz"});

        test(__LINE__, {"-n", "1",       "ab"}, active{true, {1}, false, ""});
        test(__LINE__, {"-n", "1", "2",  "ab"}, active{true, {1,2}, false, ""});
        test(__LINE__, {"-n", "1", "-3", "ab"}, active{true, {1,-3}, false, ""});

        test(__LINE__, {"-n", "1",       "-s", "ab"}, active{true, {1}, true, "ab"});
        test(__LINE__, {"-n", "1", "2",  "-s", "ab"}, active{true, {1,2}, true, "ab"});
        test(__LINE__, {"-n", "1", "-3", "-s", "ab"}, active{true, {1,-3}, true, "ab"});

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
