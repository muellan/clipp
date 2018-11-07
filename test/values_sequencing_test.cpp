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
    active(bool a_,
           bool ie_, bool je_, bool ke_, bool ve_,
           int i_, int j_, int k_, std::initializer_list<int> v_)
    :
        a{a_},
        ie{ie_}, je{je_}, ke{ke_}, ve{ve_},
        i{i_}, j{j_}, k{k_}, v{v_}
    {}

    //option presence
    bool a = false;
    //requirement error
    bool ie = false, je = false, ke = false, ve = false;
    //values
    int i = 0, j = 0, k = 0;
    std::vector<int> v;

    friend bool operator == (const active& x, const active& y) noexcept {
        return (x.a == y.a && x.ie == y.ie && x.je == y.je &&
                x.ke == y.ke && x.ve == y.ve && x.i == y.i &&
                x.j == y.j && x.k == y.k &&
                std::equal(begin(x.v), end(x.v), begin(y.v)));
    }

    template<class OStream>
    friend OStream& operator << (OStream& os, const active& x) {
        return os << x.a <<' '<< x.ie <<' '<< x.je <<' '<< x.ke <<' '
                  << x.ve <<' '<< x.i <<' '<< x.j <<' '<< x.k;
    }
};


//-------------------------------------------------------------------
void test(int lineNo,
          const std::initializer_list<const char*> args,
          const active& matches)
{
    using namespace clipp;

    active m;

    auto cli = group(
        option("-a").set(m.a) & opt_value("i", m.i).if_missing(set(m.ie))
        ,
        option("-b").set(m.a) & opt_value("i", m.i).if_missing(set(m.ie))
                              & opt_value("j", m.j).if_missing(set(m.je))
        ,
        option("-c").set(m.a) & opt_value("i", m.i).if_missing(set(m.ie))
                              & opt_value("j", m.j).if_missing(set(m.je))
                              & opt_value("k", m.k).if_missing(set(m.ke))
        ,
        option("-d").set(m.a) & opt_values("v", m.v).if_missing(set(m.ve))
        ,
        option("-e").set(m.a) & opt_value("i", m.i).if_missing(set(m.ie))
                              & opt_values("v", m.v).if_missing(set(m.ve))
        ,
        option("-f").set(m.a) & opt_value("i", m.i).if_missing(set(m.ie))
                              & opt_value("j", m.j).if_missing(set(m.je))
                              & opt_values("v", m.v)
        ,
        option("-k").set(m.a) & value("i", m.i).if_missing(set(m.ie))
        ,
        option("-l").set(m.a) & value("i", m.i).if_missing(set(m.ie))
                              & opt_value("j", m.j).if_missing(set(m.je))
        ,
        option("-m").set(m.a) & value("i", m.i).if_missing(set(m.ie))
                              & opt_value("j", m.j).if_missing(set(m.je))
                              & opt_value("k", m.k)
        ,
        option("-n").set(m.a) & values("v", m.v).if_missing(set(m.ve))
        ,
        option("-o").set(m.a) & value("i", m.i).if_missing(set(m.ie))
                              & opt_values("v", m.v).if_missing(set(m.ve))
        ,
        option("-p").set(m.a) & value("i", m.i).if_missing(set(m.ie))
                              & value("j", m.j).if_missing(set(m.je))
                              & opt_values("v", m.v).if_missing(set(m.ve))
        ,
        option("-q").set(m.a) & value("i", m.i).if_missing(set(m.ie))
                              & value("j", m.j).if_missing(set(m.je))
                              & values("v", m.v).if_missing(set(m.ve))
        ,
        option("-r").set(m.a) & value("i", m.i).if_missing(set(m.ie))
                              & value("j", m.j).if_missing(set(m.je))
        ,
        option("-s").set(m.a) & value("i", m.i).if_missing(set(m.ie))
                              & value("j", m.j).if_missing(set(m.je))
                              & opt_value("k", m.k).if_missing(set(m.ke))
        ,
        option("-t").set(m.a) & value("i", m.i).if_missing(set(m.ie))
                              & value("j", m.j).if_missing(set(m.je))
                              & value("k", m.k).if_missing(set(m.ke))
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli,
              [&]{ m = active{}; },
              [&]{ return m == matches; });
}


//-------------------------------------------------------------------
int main()
{
    try {
        test(__LINE__, {""},   active{});

        test(__LINE__, {"-a"},      active{1, 0,0,0,0, 0,0,0,{}});
        test(__LINE__, {"-a", "2"}, active{1, 0,0,0,0, 2,0,0,{}});

        test(__LINE__, {"-b"},           active{1, 0,0,0,0, 0,0,0,{}});
        test(__LINE__, {"-b", "2"},      active{1, 0,0,0,0, 2,0,0,{}});
        test(__LINE__, {"-b", "2", "3"}, active{1, 0,0,0,0, 2,3,0,{}});

        test(__LINE__, {"-c"},                active{1, 0,0,0,0, 0,0,0,{}});
        test(__LINE__, {"-c", "2"},           active{1, 0,0,0,0, 2,0,0,{}});
        test(__LINE__, {"-c", "2", "3" },     active{1, 0,0,0,0, 2,3,0,{}});
        test(__LINE__, {"-c", "2", "3", "4"}, active{1, 0,0,0,0, 2,3,4,{}});

        test(__LINE__, {"-d"},                     active{1, 0,0,0,0, 0,0,0,{}});
        test(__LINE__, {"-d", "2"},                active{1, 0,0,0,0, 0,0,0,{2}});
        test(__LINE__, {"-d", "2", "3"},           active{1, 0,0,0,0, 0,0,0,{2,3}});
        test(__LINE__, {"-d", "2", "3", "4"},      active{1, 0,0,0,0, 0,0,0,{2,3,4}});
        test(__LINE__, {"-d", "2", "3", "4", "5"}, active{1, 0,0,0,0, 0,0,0,{2,3,4, 5}});

        test(__LINE__, {"-c", "2", "-d", "3"}, active{1, 0,0,0,0, 2,0,0,{3}});
        test(__LINE__, {"-d", "3", "-c", "2"}, active{1, 0,0,0,0, 2,0,0,{3}});

        test(__LINE__, {"-e"},                     active{1, 0,0,0,0, 0,0,0,{}});
        test(__LINE__, {"-e", "2"},                active{1, 0,0,0,0, 2,0,0,{}});
        test(__LINE__, {"-e", "2", "3"},           active{1, 0,0,0,0, 2,0,0,{3}});
        test(__LINE__, {"-e", "2", "3", "4"},      active{1, 0,0,0,0, 2,0,0,{3,4}});
        test(__LINE__, {"-e", "2", "3", "4", "5"}, active{1, 0,0,0,0, 2,0,0,{3,4, 5}});

        test(__LINE__, {"-f"},                     active{1, 0,0,0,0, 0,0,0,{}});
        test(__LINE__, {"-f", "2"},                active{1, 0,0,0,0, 2,0,0,{}});
        test(__LINE__, {"-f", "2", "3"},           active{1, 0,0,0,0, 2,3,0,{}});
        test(__LINE__, {"-f", "2", "3", "4"},      active{1, 0,0,0,0, 2,3,0,{4}});
        test(__LINE__, {"-f", "2", "3", "4", "5"}, active{1, 0,0,0,0, 2,3,0,{4, 5}});

        test(__LINE__, {"-k"},      active{1, 1,0,0,0, 0,0,0,{}});
        test(__LINE__, {"-k", "2"}, active{1, 0,0,0,0, 2,0,0,{}});

        test(__LINE__, {"-l"},           active{1, 1,0,0,0, 0,0,0,{}});
        test(__LINE__, {"-l", "2"},      active{1, 0,0,0,0, 2,0,0,{}});
        test(__LINE__, {"-l", "2", "3"}, active{1, 0,0,0,0, 2,3,0,{}});

        test(__LINE__, {"-m"},                active{1, 1,0,0,0, 0,0,0,{}});
        test(__LINE__, {"-m", "2"},           active{1, 0,0,0,0, 2,0,0,{}});
        test(__LINE__, {"-m", "2", "3" },     active{1, 0,0,0,0, 2,3,0,{}});
        test(__LINE__, {"-m", "2", "3", "4"}, active{1, 0,0,0,0, 2,3,4,{}});

        test(__LINE__, {"-n"},                     active{1, 0,0,0,1, 0,0,0,{}});
        test(__LINE__, {"-n", "2"},                active{1, 0,0,0,0, 0,0,0,{2}});
        test(__LINE__, {"-n", "2", "3"},           active{1, 0,0,0,0, 0,0,0,{2,3}});
        test(__LINE__, {"-n", "2", "3", "4"},      active{1, 0,0,0,0, 0,0,0,{2,3,4}});
        test(__LINE__, {"-n", "2", "3", "4", "5"}, active{1, 0,0,0,0, 0,0,0,{2,3,4, 5}});

        test(__LINE__, {"-o"},                     active{1, 1,0,0,0, 0,0,0,{}});
        test(__LINE__, {"-o", "2"},                active{1, 0,0,0,0, 2,0,0,{}});
        test(__LINE__, {"-o", "2", "3"},           active{1, 0,0,0,0, 2,0,0,{3}});
        test(__LINE__, {"-o", "2", "3", "4"},      active{1, 0,0,0,0, 2,0,0,{3,4}});
        test(__LINE__, {"-o", "2", "3", "4", "5"}, active{1, 0,0,0,0, 2,0,0,{3,4, 5}});

        test(__LINE__, {"-p"},                     active{1, 1,1,0,0, 0,0,0,{}});
        test(__LINE__, {"-p", "2"},                active{1, 0,1,0,0, 2,0,0,{}});
        test(__LINE__, {"-p", "2", "3"},           active{1, 0,0,0,0, 2,3,0,{}});
        test(__LINE__, {"-p", "2", "3", "4"},      active{1, 0,0,0,0, 2,3,0,{4}});
        test(__LINE__, {"-p", "2", "3", "4", "5"}, active{1, 0,0,0,0, 2,3,0,{4, 5}});

        test(__LINE__, {"-q"},                     active{1, 1,1,0,1, 0,0,0,{}});
        test(__LINE__, {"-q", "2"},                active{1, 0,1,0,1, 2,0,0,{}});
        test(__LINE__, {"-q", "2", "3"},           active{1, 0,0,0,1, 2,3,0,{}});
        test(__LINE__, {"-q", "2", "3", "4"},      active{1, 0,0,0,0, 2,3,0,{4}});
        test(__LINE__, {"-q", "2", "3", "4", "5"}, active{1, 0,0,0,0, 2,3,0,{4, 5}});

        test(__LINE__, {"-r"},           active{1, 1,1,0,0, 0,0,0,{}});
        test(__LINE__, {"-r", "2"},      active{1, 0,1,0,0, 2,0,0,{}});
        test(__LINE__, {"-r", "2", "3"}, active{1, 0,0,0,0, 2,3,0,{}});

        test(__LINE__, {"-s"},                active{1, 1,1,0,0, 0,0,0,{}});
        test(__LINE__, {"-s", "2"},           active{1, 0,1,0,0, 2,0,0,{}});
        test(__LINE__, {"-s", "2", "3" },     active{1, 0,0,0,0, 2,3,0,{}});
        test(__LINE__, {"-s", "2", "3", "4"}, active{1, 0,0,0,0, 2,3,4,{}});

        test(__LINE__, {"-t"},                active{1, 1,1,1,0, 0,0,0,{}});
        test(__LINE__, {"-t", "2"},           active{1, 0,1,1,0, 2,0,0,{}});
        test(__LINE__, {"-t", "2", "3" },     active{1, 0,0,1,0, 2,3,0,{}});
        test(__LINE__, {"-t", "2", "3", "4"}, active{1, 0,0,0,0, 2,3,4,{}});

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
