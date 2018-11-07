/*****************************************************************************
 *
 * CLIPP - command line interfaces for modern C++
 *
 * released under MIT license
 *
 * (c) 2017-2018 André Müller; foss@andremueller-online.de
 *
 *****************************************************************************/

#include <cmath>

#include "testing.h"

//-------------------------------------------------------------------
#if defined(_MSC_VER)
#if _MSC_VER < 1800
namespace std {
  template <typename T>
  bool isinf(const T &x) { return !_finite(x); }
}
#endif
#endif

//-------------------------------------------------------------------
template<class T, bool = std::is_arithmetic<T>::value,
                  bool = std::is_floating_point<T>::value>
struct equals {
    static bool result(const T& a, const T&b) {
        return a == b;
    }
};

template<class T>
struct equals<T,true,false> {
    static bool result(const T& a, const T&b) {
        #if defined(_MSC_VER)
            bool ainf = a == std::numeric_limits<T>::infinity();
            bool binf = b == std::numeric_limits<T>::infinity();
            if(ainf && binf) return true;
            if(ainf || binf) return false;
        #else
            if(std::isinf(a) && std::isinf(b)) return true;
            if(std::isinf(a) || std::isinf(b)) return false;
        #endif
        return a == b;
    }
};

template<class T>
struct equals<T,true,true> {
    static bool result(const T& a, const T&b) {
        if(std::isinf(a) && std::isinf(b)) return true;
        if(std::isinf(a) || std::isinf(b)) return false;
        return std::abs(a-b) < T(1e-4);
    }
};


//-------------------------------------------------------------------
template<class T>
void test(int lineNo, T x, const std::string& arg, T expected)
{
    using namespace clipp;

    auto cli = group( value("", x) );

//    std::cout << lineNo << " " << x << "  '" << arg << "' " << expected;

    run_test({ __FILE__, lineNo }, {arg.c_str()}, cli, [&]{
//        std::cout << " -> " << x << std::endl;
        return equals<T>::result(x,expected); } );
}


//-------------------------------------------------------------------
template<class T>
void test_conv(int lineNo)
{
    test<T>(lineNo,  T(0),  "0", T(0) );
    test<T>(lineNo,  T(0),  "1", T(1) );
    test<T>(lineNo,  T(0),  "2", T(2) );
    test<T>(lineNo,  T(0), "66", T(66) );

    test<T>(lineNo,  T(0),  "  0  ", T(0) );
    test<T>(lineNo,  T(0),  "  1  ", T(1) );
    test<T>(lineNo,  T(0),  "  2  ", T(2) );
    test<T>(lineNo,  T(0), "  66  ", T(66) );

    constexpr auto maxv = std::numeric_limits<T>::max();
    test<T>(lineNo,  T(0), std::to_string(maxv), maxv);
    test<T>(lineNo,  T(0), "  " + std::to_string(maxv) + "  ", maxv);

    if(std::is_signed<T>::value) {
        constexpr auto minv = std::numeric_limits<T>::lowest();
        test<T>(lineNo, T(0), std::to_string(minv), minv);
        test<T>(lineNo, T(0), "  " + std::to_string(minv) + "  ", minv);
    }
    else {
        test<T>(lineNo, T(0), "-1", T(0) );
        test<T>(lineNo, T(0), "  -1  ", T(0) );
    }
}


//-------------------------------------------------------------------
template<class T, class Wide, bool = (sizeof(Wide) > sizeof(T))>
struct test_clamp {
    static void in(int lineNo) {
        constexpr auto maxv = std::numeric_limits<T>::max();
        test<T>(lineNo,  T(0), std::to_string(maxv), maxv);
        test<T>(lineNo,  T(0), "  " + std::to_string(maxv) + "  ", maxv);

        test<T>(lineNo,  T(0), std::to_string(Wide(maxv)+1), maxv);
        test<T>(lineNo,  T(0), "  " + std::to_string(Wide(maxv)+1) + "  ", maxv);

        if(std::is_signed<T>::value) {
            constexpr auto minv = std::numeric_limits<T>::lowest();
            test<T>(lineNo, T(0), std::to_string(Wide(minv)-1), minv);
            test<T>(lineNo, T(0), "  " + std::to_string(Wide(minv)-1) + "  ", minv);
        }
    }
};

template<class T, class Wide>
struct test_clamp<T,Wide,false> {
    static void in(int) {}
};


//-------------------------------------------------------------------
int main()
{
    try {

        test<bool>(__LINE__, false, "",  false);
        test<bool>(__LINE__, false, " ", true);
        test<bool>(__LINE__, false, "0", true);
        test<bool>(__LINE__, false, "1", true);
        test<bool>(__LINE__, false, "a", true);

        test_conv<unsigned char>( __LINE__ );
        test_conv<unsigned short int>( __LINE__ );
        test_conv<unsigned int>( __LINE__ );
        test_conv<unsigned long int>( __LINE__ );
        test_conv<unsigned long long int>( __LINE__ );

        test_conv<short int>( __LINE__ );
        test_conv<int>( __LINE__ );
        test_conv<long int>( __LINE__ );
        test_conv<long long int>( __LINE__ );

        test_conv<float>( __LINE__ );
        test_conv<double>( __LINE__ );
        test_conv<long double>( __LINE__ );

        test<char>(__LINE__, 0, "",  0);
        test<char>(__LINE__, 0, " ", ' ');
        test<char>(__LINE__, 0, "0", '0');
        test<char>(__LINE__, 0, "1", '1');
        test<char>(__LINE__, 0, "a", 'a');
        test<char>(__LINE__, 0, "11", 11);
        test<char>(__LINE__, 0, "65", 65);
        test<char>(__LINE__, 0, "127", 127);
        test<char>(__LINE__, 0, "128", 127);
        test<char>(__LINE__, 0, "-1", -1);
        test<char>(__LINE__, 0, "-128", -128);
        test<char>(__LINE__, 0, "-129", -128);

        test<std::string>(__LINE__, "", "",  "");
        test<std::string>(__LINE__, "", " ", " ");
        test<std::string>(__LINE__, "", "0", "0");
        test<std::string>(__LINE__, "", "1", "1");
        test<std::string>(__LINE__, "", "a", "a");
        test<std::string>(__LINE__, "", "ab", "ab");
        test<std::string>(__LINE__, "", "abc", "abc");

        using wide_ui_t = unsigned long long int;
        test_clamp<unsigned char,wide_ui_t>::in( __LINE__ );
        test_clamp<unsigned short int,wide_ui_t>::in( __LINE__ );
        test_clamp<unsigned int,wide_ui_t>::in( __LINE__ );
        test_clamp<unsigned long int,wide_ui_t>::in( __LINE__ );
        test_clamp<unsigned long long int,wide_ui_t>::in( __LINE__ );

        using wide_i_t  = long long int;
        test_clamp<char,wide_i_t>::in(__LINE__);
        test_clamp<short int,wide_i_t>::in( __LINE__ );
        test_clamp<int,wide_i_t>::in( __LINE__ );
        test_clamp<long int,wide_i_t>::in( __LINE__ );
        test_clamp<long long int,wide_i_t>::in( __LINE__ );

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
 }
