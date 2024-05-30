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
    test_active(bool a_, bool b_, bool c_, bool d_):
        a{a_}, b{b_}, c{c_}, d{d_}
    {}
    bool a = false, b = false, c = false, d = false;
    bool conflict = false;

    friend bool operator == (const test_active& x, const test_active& y) noexcept {
        return (x.a == y.a && x.b == y.b && x.c == y.c &&
                x.d == y.d && x.conflict == y.conflict);
    }
};


//-------------------------------------------------------------------
void test(int lineNo,
          const std::initializer_list<const char*> args,
          const test_active& matches)
{
    using namespace clipp;

    {
        test_active m;

        auto cli = option("-a").set(m.a) |
                   option("-b").set(m.b) |
                   option("-c").set(m.c) |
                   option("-d").set(m.d);

        auto res = parse(args, cli);
        m.conflict = res.any_conflict();

        if(!(m == matches)) {
            throw std::runtime_error{"failed " + std::string( __FILE__ ) +
                                     " #1 in line " + std::to_string(lineNo)};
        }
    }
    {
        test_active m;

        auto cli = (
            option("?????"),
            (
                option("-a").set(m.a) |
                option("-b").set(m.b) |
                option("-c").set(m.c) |
                option("-d").set(m.d)
            )
        );

        auto res = parse(args, cli);
        m.conflict = res.any_conflict();

        if(!(m == matches)) {
            throw std::runtime_error{"failed " + std::string( __FILE__ ) +
                                     " #2 in line " + std::to_string(lineNo)};
        }
    }
}

} // namespace

//-------------------------------------------------------------------
int TEST_MAIN()
{
    try {
        test(__LINE__, {""}, test_active{});

        test(__LINE__, {"-a"}, test_active{1,0,0,0});
        test(__LINE__, {"-b"}, test_active{0,1,0,0});
        test(__LINE__, {"-c"}, test_active{0,0,1,0});
        test(__LINE__, {"-d"}, test_active{0,0,0,1});

        {
            test_active e; e.conflict = true;
            test(__LINE__, {"-a","-b"}, e);
            test(__LINE__, {"-b","-a"}, e);
            test(__LINE__, {"-c","-b"}, e);
            test(__LINE__, {"-d","-c"}, e);
            test(__LINE__, {"-a","-d"}, e);

            test(__LINE__, {"-a","-b","-c"}, e);
            test(__LINE__, {"-c","-b","-a"}, e);
            test(__LINE__, {"-d","-a","-c"}, e);

            test(__LINE__, {"-a","-b","-c", "-d"}, e);
            test(__LINE__, {"-d","-c","-b", "-a"}, e);
            test(__LINE__, {"-c","-b","-a", "-d"}, e);
            test(__LINE__, {"-d","-a","-c", "-b"}, e);
        }
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
