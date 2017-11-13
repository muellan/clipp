/*****************************************************************************
 *
 * CLIPP - command line interfaces for modern C++
 *
 * released under MIT license
 *
 * (c) 2017 André Müller; foss@andremueller-online.de
 *
 *****************************************************************************/

#include "testing.h"

//-------------------------------------------------------------------
template<class T>
struct equals {
    static bool result(const T& a, const T&b) { return a == b; }
};

template<>
struct equals<float> {
    static bool result(const float& a, const float&b) {
        return std::abs(a-b) < 1e-4f;
    }
};

template<>
struct equals<double> {
    static bool result(const double& a, const double&b) {
        return std::abs(a-b) < 1e-8;
    }
};

template<>
struct equals<long double> {
    static bool result(const long double& a, const long double&b) {
        return std::abs(a-b) < static_cast<long double>(1e-8);

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

        test<std::string>(__LINE__, "", "",  "");
        test<std::string>(__LINE__, "", " ", " ");
        test<std::string>(__LINE__, "", "0", "0");
        test<std::string>(__LINE__, "", "1", "1");
        test<std::string>(__LINE__, "", "a", "a");
        test<std::string>(__LINE__, "", "ab", "ab");
        test<std::string>(__LINE__, "", "abc", "abc");

    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
 }
