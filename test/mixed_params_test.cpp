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
    int av = 0;
    int bv = 1;
    float cv = 0.0f, dv = 1.0f;
    double ev = 0.0, fv = 1.0;
    std::string gv;
    bool a = false, b = false, c = false, d = false, e = false, f = false,
         g = false, h = false, i = false;

    friend bool operator == (const active& x, const active& y) noexcept {
        if(x.a != y.a || x.b != y.b || x.c != y.c ||
           x.d != y.d || x.e != y.e || x.f != y.f ||
           x.g != y.g || x.h != y.h || x.i != y.i ||
           x.av != y.av || x.bv != y.bv || x.gv != y.gv) return false;

        using std::abs;
        if(abs(x.cv - y.cv) > 1e-4f || abs(x.dv - y.dv) > 1e-4f ||
           abs(x.ev - y.ev) > 1e-4  || abs(x.fv - y.fv) > 1e-4) return false;

        return true;
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
        required("-a").set(m.a)         & value("A",m.av),
        required("-b", "--bb").set(m.b) & value("B",m.bv),
        option("-c", "--cc").set(m.c)   & value("C",m.cv) & opt_value("D",m.dv),
        option("-d", "--dd").set(m.d)   & opt_value("D",m.dv),
        required("-e", "--ee").set(m.e) & value("E",m.ev),
        option("-f", "--ff").set(m.f)   & opt_value("F",m.fv),
        value("G", m.gv).set(m.g),
        option("-h", "--hh", "---hhh").set(m.h),
        required("-i", "--ii").set(m.i)
    );

    run_wrapped_variants({ __FILE__, lineNo }, args, cli,
              [&]{ m = active{}; },
              [&]{ return m == matches; });
}


//-------------------------------------------------------------------
int main()
{

    try {
        active m;
        test(__LINE__, {""}, m);

        m = active{}; m.a = true;
        test(__LINE__, {"-a"}, m);
        m = active{}; m.a = true; m.av = 65;
        test(__LINE__, {"-a", "65"}, m);
        test(__LINE__, {"-a65"}, m);

        m = active{}; m.b = true;
        test(__LINE__, {"-b"}, m);
        m = active{}; m.b = true; m.bv = 12;
        test(__LINE__, {"-b", "12"}, m);
        test(__LINE__, {"-b12"}, m);

        m = active{}; m.c = true;
        test(__LINE__, {"-c"}, m);
        m = active{}; m.c = true; m.cv = 2.3f;
        test(__LINE__, {"-c", "2.3"}, m);
        test(__LINE__, {"-c2.3"}, m);
        test(__LINE__, {"-c2", ".3"}, m);
        test(__LINE__, {"-c", "+2.3"}, m);
        test(__LINE__, {"-c+2.3"}, m);
        test(__LINE__, {"-c+2", ".3"}, m);
        m = active{}; m.c = true; m.cv = -2.3f;
        test(__LINE__, {"-c", "-2.3"}, m);
        test(__LINE__, {"-c-2.3"}, m);
        test(__LINE__, {"-c-2", ".3"}, m);

        m = active{}; m.c = true; m.cv = 1; m.dv = 2;
        test(__LINE__, {"-c", "1", "2"}, m);
        test(__LINE__, {"-c1", "2"}, m);
        test(__LINE__, {"-c1", "2"}, m);

        m = active{}; m.d = true; m.c = true; m.cv = 2;
        test(__LINE__, {"-c", "2", "-d"}, m);
        m = active{}; m.a = true; m.av = 1; m.c = true; m.cv = 2;
        test(__LINE__, {"-c", "2", "-a", "1"}, m);

        m = active{}; m.d = true;
        test(__LINE__, {"-d"}, m);
        m = active{}; m.d = true; m.dv = 2.3f;
        test(__LINE__, {"-d", "2.3"}, m);
        test(__LINE__, {"-d2.3"}, m);
        test(__LINE__, {"-d2", ".3"}, m);

        m = active{}; m.e = true;
        test(__LINE__, {"-e"}, m);
        m = active{}; m.e = true; m.ev = 2.3;
        test(__LINE__, {"-e", "2.3"}, m);
        test(__LINE__, {"-e2.3"}, m);
        test(__LINE__, {"-e2", ".3"}, m);

        m = active{}; m.f = true;
        test(__LINE__, {"-f"}, m);
        test(__LINE__, {"--ff"}, m);
        m = active{}; m.f = true; m.fv = 2.3;
        test(__LINE__, {"-f", "2.3"}, m);
        test(__LINE__, {"--ff", "2.3"}, m);
        test(__LINE__, {"-f2.3"}, m);
        test(__LINE__, {"--ff2.3"}, m);
        test(__LINE__, {"-f2", ".3"}, m);
        test(__LINE__, {"--ff2", ".3"}, m);

        m = active{}; m.g = true; m.gv = "xyz";
        test(__LINE__, {"xyz"}, m);

        m = active{}; m.g = true; m.gv = "-h";
        test(__LINE__, {"-h"}, m);
        m = active{}; m.g = true; m.gv = "--hh";
        test(__LINE__, {"--hh"}, m);
        m = active{}; m.g = true; m.gv = "---hhh";
        test(__LINE__, {"---hhh"}, m);

        m = active{}; m.g = true; m.gv = "--h";
        test(__LINE__, {"--h"}, m);
        m = active{}; m.g = true; m.gv = "--hh";
        test(__LINE__, {"--hh"}, m);
        m = active{}; m.g = true; m.gv = "-hh";
        test(__LINE__, {"-hh"}, m);
        m = active{}; m.g = true; m.gv = "-hhh";
        test(__LINE__, {"-hhh"}, m);

        m = active{}; m.h = true; m.g = true; m.gv = "x-y.z";
        test(__LINE__, {"x-y.z", "-h"}, m);
        test(__LINE__, {"x-y.z", "--hh"}, m);
        test(__LINE__, {"x-y.z", "---hhh"}, m);

        m = active{}; m.i = true; m.g = true; m.gv = "xYz";
        test(__LINE__, {"xYz", "-i"}, m);
        test(__LINE__, {"xYz", "--ii"}, m);

        m = active{}; m.g = true; m.gv = "-ii";
        test(__LINE__, {"-ii"}, m);
        m = active{}; m.g = true; m.gv = "--i";
        test(__LINE__, {"--i"}, m);

        m = active{};
        m.a = true; m.av = 65;
        m.b = true; m.bv = 12;
        m.c = true; m.cv = -0.12f;
        m.d = true; m.dv = 2.3f;
        m.e = true; m.ev = 3.4;
        m.f = true; m.fv = 5.6;
        m.g = true; m.gv = "x-y.z";
        m.h = true;
        m.i = true;

        test(__LINE__, {"-a", "65", "-b12", "-c", "-0.12f", "-d2.3",
                        "-e3", ".4", "--ff", "5.6", "x-y.z", "-h", "--ii"}, m);

        test(__LINE__, {"-b12", "-c", "-0.12f", "-d2.3", "-e3", ".4", "-a", "65",
                        "--ff", "5.6", "x-y.z", "-h", "--ii"}, m);

        test(__LINE__, {"-d2.3", "-e3", ".4", "-b12", "-c", "-0.12f", "-a", "65",
                        "--ff", "5.6", "x-y.z", "-h", "--ii"}, m);

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
