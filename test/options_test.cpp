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
    active(bool a_, bool b_, bool c_, bool d_, bool e_, bool f_):
        a{a_}, b{b_}, c{c_}, d{d_}, e{e_}, f{f_}
    {}
    bool a = false, b = false, c = false, d = false, e = false, f = false;

    friend bool operator == (const active& x, const active& y) noexcept {
        return (x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d &&
                x.e == y.e && x.f == y.f);
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
        option("-a", "--aaa").set(m.a),
        option("-b", "--bee").set(m.b),
        option("-c", "--cee").set(m.c),
        option("-d", "--dee").set(m.d),
        option("-e", "--eee").set(m.e),
        option("-f", "--eff").set(m.f) );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli,
              [&]{ m = active{}; },
              [&]{ return m == matches; });
}


//-------------------------------------------------------------------
int main()
{
    try {
        test(__LINE__, {""}, active{});
        //single option presence - primary flags
        test(__LINE__, {"-a"}, active{1,0,0,0,0,0});
        test(__LINE__, {"-b"}, active{0,1,0,0,0,0});
        test(__LINE__, {"-c"}, active{0,0,1,0,0,0});
        test(__LINE__, {"-d"}, active{0,0,0,1,0,0});
        test(__LINE__, {"-e"}, active{0,0,0,0,1,0});
        test(__LINE__, {"-f"}, active{0,0,0,0,0,1});

        //single option presence - secondary flags
        test(__LINE__, {"--aaa"}, active{1,0,0,0,0,0});
        test(__LINE__, {"--bee"}, active{0,1,0,0,0,0});
        test(__LINE__, {"--cee"}, active{0,0,1,0,0,0});
        test(__LINE__, {"--dee"}, active{0,0,0,1,0,0});
        test(__LINE__, {"--eee"}, active{0,0,0,0,1,0});
        test(__LINE__, {"--eff"}, active{0,0,0,0,0,1});

        //all (varying sequence)
        test(__LINE__, {"-a", "-b", "-c", "-d", "-e", "-f"}, active{1,1,1,1,1,1});
        test(__LINE__, {"-c", "-d", "-a", "-f", "-e", "-b"}, active{1,1,1,1,1,1});

        test(__LINE__, {"--aaa", "--bee", "--cee", "--dee", "--eee", "--eff"},
                       active{1,1,1,1,1,1});

        test(__LINE__, {"--eee", "--bee", "--eff", "--dee", "--aaa", "--cee"},
                       active{1,1,1,1,1,1});

        //several options
        test(__LINE__, {"-b", "--aaa", "-f", "--dee"}, active{1,1,0,1,0,1});
        test(__LINE__, {"-f", "-a", "-b", "--dee"}, active{1,1,0,1,0,1});

        //option repeat is ignored
        test(__LINE__, {"-b", "-a", "--bee", "--aaa"}, active{1,1,0,0,0,0});

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
