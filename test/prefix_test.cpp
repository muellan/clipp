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


//-------------------------------------------------------------------
struct active {
    active() = default;
    active(bool a_, int i_): a{a_}, i{i_} {}
    bool a = false;
    int i = 0;

    friend bool operator == (const active& x, const active& y) noexcept {
        return (x.a == y.a && x.i == y.i);
    }
};


//-------------------------------------------------------------------
static void
test(int lineNo,
     const std::initializer_list<const char*> args,
     const active& matches )
{
    using namespace clipp;
    active m;

    auto cli = (
        option("-a").set(m.a),
        option("-ab", "-a-b", "-a-b=") & value("i", m.i)
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli,
              [&]{ m = active{}; },
              [&]{ return m == matches; });
}


//-------------------------------------------------------------------
int main()
{
    using std::string;

    try {
        test(__LINE__, {""},      active{0,0});
        test(__LINE__, {"-a"},    active{1,0});

        test(__LINE__, {"-ab"},   active{0,0});
        test(__LINE__, {"-a-b"},  active{0,0});
        test(__LINE__, {"-a-b="}, active{0,0});

        test(__LINE__, {"-ab",   "2"}, active{0,2});
        test(__LINE__, {"-a-b",  "3"}, active{0,3});
        test(__LINE__, {"-a-b=", "4"}, active{0,4});

        test(__LINE__, {"-ab2"  }, active{0,2});
        test(__LINE__, {"-a-b3" }, active{0,3});
        test(__LINE__, {"-a-b=4"}, active{0,4});

        test(__LINE__, {"-a", "-ab",   "2"}, active{1,2});
        test(__LINE__, {"-a", "-a-b",  "3"}, active{1,3});
        test(__LINE__, {"-a", "-a-b=", "4"}, active{1,4});

        test(__LINE__, {"-a", "-ab2"  }, active{1,2});
        test(__LINE__, {"-a", "-a-b3" }, active{1,3});
        test(__LINE__, {"-a", "-a-b=4"}, active{1,4});

        test(__LINE__, {"-ab",   "2", "-a"}, active{1,2});
        test(__LINE__, {"-a-b",  "3", "-a"}, active{1,3});
        test(__LINE__, {"-a-b=", "4", "-a"}, active{1,4});

        test(__LINE__, {"-a", "-ab"  }, active{1,0});
        test(__LINE__, {"-a", "-a-b" }, active{1,0});
        test(__LINE__, {"-a", "-a-b="}, active{1,0});

        test(__LINE__, {"-ab",   "-a"}, active{1,0});
        test(__LINE__, {"-a-b",  "-a"}, active{1,0});
        test(__LINE__, {"-a-b=", "-a"}, active{1,0});
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
