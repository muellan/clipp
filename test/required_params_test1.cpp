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
    test_active(bool a_, bool b_, bool c_, bool d_,
           int av_, int bv_, int cv_, int dv_):
        a{a_}, b{b_}, c{c_}, d{d_},
        av{av_}, bv{bv_}, cv{cv_}, dv{dv_}
    {}
    bool a = false, b = false, c = false, d = false;
    int av = 0, bv = 0, cv = 0, dv = 0;

    friend bool operator == (const test_active& x, const test_active& y) noexcept {
        return (x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d &&
                x.av == y.av && x.bv == y.bv && x.cv == y.cv && x.dv == y.dv );
    }

    template<class OStream>
    friend OStream& operator << (OStream& os, const test_active& x) {
        return os << '{' << x.a <<' '<< x.b <<' '<< x.c <<' '<< x.d
            << " | " << x.av <<' '<< x.bv <<' '<< x.cv <<' '<< x.dv << '}';
    }
};

//---------------------------------------------------------------
struct errors {
    errors() = default;
    explicit
    errors(bool a_, bool b_, bool c_, bool d_,
           bool av_, bool bv_, bool cv_, bool dv_):
        a{a_}, b{b_}, c{c_}, d{d_},
        av{av_}, bv{bv_}, cv{cv_}, dv{dv_}
    {}
    bool a = false, b = false, c = false, d = false;
    bool av = false, bv = false, cv = false, dv = false;

    friend bool operator == (const errors& x, const errors& y) noexcept {
        return (x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d &&
                x.av == y.av && x.bv == y.bv && x.cv == y.cv && x.dv == y.dv );
    }

    template<class OStream>
    friend OStream& operator << (OStream& os, const errors& x) {
        return os << '{' << x.a <<' '<< x.b <<' '<< x.c <<' '<< x.d
            << " | " << x.av <<' '<< x.bv <<' '<< x.cv <<' '<< x.dv << '}';
    }
};


//-------------------------------------------------------------------
void test(int lineNo,
          const std::initializer_list<const char*> args,
          const test_active& matches,
          const errors& missing)
{
    using namespace clipp;

    test_active v;
    errors m;
    auto cli = (
        option("-r") & opt_value("R")
        ,
        required("-a", "--aaa").set(v.a).if_missing(set(m.a))
            & value("I", v.av).if_missing(set(m.av))
        ,
        required("-b", "--bee").set(v.b).if_missing(set(m.b))
            & value("J", v.bv).if_missing(set(m.bv))
        ,
        option("-s") & opt_value("S")
        ,
        required("-c", "--cee").set(v.c).if_missing(set(m.c))
            & opt_value("K", v.cv).if_missing(set(m.cv))
        ,
        required("-d", "--dee").set(v.d).if_missing(set(m.d))
            & value("L", v.dv).if_missing(set(m.dv))
        ,
        option("-t") & value("T")
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli,
              [&]{ v = test_active{}; m = errors{}; },
              [&]{ return v == matches && m == missing; });
}

} // namespace

//-------------------------------------------------------------------
int TEST_MAIN()
{
    try {

        test(__LINE__, {""}, test_active{0,0,0,0, 0,0,0,0}, errors{1,1,1,1, 1,1,0,1});

        test(__LINE__, {"-a"}, test_active{1,0,0,0, 0,0,0,0}, errors{0,1,1,1, 1,1,0,1});
        test(__LINE__, {"-b"}, test_active{0,1,0,0, 0,0,0,0}, errors{1,0,1,1, 1,1,0,1});
        test(__LINE__, {"-c"}, test_active{0,0,1,0, 0,0,0,0}, errors{1,1,0,1, 1,1,0,1});
        test(__LINE__, {"-d"}, test_active{0,0,0,1, 0,0,0,0}, errors{1,1,1,0, 1,1,0,1});

        test(__LINE__, {"--aaa"}, test_active{1,0,0,0, 0,0,0,0}, errors{0,1,1,1, 1,1,0,1});
        test(__LINE__, {"--bee"}, test_active{0,1,0,0, 0,0,0,0}, errors{1,0,1,1, 1,1,0,1});
        test(__LINE__, {"--cee"}, test_active{0,0,1,0, 0,0,0,0}, errors{1,1,0,1, 1,1,0,1});
        test(__LINE__, {"--dee"}, test_active{0,0,0,1, 0,0,0,0}, errors{1,1,1,0, 1,1,0,1});

        test(__LINE__, {"-a", "2"}, test_active{1,0,0,0, 2,0,0,0}, errors{0,1,1,1, 0,1,0,1});
        test(__LINE__, {"-b", "2"}, test_active{0,1,0,0, 0,2,0,0}, errors{1,0,1,1, 1,0,0,1});
        test(__LINE__, {"-c", "2"}, test_active{0,0,1,0, 0,0,2,0}, errors{1,1,0,1, 1,1,0,1});
        test(__LINE__, {"-d", "2"}, test_active{0,0,0,1, 0,0,0,2}, errors{1,1,1,0, 1,1,0,0});

        test(__LINE__, {"--aaa", "2"}, test_active{1,0,0,0, 2,0,0,0}, errors{0,1,1,1, 0,1,0,1});
        test(__LINE__, {"--bee", "2"}, test_active{0,1,0,0, 0,2,0,0}, errors{1,0,1,1, 1,0,0,1});
        test(__LINE__, {"--cee", "2"}, test_active{0,0,1,0, 0,0,2,0}, errors{1,1,0,1, 1,1,0,1});
        test(__LINE__, {"--dee", "2"}, test_active{0,0,0,1, 0,0,0,2}, errors{1,1,1,0, 1,1,0,0});

        test(__LINE__, {"-a", "2", "-b", "3"}, test_active{1,1,0,0, 2,3,0,0}, errors{0,0,1,1, 0,0,0,1});
        test(__LINE__, {"-a", "2", "-c", "3"}, test_active{1,0,1,0, 2,0,3,0}, errors{0,1,0,1, 0,1,0,1});
        test(__LINE__, {"-a", "2", "-d", "3"}, test_active{1,0,0,1, 2,0,0,3}, errors{0,1,1,0, 0,1,0,0});
        test(__LINE__, {"-b", "2", "-c", "3"}, test_active{0,1,1,0, 0,2,3,0}, errors{1,0,0,1, 1,0,0,1});
        test(__LINE__, {"-b", "2", "-d", "3"}, test_active{0,1,0,1, 0,2,0,3}, errors{1,0,1,0, 1,0,0,0});
        test(__LINE__, {"-c", "2", "-d", "3"}, test_active{0,0,1,1, 0,0,2,3}, errors{1,1,0,0, 1,1,0,0});

        test(__LINE__, {"-b", "3", "-a", "2"}, test_active{1,1,0,0, 2,3,0,0}, errors{0,0,1,1, 0,0,0,1});
        test(__LINE__, {"-c", "3", "-a", "2"}, test_active{1,0,1,0, 2,0,3,0}, errors{0,1,0,1, 0,1,0,1});
        test(__LINE__, {"-d", "3", "-a", "2"}, test_active{1,0,0,1, 2,0,0,3}, errors{0,1,1,0, 0,1,0,0});
        test(__LINE__, {"-c", "3", "-b", "2"}, test_active{0,1,1,0, 0,2,3,0}, errors{1,0,0,1, 1,0,0,1});
        test(__LINE__, {"-d", "3", "-b", "2"}, test_active{0,1,0,1, 0,2,0,3}, errors{1,0,1,0, 1,0,0,0});
        test(__LINE__, {"-d", "3", "-c", "2"}, test_active{0,0,1,1, 0,0,2,3}, errors{1,1,0,0, 1,1,0,0});

        test(__LINE__, {"-a", "2", "-b", "3", "-c", "4"}, test_active{1,1,1,0, 2,3,4,0}, errors{0,0,0,1, 0,0,0,1});
        test(__LINE__, {"-b", "3", "-c", "4", "-d", "5"}, test_active{0,1,1,1, 0,3,4,5}, errors{1,0,0,0, 1,0,0,0});
        test(__LINE__, {"-a", "2", "-b", "3", "-d", "4"}, test_active{1,1,0,1, 2,3,0,4}, errors{0,0,1,0, 0,0,0,0});

        test(__LINE__, {"-a", "2", "-c", "4", "-b", "3"}, test_active{1,1,1,0, 2,3,4,0}, errors{0,0,0,1, 0,0,0,1});
        test(__LINE__, {"-c", "4", "-b", "3", "-d", "5"}, test_active{0,1,1,1, 0,3,4,5}, errors{1,0,0,0, 1,0,0,0});
        test(__LINE__, {"-b", "3", "-a", "2", "-d", "4"}, test_active{1,1,0,1, 2,3,0,4}, errors{0,0,1,0, 0,0,0,0});
        test(__LINE__, {"-d", "4", "-b", "3", "-a", "2"}, test_active{1,1,0,1, 2,3,0,4}, errors{0,0,1,0, 0,0,0,0});

        test(__LINE__, {"-a", "2", "-b", "3", "-c", "4", "-d", "5"}, test_active{1,1,1,1, 2,3,4,5}, errors{0,0,0,0, 0,0,0,0});
        test(__LINE__, {"-c", "4", "-b", "3", "-a", "2", "-d", "5"}, test_active{1,1,1,1, 2,3,4,5}, errors{0,0,0,0, 0,0,0,0});
        test(__LINE__, {"-d", "5", "-c", "4", "-b", "3", "-a", "2"}, test_active{1,1,1,1, 2,3,4,5}, errors{0,0,0,0, 0,0,0,0});
        test(__LINE__, {"-d", "5", "-a", "2", "-c", "4", "-b", "3"}, test_active{1,1,1,1, 2,3,4,5}, errors{0,0,0,0, 0,0,0,0});
        test(__LINE__, {"-c", "4", "-a", "2", "-d", "5", "-b", "3"}, test_active{1,1,1,1, 2,3,4,5}, errors{0,0,0,0, 0,0,0,0});

        test(__LINE__, {"-r", "R", "-a", "2", "-b", "3", "-c", "4", "-d", "5"}, test_active{1,1,1,1, 2,3,4,5}, errors{0,0,0,0, 0,0,0,0});
        test(__LINE__, {"-c", "4", "-r", "R", "-b", "3", "-a", "2", "-d", "5"}, test_active{1,1,1,1, 2,3,4,5}, errors{0,0,0,0, 0,0,0,0});
        test(__LINE__, {"-d", "5", "-c", "4", "-r", "R", "-b", "3", "-a", "2"}, test_active{1,1,1,1, 2,3,4,5}, errors{0,0,0,0, 0,0,0,0});
        test(__LINE__, {"-d", "5", "-a", "2", "-c", "4", "-r", "R", "-b", "3"}, test_active{1,1,1,1, 2,3,4,5}, errors{0,0,0,0, 0,0,0,0});
        test(__LINE__, {"-c", "4", "-a", "2", "-d", "5", "-b", "3", "-r", "R"}, test_active{1,1,1,1, 2,3,4,5}, errors{0,0,0,0, 0,0,0,0});

        test(__LINE__, {"-s", "S", "-a", "2", "-b", "3", "-c", "4", "-d", "5"}, test_active{1,1,1,1, 2,3,4,5}, errors{0,0,0,0, 0,0,0,0});
        test(__LINE__, {"-c", "4", "-s", "S", "-b", "3", "-a", "2", "-d", "5"}, test_active{1,1,1,1, 2,3,4,5}, errors{0,0,0,0, 0,0,0,0});
        test(__LINE__, {"-d", "5", "-c", "4", "-s", "S", "-b", "3", "-a", "2"}, test_active{1,1,1,1, 2,3,4,5}, errors{0,0,0,0, 0,0,0,0});
        test(__LINE__, {"-d", "5", "-a", "2", "-c", "4", "-s", "S", "-b", "3"}, test_active{1,1,1,1, 2,3,4,5}, errors{0,0,0,0, 0,0,0,0});
        test(__LINE__, {"-c", "4", "-a", "2", "-d", "5", "-b", "3", "-s", "S"}, test_active{1,1,1,1, 2,3,4,5}, errors{0,0,0,0, 0,0,0,0});

        test(__LINE__, {"-t", "T", "-a", "2", "-b", "3", "-c", "4", "-d", "5"}, test_active{1,1,1,1, 2,3,4,5}, errors{0,0,0,0, 0,0,0,0});
        test(__LINE__, {"-c", "4", "-t", "T", "-b", "3", "-a", "2", "-d", "5"}, test_active{1,1,1,1, 2,3,4,5}, errors{0,0,0,0, 0,0,0,0});
        test(__LINE__, {"-d", "5", "-c", "4", "-t", "T", "-b", "3", "-a", "2"}, test_active{1,1,1,1, 2,3,4,5}, errors{0,0,0,0, 0,0,0,0});
        test(__LINE__, {"-d", "5", "-a", "2", "-c", "4", "-t", "T", "-b", "3"}, test_active{1,1,1,1, 2,3,4,5}, errors{0,0,0,0, 0,0,0,0});
        test(__LINE__, {"-c", "4", "-a", "2", "-d", "5", "-b", "3", "-t", "T"}, test_active{1,1,1,1, 2,3,4,5}, errors{0,0,0,0, 0,0,0,0});

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
