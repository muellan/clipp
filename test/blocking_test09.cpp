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
    active(const std::string f_,
           std::initializer_list<std::string> ts,
           bool http_, bool ftp_,
           std::initializer_list<std::string> ws) :
           http{http_}, ftp{ftp_}, f{f_}, tgts{ts}, wrong{ws}
    {}
    bool http = false, ftp = false;

    std::string f;
    std::vector<std::string> tgts;
    std::vector<std::string> wrong;

    friend bool operator == (const active& x, const active& y) noexcept {
        return x.http == y.http && x.f == y.f &&
               std::equal(x.tgts.begin(), x.tgts.end(), y.tgts.begin()) &&
               std::equal(x.wrong.begin(), x.wrong.end(), y.wrong.begin());
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
        value("file", m.f),
        required("-t") & values(match::prefix_not("-"), "target", m.tgts),
        option("--http").set(m.http) | option("--ftp").set(m.ftp),
        any_other(m.wrong)
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

        test(__LINE__, {"abc"}, active{"abc", {}, false, false, {}});

        test(__LINE__, {"abc", "--http"}, active{"abc", {}, true, false, {}});
        test(__LINE__, {"abc", "--ftp"}, active{"abc", {}, false, true, {}});
        test(__LINE__, {"abc", "--ftp", "--http"}, active{"abc", {}, false, true, {}});

        test(__LINE__, {"abc", "-t", "--http"}, active{"abc", {}, true, false, {}});
        test(__LINE__, {"abc", "-t", "--ftp"}, active{"abc", {}, false, true, {}});

        test(__LINE__, {"abc", "-t", "tgt1", "--http"}, active{"abc", {"tgt1"}, true, false, {}});
        test(__LINE__, {"abc", "-t", "tgt1", "--ftp"}, active{"abc", {"tgt1"}, false, true, {}});

        test(__LINE__, {"abc", "-t", "tgt1", "t2", "--http"}, active{"abc", {"tgt1", "t2"}, true, false, {}});
        test(__LINE__, {"abc", "-t", "tgt1", "t2", "--ftp"}, active{"abc", {"tgt1", "t2"}, false, true, {}});


        test(__LINE__, {"abc", "x"}, active{"abc", {}, false, false, {"x"}});
        test(__LINE__, {"abc", "-x"}, active{"abc", {}, false, false, {"-x"}});
        test(__LINE__, {"abc", "-ftp"}, active{"abc", {}, false, false, {"-ftp"}});
        test(__LINE__, {"abc", "--ftpx"}, active{"abc", {}, false, false, {"--ftpx"}});

        test(__LINE__, {"abc", "-x", "--http"}, active{"abc", {}, true, false, {"-x"}});
        test(__LINE__, {"abc", "-x", "--ftp"}, active{"abc", {}, false, true, {"-x"}});
        test(__LINE__, {"abc", "-x", "--ftp", "--http"}, active{"abc", {}, false, true, {"-x"}});

        test(__LINE__, {"abc", "-t", "-x", "--http"}, active{"abc", {}, true, false, {"-x"}});
        test(__LINE__, {"abc", "-t", "-x", "--ftp"}, active{"abc", {}, false, true, {"-x"}});

        test(__LINE__, {"abc", "-t", "tgt1", "-x", "--http"}, active{"abc", {"tgt1"}, true, false, {"-x"}});
        test(__LINE__, {"abc", "-t", "tgt1", "-x", "--ftp"}, active{"abc", {"tgt1"}, false, true, {"-x"}});

        test(__LINE__, {"abc", "-t", "tgt1", "t2", "-x", "--http"}, active{"abc", {"tgt1", "t2"}, true, false, {"-x"}});
        test(__LINE__, {"abc", "-t", "tgt1", "t2", "-x", "--ftp"}, active{"abc", {"tgt1", "t2"}, false, true, {"-x"}});

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
