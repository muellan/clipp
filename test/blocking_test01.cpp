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
    active(bool a_, bool b_, bool c_, bool d_, bool e_, bool f_, int i_ = 0) :
        a{a_}, b{b_}, c{c_}, d{d_}, e{e_}, f{f_}, i{i_}
    {}
    bool a = false, b = false, c = false, d = false, e = false, f = false;
    int i = 0;

    friend bool operator == (const active& x, const active& y) noexcept {
        return (x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d &&
                x.e == y.e && x.f == y.f && x.i == y.i);
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


//-------------------------------------------------------------------
int main()
{
    try {
        test(__LINE__, {""}, active{});

        test(__LINE__, {"-a"}, active{1,0,0,0,0,0, 0});

        test(__LINE__, {"-a", "-c"}, active{1,0,0,0,0,0, 0});
        test(__LINE__, {"-a", "-d"}, active{1,0,0,0,0,0, 0});
        test(__LINE__, {"-a", "-e"}, active{1,0,0,0,0,0, 0});
        test(__LINE__, {"-a", "-f"}, active{1,0,0,0,0,0, 0});
        test(__LINE__, {"-a", "-i", "123"}, active{1,0,0,0,0,0, 0});

        test(__LINE__, {"-b"}, active{0,1,0,0,0,0, 0});
        test(__LINE__, {"-b", "-a"}, active{0,1,0,0,0,0, 0});
        test(__LINE__, {"-a", "-b"}, active{1,1,0,0,0,0, 0});

        test(__LINE__, {"-b", "-c"}, active{0,1,1,0,0,0, 0});
        test(__LINE__, {"-a", "-b", "-c"}, active{1,1,1,0,0,0, 0});
        test(__LINE__, {"-a", "-b", "-c", "-i"}, active{1,1,1,0,0,0, 0});
        test(__LINE__, {"-a", "-b", "-c", "-i", "123"}, active{1,1,1,0,0,0, 123});

        test(__LINE__, {"-b", "-i", "123"},             active{0,1,0,0,0,0, 123});
        test(__LINE__, {"-a", "-b", "-i", "123"},       active{1,1,0,0,0,0, 123});
        test(__LINE__, {"-a", "-b", "-c", "-i", "123"}, active{1,1,1,0,0,0, 123});
        test(__LINE__, {"-a", "-b", "-d", "-i", "123"}, active{1,1,0,1,0,0, 123});
        test(__LINE__, {"-a", "-b", "-i", "123", "-c"}, active{1,1,1,0,0,0, 123});
        test(__LINE__, {"-a", "-b", "-i", "123", "-d"}, active{1,1,0,1,0,0, 123});

        test(__LINE__, {"-b", "-i", "123", "-a"}, active{0,1,0,0,0,0, 123});
        test(__LINE__, {"-b", "-i", "123", "-a"}, active{0,1,0,0,0,0, 123});

        test(__LINE__, {"-b", "-i", "123", "-d", "-c"}, active{0,1,1,1,0,0, 123});
        test(__LINE__, {"-b", "-i", "123", "-c", "-d"}, active{0,1,1,1,0,0, 123});
        test(__LINE__, {"-b", "-c", "-i", "123", "-d"}, active{0,1,1,1,0,0, 123});
        test(__LINE__, {"-b", "-c", "-d", "-i", "123"}, active{0,1,1,1,0,0, 123});
        test(__LINE__, {"-b", "-d", "-c", "-i", "123"}, active{0,1,1,1,0,0, 123});
        test(__LINE__, {"-b", "-d", "-i", "123", "-c"}, active{0,1,1,1,0,0, 123});

        test(__LINE__, {"-a", "-b", "-i", "123", "-d", "-c"}, active{1,1,1,1,0,0, 123});
        test(__LINE__, {"-a", "-b", "-i", "123", "-c", "-d"}, active{1,1,1,1,0,0, 123});
        test(__LINE__, {"-a", "-b", "-c", "-i", "123", "-d"}, active{1,1,1,1,0,0, 123});
        test(__LINE__, {"-a", "-b", "-c", "-d", "-i", "123"}, active{1,1,1,1,0,0, 123});
        test(__LINE__, {"-a", "-b", "-d", "-c", "-i", "123"}, active{1,1,1,1,0,0, 123});
        test(__LINE__, {"-a", "-b", "-d", "-i", "123", "-c"}, active{1,1,1,1,0,0, 123});

        test(__LINE__, {"-b", "-c", "-d"},             active{0,1,1,1,0,0, 0});
        test(__LINE__, {"-b", "-c", "-d", "-e"},       active{0,1,1,1,1,0, 0});
        test(__LINE__, {"-b", "-c", "-d", "-e", "-f"}, active{0,1,1,1,1,1, 0});

        test(__LINE__, {"-a", "-b", "-c", "-d"},             active{1,1,1,1,0,0, 0});
        test(__LINE__, {"-a", "-b", "-c", "-d", "-e"},       active{1,1,1,1,1,0, 0});
        test(__LINE__, {"-a", "-b", "-c", "-d", "-e", "-f"}, active{1,1,1,1,1,1, 0});

        test(__LINE__, {"-b", "-f"},       active{0,1,0,0,0,0, 0});
        test(__LINE__, {"-b", "-e", "-f"}, active{0,1,0,0,1,1, 0});

        test(__LINE__, {"-a", "-b", "-f"},       active{1,1,0,0,0,0, 0});
        test(__LINE__, {"-a", "-b", "-e", "-f"}, active{1,1,0,0,1,1, 0});

        test(__LINE__, {"-b", "-e", "-c"},        active{0,1,0,0,1,0, 0});
        test(__LINE__, {"-b", "-e", "-d"},        active{0,1,0,0,1,0, 0});
        test(__LINE__, {"-b", "-e", "-i", "123"}, active{0,1,0,0,1,0, 0});

        test(__LINE__, {"-a", "-b", "-e", "-c"},        active{1,1,0,0,1,0, 0});
        test(__LINE__, {"-a", "-b", "-e", "-d"},        active{1,1,0,0,1,0, 0});
        test(__LINE__, {"-a", "-b", "-e", "-i", "123"}, active{1,1,0,0,1,0, 0});

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
