/*****************************************************************************
 *
 * CLIPP - command line interfaces for modern C++
 *
 * released under MIT license
 *
 * (c) 2017-2018-2018 André Müller; foss@andremueller-online.de
 *
 *****************************************************************************/

#include "testing.h"


//-------------------------------------------------------------------
struct test_active {
    test_active() = default;
    explicit
    test_active(bool o_, std::string ov_ = "_", std::string pv_ = "_"):
        o{o_}, ov{std::move(ov_)}, pv{std::move(pv_)}
    {}

    bool o = false;
    std::string ov = "_";
    std::string pv = "_";

    friend bool operator == (const test_active& x, const test_active& y) noexcept {
        return (x.o == y.o && x.ov == y.ov && x.pv == y.pv);
    }
};


//-------------------------------------------------------------------
void test_empty(int lineNo,
                const std::initializer_list<const char*> args,
                const test_active& matches)
{
    using namespace clipp;

    test_active m;
    auto cli = (
      option("-o", "--opt").set(m.o) & value(match::any, "O", m.ov),
      value("P", m.pv)
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli,
              [&]{ m = test_active{}; },
              [&]{ return m == matches; });

}


//-------------------------------------------------------------------
void test_nonempty(int lineNo,
                   const std::initializer_list<const char*> args,
                   const test_active& matches)
{
    using namespace clipp;

    test_active m;
    auto cli = (
      option("-o", "--opt").set(m.o) & value(match::nonempty, "O", m.ov),
      value("P", m.pv)
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli,
              [&]{ m = test_active{}; },
              [&]{ return m == matches; });

}


//-------------------------------------------------------------------
int TEST_MAIN()
{
    try {
        test_empty(__LINE__, {}, test_active{});
        test_empty(__LINE__, {""}, test_active{});
        test_empty(__LINE__, {"-o"}, test_active{true});
        test_empty(__LINE__, {"-o", ""}, test_active{true, ""});
        test_empty(__LINE__, {"-o", "X"}, test_active{true, "X"});
        test_empty(__LINE__, {"X"}, test_active{false, "_", "X"});
        test_empty(__LINE__, {"-o", "", "X"}, test_active{true, "", "X"});


        test_nonempty(__LINE__, {}, test_active{});
        test_nonempty(__LINE__, {""}, test_active{});
        test_nonempty(__LINE__, {"-o"}, test_active{true});
        test_nonempty(__LINE__, {"-o", ""}, test_active{true});
        test_nonempty(__LINE__, {"-o", "X"}, test_active{true, "X"});
        test_nonempty(__LINE__, {"X"}, test_active{false, "_", "X"});

        //ambiguous -> cannot map to value "P", expects value "O" first
        test_nonempty(__LINE__, {"-o", "", "X"}, test_active{true});

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
