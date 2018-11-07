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
#include <cmath>


//-------------------------------------------------------------------
struct active {
    active() = default;

    explicit
    active(std::initializer_list<double> il): xs{il} {}

    std::vector<double> xs;

    friend bool operator == (const active& a, const active& b) noexcept {
        return std::equal(begin(a.xs), end(a.xs), begin(b.xs),
            [](double x, double y) {
                return std::abs(x - y) < 1e-5;
            });
    }
};


//-------------------------------------------------------------------
void test(int lineNo,
          const std::initializer_list<const char*> args,
          const active& matches)
{
    using namespace clipp;

    active m;
    auto cli = joinable( repeatable(
                   option(",") , opt_number("number", m.xs)
               ) );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli,
              [&]{ m = active{}; },
              [&]{ return m == matches; });
}


//-------------------------------------------------------------------
int main()
{
    try {
        test(__LINE__, {""}, active{});

        test(__LINE__, {"1"          }, active{1});
        test(__LINE__, {"1", "2"     }, active{1,2});
        test(__LINE__, {"1", "2", "3"}, active{1,2,3});
        test(__LINE__, {"1.1"              }, active{1.1});
        test(__LINE__, {"1.1", "2.2"       }, active{1.1,2.2});
        test(__LINE__, {"1.1", "2.2", "3.3"}, active{1.1,2.2,3.3});

        test(__LINE__, {"1"          }, active{1});
        test(__LINE__, {"1", ",", "2"     }, active{1,2});
        test(__LINE__, {"1", ",", "2", ",", "3"}, active{1,2,3});
        test(__LINE__, {"1.1"              }, active{1.1});
        test(__LINE__, {"1.1", ",", "2.2"       }, active{1.1,2.2});
        test(__LINE__, {"1.1", ",", "2.2", ",", "3.3"}, active{1.1,2.2,3.3});

        test(__LINE__, {"1,"          }, active{1});
        test(__LINE__, {"1,", "2"     }, active{1,2});
        test(__LINE__, {"1,", "2", "3"}, active{1,2,3});
        test(__LINE__, {"1.1,"              }, active{1.1});
        test(__LINE__, {"1.1,", "2.2"       }, active{1.1,2.2});
        test(__LINE__, {"1.1,", "2.2", "3.3"}, active{1.1,2.2,3.3});

        test(__LINE__, {"1,"          }, active{1});
        test(__LINE__, {"1,", "2,"     }, active{1,2});
        test(__LINE__, {"1,", "2,", "3"}, active{1,2,3});
        test(__LINE__, {"1.1,"              }, active{1.1});
        test(__LINE__, {"1.1,", "2.2,"       }, active{1.1,2.2});
        test(__LINE__, {"1.1,", "2.2,", "3.3"}, active{1.1,2.2,3.3});

        test(__LINE__, {"1,"          }, active{1});
        test(__LINE__, {"1,", "2,"     }, active{1,2});
        test(__LINE__, {"1,", "2,", "3,"}, active{1,2,3});
        test(__LINE__, {"1.1,"              }, active{1.1});
        test(__LINE__, {"1.1,", "2.2,"       }, active{1.1,2.2});
        test(__LINE__, {"1.1,", "2.2,", "3.3,"}, active{1.1,2.2,3.3});

        test(__LINE__, {"1"          }, active{1});
        test(__LINE__, {"1", ",2"     }, active{1,2});
        test(__LINE__, {"1", ",2", "3"}, active{1,2,3});
        test(__LINE__, {"1.1"              }, active{1.1});
        test(__LINE__, {"1.1", ",2.2"       }, active{1.1,2.2});
        test(__LINE__, {"1.1", ",2.2", "3.3"}, active{1.1,2.2,3.3});

        test(__LINE__, {"1"          }, active{1});
        test(__LINE__, {"1", ",2"     }, active{1,2});
        test(__LINE__, {"1", ",2", ",3"}, active{1,2,3});
        test(__LINE__, {"1.1"              }, active{1.1});
        test(__LINE__, {"1.1", ",2.2"       }, active{1.1,2.2});
        test(__LINE__, {"1.1", ",2.2", ",3.3"}, active{1.1,2.2,3.3});

        test(__LINE__, {"1"          }, active{1});
        test(__LINE__, {"1,2", "3"}, active{1,2,3});
        test(__LINE__, {"1.1,2.2"       }, active{1.1,2.2});
        test(__LINE__, {"1.1", "2.2", "3.3"}, active{1.1,2.2,3.3});

        test(__LINE__, {"1", "2,3"}, active{1,2,3});

        test(__LINE__, {"1.1"              }, active{1.1});
        test(__LINE__, {"1.1", "2.2"       }, active{1.1,2.2});
        test(__LINE__, {"1.1", "2.2,3.3"}, active{1.1,2.2,3.3});

        test(__LINE__, {"1"          }, active{1});
        test(__LINE__, {"1,2"     }, active{1,2});
        test(__LINE__, {"1,2,3"}, active{1,2,3});
        test(__LINE__, {"1.1"              }, active{1.1});
        test(__LINE__, {"1.1,2.2"       }, active{1.1,2.2});
        test(__LINE__, {"1.1,2.2,3.3"}, active{1.1,2.2,3.3});

        test(__LINE__, {"1", "2,3", "4", ",", "5", "6,7,8"}, active{1,2,3,4,5,6,7,8});
        test(__LINE__, {"1", "2.2,3", "4.4", ",", "5.5", "6,7,8"}, active{1,2.2,3,4.4,5.5,6,7,8});

        test(__LINE__, {","}, active{});
        test(__LINE__, {"1", ","}, active{1});
        test(__LINE__, {"1", ",", "2", ","}, active{1,2});

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
