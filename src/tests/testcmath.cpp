// This file is part of the SpeedCrunch project
// Copyright (C) 2004-2006 Ariya Hidayat <ariya@kde.org>
// Copyright (C) 2007-2016 @heldercorreia
// Copyright (C) 2015-2016 Hadrien Theveneau <hadrien.theveneau@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; see the file COPYING.  If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.

#include "math/cmath.h"
#include "math/floatconst.h"

#include "tests/testcommon.h"

#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

typedef CNumber::Format Format;

#define CHECK(x,y) check_value(__FILE__,__LINE__,#x,x,y)
#define CHECK_FORMAT(f,x,y) check_format(__FILE__,__LINE__,#x,x,f,y)
#define CHECK_PRECISE(x,y) check_precise(__FILE__,__LINE__,#x,x,y)
#define CHECK_KNOWN_ISSUE(x,y,n) check_value(__FILE__,__LINE__,#x,x,y,n)

static int cmath_total_tests = 0;
static int cmath_failed_tests = 0;
static int cmath_new_failed_tests = 0;

CNumber PI;

static void check_value(const char* file, int line, const char* msg, const CNumber& n, const char* expected, int issue = 0)
{
    ++cmath_total_tests;
    string result = CMath::format(n, Format::Fixed()).toStdString();
    DisplayErrorOnMismatch(file, line, msg, result, expected, cmath_failed_tests, cmath_new_failed_tests, issue);
}

static void check_format(const char* file, int line, const char* msg, const CNumber& n, Format format, const char* expected)
{
    ++cmath_total_tests;
    string result = CMath::format(n, format).toStdString();
    DisplayErrorOnMismatch(file, line, msg, result, expected, cmath_failed_tests, cmath_new_failed_tests, 0);
}

static void check_precise(const char* file, int line, const char* msg, const CNumber& n, const char* expected)
{
    ++cmath_total_tests;
    string result = CMath::format(n, Format::Fixed() + Format::Precision(50)).toStdString();
    DisplayErrorOnMismatch(file, line, msg, result, expected, cmath_failed_tests, cmath_new_failed_tests, 0);
}

void test_create()
{
    // Real numbers
    CHECK(CNumber("1"),      "1");
    CHECK(CNumber("2"),      "2");
    CHECK(CNumber("1.0"),    "1");
    CHECK(CNumber("1e-3"),   "0.001");
    CHECK(CNumber("1.0e-3"), "0.001");

    // Complex number
    CHECK(CNumber("0+1j"),           "1j");
    CHECK(CNumber("1+1j"),           "1+1j");
    CHECK(CNumber("2+2j"),           "2+2j");
    CHECK(CNumber("0+1.0j"),         "1j");
    CHECK(CNumber("0.0+1.0j"),       "1j");
    CHECK(CNumber("1.0+1.0j"),       "1+1j");
    CHECK(CNumber("1e-3+1e-3j"),     "0.001+0.001j");
    CHECK(CNumber("1.0e-3+1.0e-3j"), "0.001+0.001j");

    // Too large or too small.
    CHECK(CNumber("1e1000000000"), "NaN");
    CHECK(CNumber("1e-1000000000"), "NaN");
    CHECK_FORMAT(Format::Scientific() + Format::Precision(2), CNumber("1e1000000000"), "NaN");
    CHECK_FORMAT(Format::Scientific() + Format::Precision(2), CNumber("1e-1000000000"), "NaN");

    CHECK(CMath::i()*CMath::i(), "-1");

    // Long numbers, like in session history
    CHECK(CNumber("0+1.000000000000000000000000000000000000000000000000000000000000000000000000000000j"), "1j");
}

void test_format()
{
    Format f = Format::Fixed();
    Format n = Format::Engineering();
    Format e = Format::Scientific();
    Format g = Format::General();

    // Fixed decimal digits.
    CHECK_FORMAT(f + Format::Precision(0), CNumber("NaN"), "NaN");
    CHECK_FORMAT(f + Format::Precision(0), CNumber("0"), "0");
    CHECK_FORMAT(f + Format::Precision(0), CNumber("1.1"), "1");
    CHECK_FORMAT(f + Format::Precision(1), CNumber("2.11"), "2.1");
    CHECK_FORMAT(f + Format::Precision(2), CNumber("3.111"), "3.11");
    CHECK_FORMAT(f + Format::Precision(1), CNumber("4.001"), "4.0");
    CHECK_FORMAT(f + Format::Precision(3), CNumber("4.1111"), "4.111");
    CHECK_FORMAT(f + Format::Precision(2), CNumber("3.14"), "3.14");
    CHECK_FORMAT(f + Format::Precision(3), CNumber("3.14"), "3.140");
    CHECK_FORMAT(f + Format::Precision(5), CNumber("3.14"), "3.14000");
    CHECK_FORMAT(f + Format::Precision(7), CNumber("3.14"), "3.1400000");
    CHECK_FORMAT(f + Format::Precision(7), CNumber("-0.001"), "-0.0010000");
    CHECK_FORMAT(f + Format::Precision(8), CNumber("-0.001"), "-0.00100000");
    CHECK_FORMAT(f + Format::Precision(9), CNumber("-0.001"), "-0.001000000");
    CHECK_FORMAT(f + Format::Precision(-1), CNumber("4.000000000000000000000000000000000000000000001"), "4");

    // Engineering notation.
    CHECK_FORMAT(n + Format::Precision(0), CNumber("NaN"), "NaN");
    CHECK_FORMAT(n + Format::Precision(0), CNumber("0"), "0");
    CHECK_FORMAT(n + Format::Precision(0), CNumber("3.14"), "3.14e0");
    CHECK_FORMAT(n + Format::Precision(1), CNumber("3.14"), "3.14e0");
    CHECK_FORMAT(n + Format::Precision(2), CNumber("3.14"), "3.14e0");
    CHECK_FORMAT(n + Format::Precision(3), CNumber("3.14"), "3.140e0");
    CHECK_FORMAT(n + Format::Precision(5), CNumber("3.14"), "3.14000e0");
    CHECK_FORMAT(n + Format::Precision(7), CNumber("3.14"), "3.1400000e0");
    CHECK_FORMAT(n + Format::Precision(3), CNumber("-0.001"), "-1.000e-3");
    CHECK_FORMAT(n + Format::Precision(2), CNumber("0.0001"), "100.e-6");
    CHECK_FORMAT(n + Format::Precision(2), CNumber("0.001"), "1.00e-3");
    CHECK_FORMAT(n + Format::Precision(2), CNumber("0.01"), "10.0e-3");
    CHECK_FORMAT(n + Format::Precision(2), CNumber("0.1"), "100.e-3");
    CHECK_FORMAT(n + Format::Precision(2), CNumber("1"), "1.00e0");
    CHECK_FORMAT(n + Format::Precision(2), CNumber("10"), "10.0e0");
    CHECK_FORMAT(n + Format::Precision(2), CNumber("100"), "100.e0");
    CHECK_FORMAT(n + Format::Precision(2), CNumber("1000"), "1.00e3");
    CHECK_FORMAT(n + Format::Precision(2), CNumber("10000"), "10.0e3");
    CHECK_FORMAT(n + Format::Precision(2), CNumber("100000"), "100.e3");
    CHECK_FORMAT(n + Format::Precision(2), CNumber("1000000"), "1.00e6");
    CHECK_FORMAT(n + Format::Precision(2), CNumber("10000000"), "10.0e6");

    // Scientific notation.
    CHECK_FORMAT(e + Format::Precision(0), CNumber("NaN"), "NaN");
    CHECK_FORMAT(e + Format::Precision(0), CNumber("0"), "0");
    CHECK_FORMAT(e + Format::Precision(0), CNumber("3.14"), "3e0");
    CHECK_FORMAT(e + Format::Precision(1), CNumber("3.14"), "3.1e0");
    CHECK_FORMAT(e + Format::Precision(2), CNumber("3.14"), "3.14e0");
    CHECK_FORMAT(e + Format::Precision(3), CNumber("3.14"), "3.140e0");
    CHECK_FORMAT(e + Format::Precision(5), CNumber("3.14"), "3.14000e0");
    CHECK_FORMAT(e + Format::Precision(7), CNumber("3.14"), "3.1400000e0");
    CHECK_FORMAT(e + Format::Precision(3), CNumber("-0.001"), "-1.000e-3");
    CHECK_FORMAT(e + Format::Precision(2), CNumber("0.0001"), "1.00e-4");
    CHECK_FORMAT(e + Format::Precision(2), CNumber("0.001"), "1.00e-3");
    CHECK_FORMAT(e + Format::Precision(2), CNumber("0.01"), "1.00e-2");
    CHECK_FORMAT(e + Format::Precision(2), CNumber("0.1"), "1.00e-1");
    CHECK_FORMAT(e + Format::Precision(2), CNumber("1"), "1.00e0");
    CHECK_FORMAT(e + Format::Precision(2), CNumber("10"), "1.00e1");
    CHECK_FORMAT(e + Format::Precision(2), CNumber("100"), "1.00e2");
    CHECK_FORMAT(e + Format::Precision(2), CNumber("1000"), "1.00e3");
    CHECK_FORMAT(e + Format::Precision(2), CNumber("10000"), "1.00e4");
    CHECK_FORMAT(e + Format::Precision(2), CNumber("100000"), "1.00e5");
    CHECK_FORMAT(e + Format::Precision(2), CNumber("1000000"), "1.00e6");
    CHECK_FORMAT(e + Format::Precision(2), CNumber("10000000"), "1.00e7");

    // General format.
    CHECK_FORMAT(g + Format::Precision(-1), PI, "3.14159265358979323846");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("0"), "0");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("0.000000001"), "1.000e-9");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("0.00000001"), "1.000e-8");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("0.0000001"), "1.000e-7");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("0.000001"), "1.000e-6");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("0.00001"), "1.000e-5");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("0.0001"), "1.000e-4");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("0.001"), "0.001");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("0.01"), "0.010");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("0.1"), "0.100");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("10"), "10.000");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("100"), "100.000");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("1000"), "1000.000");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("10000"), "10000.000");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("100000"), "100000.000");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("1000000"), "1.000e6");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("10000000"), "1.000e7");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("100000000"), "1.000e8");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("1403.1977"), "1403.198");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("2604.1980"), "2604.198");
    CHECK_FORMAT(g + Format::Precision(3), CNumber("2.47e4"), "24700.000");
}

void test_op()
{
    // Addition.
    CHECK(CNumber(0) + CNumber(0), "0");
    CHECK(CNumber(1) + CNumber(0), "1");
    CHECK(CNumber(1) + CNumber(1), "2");
    CHECK(CNumber(1) + CNumber(2), "3");
    CHECK(CNumber(1) + CNumber(10), "11");
    CHECK(CNumber(1) + CNumber(100), "101");
    CHECK(CNumber(1) + CNumber(1000), "1001");
    CHECK(CNumber(5) + CNumber("NaN"), "NaN");
    CHECK(CNumber("NaN") + CNumber(5), "NaN");

    // Subtraction.
    CHECK(CNumber(0) - CNumber(0), "0");
    CHECK(CNumber(1) - CNumber(0), "1");
    CHECK(CNumber(1) - CNumber(2), "-1");
    CHECK(CNumber(1) - CNumber("NaN"), "NaN");
    CHECK(CNumber("NaN") - CNumber(1), "NaN");

    // Division.
    CHECK(CNumber(1) / CNumber(2), "0.5");
    CHECK(CNumber(1) / CMath::sin(PI), "NaN");
    CHECK(CNumber(1) / CNumber("NaN"), "NaN");
    CHECK(CNumber("NaN") / CNumber(4), "NaN");

    // Division by zero.
    CHECK(CNumber(1) / CNumber(0), "NaN");
    CHECK(CNumber(1) / CMath::sin(0), "NaN");
    CHECK(CNumber(1) / CMath::sin(PI), "NaN");
    CHECK(CNumber(1) / CMath::cos(PI/2), "NaN");
    CHECK(CNumber(1) / CMath::tan(0), "NaN");
    CHECK(CNumber(1) / CMath::tan(PI), "NaN");
    CHECK(CNumber(1) / CMath::trunc("0.123"), "NaN");
    CHECK(CNumber(1) / CMath::round("0.456"), "NaN");
    CHECK(CNumber(1) / CMath::binomialPmf(1, 10, 0), "NaN");

    CHECK_PRECISE(CNumber(1) / CNumber(3), "0.33333333333333333333333333333333333333333333333333");
    CHECK_PRECISE(CNumber(2) / CNumber(3), "0.66666666666666666666666666666666666666666666666667");
    CHECK_PRECISE(CNumber(1) / CNumber(7), "0.14285714285714285714285714285714285714285714285714");
    CHECK_PRECISE(CNumber(2) / CNumber(7), "0.28571428571428571428571428571428571428571428571429");
    CHECK_PRECISE(CNumber(3) / CNumber(7), "0.42857142857142857142857142857142857142857142857143");
    CHECK_PRECISE(CNumber(4) / CNumber(7), "0.57142857142857142857142857142857142857142857142857");
    CHECK_PRECISE(CNumber(1) / CNumber(9), "0.11111111111111111111111111111111111111111111111111");

    // Multiplication.
    CHECK(CNumber(0)* CNumber(0), "0");
    CHECK(CNumber("1.0")* CNumber("0.0"), "0");
    CHECK(CNumber(1)* CNumber(1), "1");
    CHECK(CNumber(3)* CNumber(-4), "-12");
    CHECK(CNumber(-2)* CNumber(5), "-10");
    CHECK(CNumber(6)* CNumber(7), "42");
    CHECK(CNumber("1.5")* CNumber("1.5"), "2.25");
    CHECK(CNumber(4) * CNumber("NaN"), "NaN");
    CHECK(CNumber("NaN") * CNumber(4), "NaN");
}

void test_functions()
{
    CHECK(PI, "3.14159265358979323846");
    CHECK_PRECISE(PI, "3.14159265358979323846264338327950288419716939937511");

    CHECK(CMath::abs("NaN"), "NaN");
    CHECK(CMath::abs("0"), "0");
    CHECK(CMath::abs("1"), "1");
    CHECK(CMath::abs("100"), "100");
    CHECK(CMath::abs("-100"), "100");
    CHECK(CMath::abs("-3.14159"), "3.14159");
    CHECK(CMath::abs("-0.00000014159"), "0.00000014159");

    CHECK(CMath::floor("NaN"), "NaN");
    CHECK(CMath::floor("0"), "0");
    CHECK(CMath::floor("5"), "5");
    CHECK(CMath::floor("-5"), "-5");
    CHECK(CMath::floor("3.14"), "3");
    CHECK(CMath::floor("1.77"), "1");
    CHECK(CMath::floor("-2.6041980"), "-3");
    CHECK(CMath::floor("2.6041980"), "2");
    CHECK(CMath::floor("0.000001"), "0");
    CHECK(CMath::floor("-0.000001"), "-1");
    CHECK(CMath::floor(CNumber(1) / 3 * 3), "1");

    CHECK(CMath::ceil("NaN"), "NaN");
    CHECK(CMath::ceil("0"), "0");
    CHECK(CMath::ceil("5"), "5");
    CHECK(CMath::ceil("-5"), "-5");
    CHECK(CMath::ceil("3.14"), "4");
    CHECK(CMath::ceil("1.77"), "2");
    CHECK(CMath::ceil("-2.6041980"), "-2");
    CHECK(CMath::ceil("2.6041980"), "3");
    CHECK(CMath::ceil("0.000001"), "1");
    CHECK(CMath::ceil("-0.000001"), "0");
    CHECK(CMath::ceil(CMath::log(2, 128)), "7");

    CHECK(CMath::gcd("NaN", "NaN"), "NaN");
    CHECK(CMath::gcd("NaN", "5"), "NaN");
    CHECK(CMath::gcd("5", "NaN"), "NaN");
    CHECK(CMath::gcd("0", "0"), "0");
    CHECK(CMath::gcd("0", "5"), "5");
    CHECK(CMath::gcd("5", "0"), "5");
    CHECK(CMath::gcd("0", "-5"), "5");
    CHECK(CMath::gcd("-5", "0"), "5");
    CHECK(CMath::gcd("9", "-27"), "9");
    CHECK(CMath::gcd("99", "103"), "1");
    CHECK(CMath::gcd("-102", "306"), "102");

    CHECK(CMath::round("NaN"), "NaN");
    CHECK(CMath::round("3.14"), "3");
    CHECK(CMath::round("-1.77"), "-2");

    CHECK(CMath::round("NaN", 3), "NaN");
    CHECK(CMath::round("0.005", -3), "0");
    CHECK(CMath::round("0.005", 2), "0");
    CHECK(CMath::round("3.14159", 4), "3.1416");
    CHECK(CMath::round("3.14159", 3), "3.142");
    CHECK(CMath::round("3.14159", 2), "3.14");
    CHECK(CMath::round("3.14159", 1), "3.1");
    CHECK(CMath::round("3.14159", 0), "3");
    CHECK(CMath::round("-2.6041980", 4), "-2.6042");
    CHECK(CMath::round("-2.6041980", 3), "-2.604");
    CHECK(CMath::round("-2.6041980", 2), "-2.6");
    CHECK(CMath::round("-2.6041980", 1), "-2.6");
    CHECK(CMath::round("-2.6041980", 0), "-3");

    CHECK(CMath::trunc("NaN"), "NaN");
    CHECK(CMath::trunc("0"), "0");
    CHECK(CMath::trunc("3.14"), "3");
    CHECK(CMath::trunc("1.77"), "1");

    CHECK(CMath::trunc("NaN", 3), "NaN");
    CHECK(CMath::trunc("0.00013", -3), "0");
    CHECK(CMath::trunc("0.00013", 4), "0.0001");
    CHECK(CMath::trunc("3.14159", 4), "3.1415");
    CHECK(CMath::trunc("3.14159", 3), "3.141");
    CHECK(CMath::trunc("3.14159", 2), "3.14");
    CHECK(CMath::trunc("3.14159", 1), "3.1");
    CHECK(CMath::trunc("3.14159", 0), "3");
    CHECK(CMath::trunc("-2.6041980", 4), "-2.6041");
    CHECK(CMath::trunc("-2.6041980", 3), "-2.604");
    CHECK(CMath::trunc("-2.6041980", 2), "-2.6");
    CHECK(CMath::trunc("-2.6041980", 1), "-2.6");
    CHECK(CMath::trunc("-2.6041980", 0), "-2");
    CHECK(CMath::trunc("-2.6041980", 999), "-2.604198");
    CHECK(CMath::trunc("-2.6041980", -2), "0");

    CHECK(CMath::integer("NaN"), "NaN");
    CHECK(CMath::integer("0"), "0");
    CHECK(CMath::integer("0.25"), "0");
    CHECK(CMath::integer("0.85"), "0");
    CHECK(CMath::integer("14.0377"), "14");
    CHECK(CMath::integer("-0.25"), "0");
    CHECK(CMath::integer("-0.85"), "0");
    CHECK(CMath::integer("-14.0377"), "-14");

    CHECK(CMath::frac("NaN"), "NaN");
    CHECK(CMath::frac("0"), "0");
    CHECK(CMath::frac("3.14159"), "0.14159");
    CHECK(CMath::frac("0.14159"), "0.14159");
    CHECK(CMath::frac("-3.14159"), "-0.14159");
    CHECK(CMath::frac("-0.14159"), "-0.14159");

    CHECK(CMath::sqrt("NaN"), "NaN");
    CHECK(CMath::sqrt(-1), "1j");
    CHECK(CMath::sqrt(0), "0");
    CHECK(CMath::sqrt(1), "1");
    CHECK(CMath::sqrt(4), "2");
    CHECK(CMath::sqrt(9), "3");
    CHECK(CMath::sqrt(16), "4");
    CHECK(CMath::sqrt("0.04"), "0.2");
    CHECK(CMath::sqrt("0.09"), "0.3");
    CHECK(CMath::sqrt("0.16"), "0.4");
    CHECK_PRECISE(CMath::sqrt(2), "1.41421356237309504880168872420969807856967187537695");
    CHECK_PRECISE(CMath::sqrt(3), "1.73205080756887729352744634150587236694280525381038");
    CHECK_PRECISE(CMath::sqrt(5), "2.23606797749978969640917366873127623544061835961153");
    CHECK_PRECISE(CMath::sqrt(7), "2.64575131106459059050161575363926042571025918308245");
    CHECK_PRECISE(CMath::sqrt(8), "2.82842712474619009760337744841939615713934375075390");
    CHECK_PRECISE(CMath::sqrt(10), "3.16227766016837933199889354443271853371955513932522");
    CHECK_PRECISE(CMath::sqrt(11), "3.31662479035539984911493273667068668392708854558935");
    CHECK_PRECISE(CMath::sqrt(12), "3.46410161513775458705489268301174473388561050762076");
    CHECK_PRECISE(CMath::sqrt(13), "3.60555127546398929311922126747049594625129657384525");
    CHECK_PRECISE(CMath::sqrt(14), "3.74165738677394138558374873231654930175601980777873");
    CHECK_PRECISE(CMath::sqrt(15), "3.87298334620741688517926539978239961083292170529159");
    CHECK_PRECISE(CMath::sqrt(17), "4.12310562561766054982140985597407702514719922537362");
    CHECK_PRECISE(CMath::sqrt(18), "4.24264068711928514640506617262909423570901562613084");
    CHECK_PRECISE(CMath::sqrt(19), "4.35889894354067355223698198385961565913700392523244");
    CHECK_PRECISE(CMath::sqrt(20), "4.47213595499957939281834733746255247088123671922305");

    CHECK(CMath::cbrt("NaN"), "NaN");
    CHECK(CMath::cbrt(0), "0");
    CHECK(CMath::cbrt(1), "1");
    CHECK(CMath::cbrt(-1), "-1");
    CHECK(CMath::cbrt(8), "2");
    CHECK(CMath::cbrt(-8), "-2");
    CHECK(CMath::cbrt(27), "3");
    CHECK(CMath::cbrt(-27), "-3");
    CHECK(CMath::cbrt(64), "4");
    CHECK(CMath::cbrt(-64), "-4");
    CHECK(CMath::cbrt("0.008"), "0.2");
    CHECK(CMath::cbrt("-0.008"), "-0.2");
    CHECK(CMath::cbrt("0.027"), "0.3");
    CHECK(CMath::cbrt("-0.027"), "-0.3");
    CHECK(CMath::cbrt("0.064"), "0.4");
    CHECK(CMath::cbrt("-0.064"), "-0.4");
    CHECK_PRECISE(CMath::cbrt(2), "1.25992104989487316476721060727822835057025146470151");
    CHECK_PRECISE(CMath::cbrt(3), "1.44224957030740838232163831078010958839186925349935");
    CHECK_PRECISE(CMath::cbrt(-20), "-2.71441761659490657151808946967948920480510776948910");

    CHECK(CMath::sgn("NaN"), "NaN");
    CHECK(CMath::sgn(0), "0");
    CHECK(CMath::sgn(1), "1");
    CHECK(CMath::sgn(-1), "-1");
    CHECK(CMath::sgn(2), "1");
    CHECK(CMath::sgn(-2), "-1");

    CHECK(CMath::factorial("NaN"), "NaN");
    CHECK(CMath::factorial(-1), "NaN");
    CHECK(CMath::factorial(0), "1");
    CHECK(CMath::factorial(1), "1");
    CHECK(CMath::factorial(2), "2");
    CHECK(CMath::factorial(3), "6");
    CHECK(CMath::factorial(4), "24");
    CHECK(CMath::factorial(5), "120");
    CHECK(CMath::factorial(6), "720");
    CHECK(CMath::factorial(7), "5040");
    CHECK(CMath::factorial(8), "40320");
    CHECK_PRECISE(CMath::factorial("5.23"), "178.50732778544229114185259335979946974446422321576241");
    CHECK(CMath::factorial("-5"), "NaN");

    CHECK(CMath::factorial("NaN", "NaN"), "NaN");
    CHECK(CMath::factorial("NaN", 7), "NaN");
    CHECK(CMath::factorial(8, "NaN"), "NaN");
    CHECK(CMath::factorial(8, 0), "0");
    CHECK(CMath::factorial(8, 6), "336");
    CHECK(CMath::factorial(8, 7), "56");
    CHECK(CMath::factorial(8, 8), "8");

    CHECK(CMath::nCr("NaN", "NaN"), "NaN");
    CHECK(CMath::nCr("NaN", 5), "NaN");
    CHECK(CMath::nCr(5, "NaN"), "NaN");
    CHECK(CMath::nCr(21, -1), "0");
    CHECK(CMath::nCr(-21, 2), "231");
    CHECK(CMath::nCr(21, 0), "1");
    CHECK(CMath::nCr(21, 1), "21");
    CHECK(CMath::nCr(21, 2), "210");
    CHECK(CMath::nCr(21, 3), "1330");
    CHECK(CMath::nCr(21, 19), "210");
    CHECK(CMath::nCr(21, 20), "21");
    CHECK(CMath::nCr(21, 21), "1");
    CHECK(CMath::nCr(21, 22), "0");
    CHECK(CMath::nCr(0, 0), "1");

    CHECK(CMath::nPr("NaN", "NaN"), "NaN");
    CHECK(CMath::nPr("NaN", 5), "NaN");
    CHECK(CMath::nPr(5, "NaN"), "NaN");
    CHECK(CMath::nPr(21, 22), "0");
    CHECK(CMath::nPr(-21, 2), "462");
    CHECK(CMath::nPr(21, -1), "0.04545454545454545455");
    CHECK(CMath::nPr(0, 0), "1");
    CHECK(CMath::nPr(21, 0), "1");
    CHECK(CMath::nPr(21, 1), "21");
    CHECK(CMath::nPr(21, 2), "420");
    CHECK(CMath::nPr(21, 3), "7980");
    CHECK(CMath::nPr(21, 4), "143640");
    CHECK(CMath::nPr(21, 5), "2441880");
    CHECK(CMath::nPr(21, 6), "39070080");
    CHECK(CMath::nPr(21, 7), "586051200");

    CHECK(CMath::raise("NaN", "NaN"), "NaN");
    CHECK(CMath::raise("NaN", "0"), "NaN");
    CHECK(CMath::raise("-1", "NaN"), "NaN");
    CHECK(CMath::raise(10, -3), "0.001");
    CHECK(CMath::raise(10, -2), "0.01");
    CHECK(CMath::raise(10, -1), "0.1");
    CHECK(CMath::raise(10, 0), "1");
    CHECK(CMath::raise(10, 1), "10");
    CHECK(CMath::raise(10, 2), "100");
    CHECK(CMath::raise(10, 3), "1000");
    CHECK(CMath::raise(10, 4), "10000");
    CHECK(CMath::raise("2", "2"), "4");
    CHECK(CMath::raise("3", "3"), "27");
    CHECK(CMath::raise("4", "4"), "256");
    CHECK(CMath::raise("-27", CNumber("1") / CNumber("3")), "1.5+2.59807621135331594029j");
    CHECK(CMath::raise("-27", CNumber("-1") / CNumber("3")), "0.16666666666666666667-0.28867513459481288225j");
    CHECK_PRECISE(CMath::raise("2", "0.1"), "1.07177346253629316421300632502334202290638460497756");
    CHECK_PRECISE(CMath::raise("2", "0.2"), "1.14869835499703500679862694677792758944385088909780");
    CHECK_PRECISE(CMath::raise("2", "0.3"), "1.23114441334491628449939306916774310987613776110082");

    CHECK(CMath::exp("NaN"), "NaN");
    CHECK(CMath::exp("0"), "1");
    CHECK_PRECISE(CMath::exp("0.1"), "1.10517091807564762481170782649024666822454719473752");
    CHECK_PRECISE(CMath::exp("0.2"), "1.22140275816016983392107199463967417030758094152050");
    CHECK_PRECISE(CMath::exp("0.3"), "1.34985880757600310398374431332800733037829969735937");
    CHECK_PRECISE(CMath::exp("0.4"), "1.49182469764127031782485295283722228064328277393743");
    CHECK_PRECISE(CMath::exp("0.5"), "1.64872127070012814684865078781416357165377610071015");
    CHECK_PRECISE(CMath::exp("0.6"), "1.82211880039050897487536766816286451338223880854644");
    CHECK_PRECISE(CMath::exp("0.7"), "2.01375270747047652162454938858306527001754239414587");
    CHECK_PRECISE(CMath::exp("0.8"), "2.22554092849246760457953753139507675705363413504848");
    CHECK_PRECISE(CMath::exp("0.9"), "2.45960311115694966380012656360247069542177230644008");
    CHECK_PRECISE(CMath::exp("1.0"), "2.71828182845904523536028747135266249775724709369996");
    CHECK_PRECISE(CMath::exp("100"), "26881171418161354484126255515800135873611118.77374192241519160861528028703490956400000000000000"/*"91415887109722"*/);

    CHECK(CMath::binomialPmf("NaN", "NaN", "NaN"), "NaN");
    CHECK(CMath::binomialPmf("NaN", "NaN", "0.5"), "NaN");
    CHECK(CMath::binomialPmf("NaN", "10", "NaN"), "NaN");
    CHECK(CMath::binomialPmf("NaN", "10", "0.5"), "NaN");
    CHECK(CMath::binomialPmf("5", "NaN", "NaN"), "NaN");
    CHECK(CMath::binomialPmf("5", "NaN", "0.5"), "NaN");
    CHECK(CMath::binomialPmf("5", "10", "NaN"), "NaN");
    CHECK(CMath::binomialPmf("5", "10", "0.5"), "0.24609375");
    CHECK(CMath::binomialPmf("-5", "10", "0.5"), "0");
    CHECK_PRECISE(CMath::binomialPmf("5", "10", "0.5"), "0.24609375000000000000000000000000000000000000000000");

    CHECK(CMath::binomialCdf("NaN", "NaN", "NaN"), "NaN");
    CHECK(CMath::binomialCdf("NaN", "NaN", "0.5"), "NaN");
    CHECK(CMath::binomialCdf("NaN", "10", "NaN"), "NaN");
    CHECK(CMath::binomialCdf("NaN", "10", "0.5"), "NaN");
    CHECK(CMath::binomialCdf("5", "NaN", "NaN"), "NaN");
    CHECK(CMath::binomialCdf("5", "NaN", "0.5"), "NaN");
    CHECK(CMath::binomialCdf("5", "10", "NaN"), "NaN");
    CHECK(CMath::binomialCdf("5", "10", "0.5"), "0.623046875");
    CHECK(CMath::binomialCdf("-5", "10", "0.5"), "0");
    CHECK_PRECISE(CMath::binomialCdf("5", "10", "0.5"), "0.62304687500000000000000000000000000000000000000000");

    CHECK(CMath::binomialMean("NaN", "NaN"), "NaN");
    CHECK(CMath::binomialMean("NaN", "0.5"), "NaN");
    CHECK(CMath::binomialMean("10", "NaN"), "NaN");
    CHECK(CMath::binomialMean("10", "0.5"), "5");
    CHECK(CMath::binomialMean("-10", "0.5"), "NaN");
    CHECK_PRECISE(CMath::binomialMean("10", "0.5"), "5.00000000000000000000000000000000000000000000000000");

    CHECK(CMath::binomialVariance("NaN", "NaN"), "NaN");
    CHECK(CMath::binomialVariance("NaN", "0.5"), "NaN");
    CHECK(CMath::binomialVariance("10", "NaN"), "NaN");
    CHECK(CMath::binomialVariance("10", "0.5"), "2.5");
    CHECK(CMath::binomialVariance("-10", "0.5"), "NaN");
    CHECK_PRECISE(CMath::binomialVariance("10", "0.5"), "2.50000000000000000000000000000000000000000000000000");

    CHECK(CMath::hypergeometricPmf("NaN", "NaN", "NaN", "NaN"), "NaN");
    CHECK(CMath::hypergeometricPmf("NaN", "NaN", "NaN", "5"), "NaN");
    CHECK(CMath::hypergeometricPmf("NaN", "NaN", "10", "NaN"), "NaN");
    CHECK(CMath::hypergeometricPmf("NaN", "NaN", "10", "5"), "NaN");
    CHECK(CMath::hypergeometricPmf("1", "15", "NaN", "NaN"), "NaN");
    CHECK(CMath::hypergeometricPmf("1", "15", "NaN", "5"), "NaN");
    CHECK(CMath::hypergeometricPmf("1", "15", "10", "NaN"), "NaN");
    CHECK(CMath::hypergeometricPmf("-1", "15", "10", "5"), "0");
    CHECK(CMath::hypergeometricPmf("1", "15", "10", "5"), "0.01665001665001665002");
    CHECK_PRECISE(CMath::hypergeometricPmf("1", "15", "10", "5"), "0.01665001665001665001665001665001665001665001665002");

    CHECK(CMath::hypergeometricCdf("NaN", "NaN", "NaN", "NaN"), "NaN");
    CHECK(CMath::hypergeometricCdf("NaN", "NaN", "NaN", "5"), "NaN");
    CHECK(CMath::hypergeometricCdf("NaN", "NaN", "10", "NaN"), "NaN");
    CHECK(CMath::hypergeometricCdf("NaN", "NaN", "10", "5"), "NaN");
    CHECK(CMath::hypergeometricCdf("1", "15", "NaN", "NaN"), "NaN");
    CHECK(CMath::hypergeometricCdf("1", "15", "NaN", "5"), "NaN");
    CHECK(CMath::hypergeometricCdf("1", "15", "10", "NaN"), "NaN");
    CHECK(CMath::hypergeometricCdf("-1", "15", "10", "5"), "0");
    CHECK(CMath::hypergeometricCdf("1", "15", "10", "5"), "0.01698301698301698302");
    CHECK_PRECISE(CMath::hypergeometricCdf("1", "15", "10", "5"), "0.01698301698301698301698301698301698301698301698302");

    CHECK(CMath::hypergeometricMean("NaN", "NaN", "NaN"), "NaN");
    CHECK(CMath::hypergeometricMean("NaN", "NaN", "5"), "NaN");
    CHECK(CMath::hypergeometricMean("15", "10", "NaN"), "NaN");
    CHECK(CMath::hypergeometricMean("15", "10", "5"), "3.33333333333333333333");
    CHECK_PRECISE(CMath::hypergeometricMean("15", "10", "5"), "3.33333333333333333333333333333333333333333333333333");

    CHECK(CMath::hypergeometricVariance("NaN", "NaN", "NaN"), "NaN");
    CHECK(CMath::hypergeometricVariance("NaN", "NaN", "5"), "NaN");
    CHECK(CMath::hypergeometricVariance("15", "10", "NaN"), "NaN");
    CHECK(CMath::hypergeometricVariance("15", "10", "5"), "0.79365079365079365079");
    CHECK_PRECISE(CMath::hypergeometricVariance("15", "10", "5"), "0.79365079365079365079365079365079365079365079365079");

    CHECK(CMath::poissonPmf("NaN", "NaN"), "NaN");
    CHECK(CMath::poissonPmf("NaN", "5"), "NaN");
    CHECK(CMath::poissonPmf("2", "NaN"), "NaN");
    CHECK(CMath::poissonPmf("-2", "5"), "0");
    CHECK(CMath::poissonPmf("2", "5"), "0.08422433748856833871");
    CHECK_PRECISE(CMath::poissonPmf("2", "5"), "0.08422433748856833870795060528935530311061981284194");

    CHECK(CMath::poissonCdf("NaN", "NaN"), "NaN");
    CHECK(CMath::poissonCdf("NaN", "5"), "NaN");
    CHECK(CMath::poissonCdf("2", "NaN"), "NaN");
    CHECK(CMath::poissonCdf("-2", "5"), "0");
    CHECK(CMath::poissonCdf("2", "5"), "0.12465201948308114129");
    CHECK_PRECISE(CMath::poissonCdf("2", "5"), "0.12465201948308114128776689582824584860371732300607");

    CHECK(CMath::poissonMean("NaN"), "NaN");
    CHECK(CMath::poissonMean("5"), "5");
    CHECK(CMath::poissonMean("-5"), "NaN");
    CHECK_PRECISE(CMath::poissonMean("5"), "5.00000000000000000000000000000000000000000000000000");

    CHECK(CMath::poissonVariance("NaN"), "NaN");
    CHECK(CMath::poissonVariance("5"), "5");
    CHECK(CMath::poissonVariance("-5"), "NaN");
    CHECK_PRECISE(CMath::poissonVariance("5"), "5.00000000000000000000000000000000000000000000000000");

    CHECK(CMath::ln("NaN"), "NaN");
    CHECK(CMath::ln("-0.1"), "-2.30258509299404568402+3.14159265358979323846j");
    CHECK(CMath::ln("0"), "NaN");
    CHECK(CMath::ln("0.00000000001"), "-25.3284360229345025242");
    CHECK_PRECISE(CMath::ln("0.1"), "-2.30258509299404568401799145468436420760110148862877");
    CHECK_PRECISE(CMath::ln("0.2"), "-1.60943791243410037460075933322618763952560135426852");
    CHECK_PRECISE(CMath::ln("0.3"), "-1.20397280432593599262274621776183850295361093080602");
    CHECK_PRECISE(CMath::ln("0.4"), "-0.91629073187415506518352721176801107145010121990826");
    CHECK_PRECISE(CMath::ln("0.5"), "-0.69314718055994530941723212145817656807550013436026");
    CHECK_PRECISE(CMath::ln("0.6"), "-0.51082562376599068320551409630366193487811079644577");
    CHECK_PRECISE(CMath::ln("0.7"), "-0.35667494393873237891263871124118447796401675904691");
    CHECK_PRECISE(CMath::ln("0.8"), "-0.22314355131420975576629509030983450337460108554801");
    CHECK_PRECISE(CMath::ln("0.9"), "-0.10536051565782630122750098083931279830612037298327");
    CHECK_PRECISE(CMath::ln("1.0"), "0");
    CHECK_PRECISE(CMath::ln("1.1"), "0.09531017980432486004395212328076509222060536530864");
    CHECK_PRECISE(CMath::ln("1.2"), "0.18232155679395462621171802515451463319738933791449");
    CHECK_PRECISE(CMath::ln("1.3"), "0.26236426446749105203549598688095439720416645613143");
    CHECK_PRECISE(CMath::ln("1.4"), "0.33647223662121293050459341021699209011148337531334");
    CHECK_PRECISE(CMath::ln("1.5"), "0.40546510810816438197801311546434913657199042346249");
    CHECK_PRECISE(CMath::ln("1.6"), "0.47000362924573555365093703114834206470089904881225");
    CHECK_PRECISE(CMath::ln("1.7"), "0.53062825106217039623154316318876232798710152395697");
    CHECK_PRECISE(CMath::ln("1.8"), "0.58778666490211900818973114061886376976937976137698");
    CHECK_PRECISE(CMath::ln("1.9"), "0.64185388617239477599103597720348932963627777267036");
    CHECK_PRECISE(CMath::ln("2.0"), "0.69314718055994530941723212145817656807550013436026");
    CHECK_PRECISE(CMath::ln("3.0"), "1.09861228866810969139524523692252570464749055782275");
    CHECK_PRECISE(CMath::ln("4.0"), "1.38629436111989061883446424291635313615100026872051");
    CHECK_PRECISE(CMath::ln("100"), "4.60517018598809136803598290936872841520220297725755");

    CHECK(CMath::lg("NaN"), "NaN");
    CHECK(CMath::lg("-1"), "1.36437635384184134749j");
    CHECK(CMath::lg("0"), "NaN");
    CHECK(CMath::lg("1e-5"), "-5");
    CHECK(CMath::lg("1e-4"), "-4");
    CHECK(CMath::lg("1e-3"), "-3");
    CHECK(CMath::lg("10"), "1");
    CHECK(CMath::lg("100"), "2");
    CHECK(CMath::lg("1000"), "3");
    CHECK(CMath::lg("10000"), "4");
    CHECK(CMath::lg("1e5"), "5");
    CHECK(CMath::lg("1e6"), "6");
    CHECK(CMath::lg("1e7"), "7");
    CHECK(CMath::lg("1e8"), "8");
    CHECK(CMath::lg("1e9"), "9");
    CHECK(CMath::lg("1e10"), "10");
    CHECK(CMath::lg("0.00000000001"), "-11");

    CHECK(CMath::lb("NaN"), "NaN");
    CHECK(CMath::lb("-1"), "4.53236014182719380963j");
    CHECK(CMath::lb("0"), "NaN");
    CHECK(CMath::lb("1"), "0");
    CHECK(CMath::lb("2"), "1");
    CHECK(CMath::lb("4"), "2");
    CHECK(CMath::lb("8"), "3");
    CHECK(CMath::lb("16"), "4");
    CHECK(CMath::lb("32"), "5");
    CHECK(CMath::lb("0.00000000001"), "-36.54120904376098582657");

    CHECK(CMath::log("NaN", "NaN"), "NaN");
    CHECK(CMath::log("1", "NaN"), "NaN");
    CHECK(CMath::log("NaN", "1"), "NaN");
    CHECK(CMath::log("123", "1234"), "1.47916531845224081828");
    CHECK(CMath::log("0.123", "0.1234"), "0.99845065797473594741");
    CHECK(CMath::log("0.00000000001", "0.00000000000001"), "1.27272727272727272727");

    CHECK(CMath::sin("NaN"), "NaN");
    CHECK(CMath::sin("0"), "0");
    CHECK(CMath::sin(PI), "0");
    CHECK(CMath::sin(PI*2), "0");
    CHECK(CMath::sin(PI*(-1)), "0");
    CHECK(CMath::sin(PI*(-3)), "0");
    CHECK(CMath::sin(PI*6/3), "0");
    CHECK(CMath::sin(PI*9/3), "0");
    CHECK(CMath::sin(PI/2), "1");
    CHECK(CMath::sin(PI/4), "0.7071067811865475244");
    CHECK(CMath::sin(PI/3), "0.86602540378443864676");
    CHECK(CMath::sin(PI*2/3), "0.86602540378443864676");
    CHECK(CMath::sin(PI*4/3), "-0.86602540378443864676");
    CHECK(CMath::sin(PI*5/3), "-0.86602540378443864676");
    CHECK(CMath::sin(PI*7/3), "0.86602540378443864676");
    CHECK_PRECISE(CMath::sin("0.0"), "0");
    CHECK_PRECISE(CMath::sin("0.1"), "0.09983341664682815230681419841062202698991538801798");
    CHECK_PRECISE(CMath::sin("0.2"), "0.19866933079506121545941262711838975037020672954021");
    CHECK_PRECISE(CMath::sin("0.3"), "0.29552020666133957510532074568502737367783211174262");
    CHECK_PRECISE(CMath::sin("0.4"), "0.38941834230865049166631175679570526459306018344396");
    CHECK_PRECISE(CMath::sin("0.5"), "0.47942553860420300027328793521557138808180336794060");
    CHECK_PRECISE(CMath::sin("0.6"), "0.56464247339503535720094544565865790710988808499415");
    CHECK_PRECISE(CMath::sin("0.7"), "0.64421768723769105367261435139872018306581384457369");
    CHECK_PRECISE(CMath::sin("0.8"), "0.71735609089952276162717461058138536619278523779142");
    CHECK_PRECISE(CMath::sin("0.9"), "0.78332690962748338846138231571354862314014792572031");
    CHECK_PRECISE(CMath::sin("1.0"), "0.84147098480789650665250232163029899962256306079837");
    CHECK_PRECISE(CMath::sin("2.0"), "0.90929742682568169539601986591174484270225497144789");
    CHECK_PRECISE(CMath::sin("3.0"), "0.14112000805986722210074480280811027984693326425227");
    CHECK_PRECISE(CMath::sin("4.0"), "-0.75680249530792825137263909451182909413591288733647");
    CHECK_PRECISE(CMath::sin("5.0"), "-0.95892427466313846889315440615599397335246154396460");
    CHECK_PRECISE(CMath::sin("1e22"), "-0.85220084976718880177270589375302936826176215041004");

    CHECK(CMath::cos("NaN"), "NaN");
    CHECK(CMath::cos(PI/2), "0");
    CHECK(CMath::cos(PI*3/2), "0");
    CHECK(CMath::cos(PI/2*(-1)*9), "0");
    CHECK(CMath::cos(0), "1");
    CHECK(CMath::cos(PI*6/3), "1");
    CHECK(CMath::cos(PI*9/3), "-1");
    CHECK(CMath::cos(PI/1), "-1");
    CHECK(CMath::cos(PI/3), "0.5");
    CHECK(CMath::cos(PI*2/3), "-0.5");
    CHECK(CMath::cos(PI*4/3), "-0.5");
    CHECK(CMath::cos(PI*5/3), "0.5");
    CHECK(CMath::cos(PI*7/3), "0.5");
    CHECK(CMath::cos(PI/4), "0.7071067811865475244");
    CHECK_PRECISE(CMath::cos("0.0"), "1.00000000000000000000000000000000000000000000000000");
    CHECK_PRECISE(CMath::cos("0.1"), "0.99500416527802576609556198780387029483857622541508");
    CHECK_PRECISE(CMath::cos("0.2"), "0.98006657784124163112419651674816887739352436080657");
    CHECK_PRECISE(CMath::cos("0.3"), "0.95533648912560601964231022756804989824421408263204");
    CHECK_PRECISE(CMath::cos("0.4"), "0.92106099400288508279852673205180161402585956931985");
    CHECK_PRECISE(CMath::cos("0.5"), "0.87758256189037271611628158260382965199164519710974");
    CHECK_PRECISE(CMath::cos("0.6"), "0.82533561490967829724095249895537603887809103918847");
    CHECK_PRECISE(CMath::cos("0.7"), "0.76484218728448842625585999019186490926821055037370");
    CHECK_PRECISE(CMath::cos("0.8"), "0.69670670934716542092074998164232492610178601370806");
    CHECK_PRECISE(CMath::cos("0.9"), "0.62160996827066445648471615140713350872176136659124");
    CHECK_PRECISE(CMath::cos("1.0"), "0.54030230586813971740093660744297660373231042061792");
    CHECK_PRECISE(CMath::cos("2.0"), "-0.41614683654714238699756822950076218976600077107554");
    CHECK_PRECISE(CMath::cos("3.0"), "-0.98999249660044545727157279473126130239367909661559");
    CHECK_PRECISE(CMath::cos("4.0"), "-0.65364362086361191463916818309775038142413359664622");
    CHECK_PRECISE(CMath::cos("1e22"), "0.52321478539513894549759447338470949214091997243939");

    CHECK(CMath::tan("NaN"), "NaN");
    CHECK(CMath::tan(PI/2), "NaN");
    CHECK(CMath::tan(PI/2*3), "NaN");
    CHECK(CMath::tan(PI/2*5), "NaN");
    CHECK(CMath::tan(PI/2*(-3)), "NaN");
    CHECK(CMath::tan(PI/2*(-5)), "NaN");
    CHECK(CMath::tan(PI/1), "0");
    CHECK(CMath::tan(PI*(-3)), "0");
    CHECK(CMath::tan(PI*11), "0");
    CHECK(CMath::tan(PI/4), "1");
    CHECK(CMath::tan(PI/3), "1.73205080756887729353");
    CHECK_PRECISE(CMath::tan("0.0"), "0");
    CHECK_PRECISE(CMath::tan("0.1"), "0.10033467208545054505808004578111153681900480457644");
    CHECK_PRECISE(CMath::tan("0.2"), "0.20271003550867248332135827164753448262687566965163");
    CHECK_PRECISE(CMath::tan("0.3"), "0.30933624960962323303530367969829466725781590680046");
    CHECK_PRECISE(CMath::tan("0.4"), "0.42279321873816176198163542716529033394198977271569");
    CHECK_PRECISE(CMath::tan("0.5"), "0.54630248984379051325517946578028538329755172017979");
    CHECK_PRECISE(CMath::tan("0.6"), "0.68413680834169231707092541746333574524265408075678");
    CHECK_PRECISE(CMath::tan("0.7"), "0.84228838046307944812813500221293771718722125080420");
    CHECK_PRECISE(CMath::tan("0.8"), "1.02963855705036401274636117282036528416821960677231");
    CHECK_PRECISE(CMath::tan("0.9"), "1.26015821755033913713457548539574847783362583439629");
    CHECK_PRECISE(CMath::tan("1.0"), "1.55740772465490223050697480745836017308725077238152");
    CHECK_PRECISE(CMath::tan("2.0"), "-2.18503986326151899164330610231368254343201774622766");
    CHECK_PRECISE(CMath::tan("3.0"), "-0.14254654307427780529563541053391349322609228490180");
    CHECK_PRECISE(CMath::tan("4.0"), "1.15782128234957758313734241826732392311976276736714");

    CHECK(CMath::cot("NaN"), "NaN");
    CHECK(CMath::cot(0), "NaN");
    CHECK(CMath::cot(PI), "NaN");
    CHECK(CMath::cot(PI*2), "NaN");
    CHECK(CMath::cot(PI*3), "NaN");
    CHECK(CMath::cot(PI*(-1)), "NaN");
    CHECK(CMath::cot(PI*(-2)), "NaN");
    CHECK(CMath::cot(PI*(-3)), "NaN");
    CHECK(CMath::cot(PI/2), "0");
    CHECK(CMath::cot(PI/2*3), "0");
    CHECK(CMath::cot(PI/2*5), "0");
    CHECK(CMath::cot(PI/(-2)), "0");
    CHECK(CMath::cot(PI/(-2)*3), "0");
    CHECK(CMath::cot(PI/(-2)*5), "0");
    CHECK_PRECISE(CMath::cot("0.1"), "9.96664442325923785979411268927059390763024832915145");
    CHECK_PRECISE(CMath::cot("0.2"), "4.93315487558689365736801632174474118540562176228750");
    CHECK_PRECISE(CMath::cot("0.3"), "3.23272814376582751371392053451257979612306377207793");
    CHECK_PRECISE(CMath::cot("0.4"), "2.36522242003911058702332902504860335138937304631794");
    CHECK_PRECISE(CMath::cot("0.5"), "1.83048772171245191926801943896881662375810794801613");
    CHECK_PRECISE(CMath::cot("0.6"), "1.46169594707810214033930842740714256443262393263873");
    CHECK_PRECISE(CMath::cot("0.7"), "1.18724183212667935367236269369115744209994867903861");
    CHECK_PRECISE(CMath::cot("0.8"), "0.97121460065047441252084679894165650872369163680112");
    CHECK_PRECISE(CMath::cot("0.9"), "0.79355114784231712550449309982343541572162687887550");
    CHECK_PRECISE(CMath::cot("1.0"), "0.64209261593433070300641998659426562023027811391817");
    CHECK_PRECISE(CMath::cot("2.0"), "-0.45765755436028576375027741043204727642848632923167");
    CHECK_PRECISE(CMath::cot("3.0"), "-7.01525255143453346942855137952647657829310335209635");
    CHECK_PRECISE(CMath::cot("4.0"), "0.86369115445061661394651434594081763350176570849799");

    CHECK(CMath::sec("NaN"), "NaN");
    CHECK(CMath::sec(0), "1");
    CHECK(CMath::sec(PI), "-1");
    CHECK(CMath::sec(PI*2), "1");
    CHECK(CMath::sec(PI*3), "-1");
    CHECK(CMath::sec(PI*(-1)), "-1");
    CHECK(CMath::sec(PI*(-2)), "1");
    CHECK(CMath::sec(PI*(-3)), "-1");
    CHECK(CMath::sec(PI/2), "NaN");
    CHECK(CMath::sec(PI/2*3), "NaN");
    CHECK(CMath::sec(PI/2*5), "NaN");
    CHECK(CMath::sec(PI/(-2)), "NaN");
    CHECK(CMath::sec(PI/(-2)*3), "NaN");
    CHECK(CMath::sec(PI/(-2)*5), "NaN");
    CHECK_PRECISE(CMath::sec("0.1"), "1.00502091840045542846511410131163581939665289709269");
    CHECK_PRECISE(CMath::sec("0.2"), "1.02033884494119268979243277684919018946197757380999");
    CHECK_PRECISE(CMath::sec("0.3"), "1.04675160153808560093279084558703692841456893499328");
    CHECK_PRECISE(CMath::sec("0.4"), "1.08570442838323870311392995515876310121797526730283");
    CHECK_PRECISE(CMath::sec("0.5"), "1.13949392732454912231332776820494992842372524604900");
    CHECK_PRECISE(CMath::sec("0.6"), "1.21162831451231670461455512137241473057527998567989");
    CHECK_PRECISE(CMath::sec("0.7"), "1.30745925973359386987467283530531145422836345495873");
    CHECK_PRECISE(CMath::sec("0.8"), "1.43532419967223980049695240816284402319218304405383");
    CHECK_PRECISE(CMath::sec("0.9"), "1.60872581046604951302401286230639310432053967212693");
    CHECK_PRECISE(CMath::sec("1.0"), "1.85081571768092561791175324139865019347039665509401");
    CHECK_PRECISE(CMath::sec("2.0"), "-2.40299796172238098975460040142006622624512109315453");
    CHECK_PRECISE(CMath::sec("3.0"), "-1.01010866590799375130303648146319295518501902819060");
    CHECK_PRECISE(CMath::sec("4.0"), "-1.52988565646639757462951092293735504703880002436905");

    CHECK(CMath::csc("NaN"), "NaN");
    CHECK(CMath::csc(0), "NaN");
    CHECK(CMath::csc(PI), "NaN");
    CHECK(CMath::csc(PI*2), "NaN");
    CHECK(CMath::csc(PI*3), "NaN");
    CHECK(CMath::csc(PI*(-1)), "NaN");
    CHECK(CMath::csc(PI*(-2)), "NaN");
    CHECK(CMath::csc(PI*(-3)), "NaN");
    CHECK(CMath::csc(PI/2), "1");
    CHECK(CMath::csc(PI/2*3), "-1");
    CHECK(CMath::csc(PI/2*5), "1");
    CHECK(CMath::csc(PI/(-2)), "-1");
    CHECK(CMath::csc(PI/(-2)*3), "1");
    CHECK(CMath::csc(PI/(-2)*5), "-1");
    CHECK_PRECISE(CMath::csc("0.1"), "10.01668613163477664870635254208068373689462246452006");
    CHECK_PRECISE(CMath::csc("0.2"), "5.03348954767234420242609636752585272222462656686394");
    CHECK_PRECISE(CMath::csc("0.3"), "3.38386336182412258496047655147742034395467222595556");
    CHECK_PRECISE(CMath::csc("0.4"), "2.56793245554778307034468729669613783401624871596957");
    CHECK_PRECISE(CMath::csc("0.5"), "2.08582964293348818577250167545929030196230958681696");
    CHECK_PRECISE(CMath::csc("0.6"), "1.77103219668772537337461210710543723169043983943920");
    CHECK_PRECISE(CMath::csc("0.7"), "1.55227032695710391198997364095492783063640855362864");
    CHECK_PRECISE(CMath::csc("0.8"), "1.39400781938863617450248222610694684266568140951681");
    CHECK_PRECISE(CMath::csc("0.9"), "1.27660621345889549601564565223201364314354454128779");
    CHECK_PRECISE(CMath::csc("1.0"), "1.18839510577812121626159945237455100352782983409796");
    CHECK_PRECISE(CMath::csc("2.0"), "1.09975017029461646675669739702631289665876444314985");
    CHECK_PRECISE(CMath::csc("3.0"), "7.08616739573718591821753227246127986736644022513951");
    CHECK_PRECISE(CMath::csc("4.0"), "-1.32134870881090237769679175637286490993025203772967");

    CHECK(CMath::arctan("NaN"), "NaN");
    CHECK(CMath::arctan("0.10033467208545054505808004578111153681900480457644"), "0.1");
    CHECK(CMath::arctan("0.20271003550867248332135827164753448262687566965163"), "0.2");
    CHECK(CMath::arctan("0.30933624960962323303530367969829466725781590680046"), "0.3");
    CHECK(CMath::arctan("0.42279321873816176198163542716529033394198977271569"), "0.4");
    CHECK(CMath::arctan("0.54630248984379051325517946578028538329755172017979"), "0.5");
    CHECK(CMath::arctan("0.68413680834169231707092541746333574524265408075678"), "0.6");
    CHECK(CMath::arctan("0.84228838046307944812813500221293771718722125080420"), "0.7");
    CHECK(CMath::arctan("1.02963855705036401274636117282036528416821960677231"), "0.8");
    CHECK(CMath::arctan("1.26015821755033913713457548539574847783362583439629"), "0.9");
    CHECK(CMath::arctan("1.55740772465490223050697480745836017308725077238152"), "1");
    CHECK_PRECISE(CMath::arctan("0.0"), "0");
    CHECK_PRECISE(CMath::arctan("0.1"), "0.09966865249116202737844611987802059024327832250431");
    CHECK_PRECISE(CMath::arctan("0.2"), "0.19739555984988075837004976519479029344758510378785");
    CHECK_PRECISE(CMath::arctan("0.3"), "0.29145679447786709199560462143289119350316759901207");
    CHECK_PRECISE(CMath::arctan("0.4"), "0.38050637711236488630358791681043310449740571365810");
    CHECK_PRECISE(CMath::arctan("0.5"), "0.46364760900080611621425623146121440202853705428612");
    CHECK_PRECISE(CMath::arctan("0.6"), "0.54041950027058415544357836460859991013514825146259");
    CHECK_PRECISE(CMath::arctan("1.0"), "0.78539816339744830961566084581987572104929234984378");
    CHECK_PRECISE(CMath::arctan("-0.1"), "-0.09966865249116202737844611987802059024327832250431");
    CHECK_PRECISE(CMath::arctan("-0.2"), "-0.19739555984988075837004976519479029344758510378785");
    CHECK_PRECISE(CMath::arctan("-0.3"), "-0.29145679447786709199560462143289119350316759901207");
    CHECK_PRECISE(CMath::arctan("-0.4"), "-0.38050637711236488630358791681043310449740571365810");
    CHECK_PRECISE(CMath::arctan("-0.5"), "-0.46364760900080611621425623146121440202853705428612");
    CHECK_PRECISE(CMath::arctan("-0.6"), "-0.54041950027058415544357836460859991013514825146259");
    CHECK_PRECISE(CMath::arctan("-1.0"), "-0.78539816339744830961566084581987572104929234984378");

    CHECK(CMath::arcsin("NaN"), "NaN");
    CHECK(CMath::arcsin("-1"), "-1.57079632679489661923");
    CHECK(CMath::arcsin("0"), "0");
    CHECK(CMath::arcsin("1"), "1.57079632679489661923");
    CHECK(CMath::arcsin("0.09983341664682815230681419841062202698991538801798"), "0.1");
    CHECK(CMath::arcsin("0.19866933079506121545941262711838975037020672954021"), "0.2");
    CHECK(CMath::arcsin("0.29552020666133957510532074568502737367783211174262"), "0.3");
    CHECK(CMath::arcsin("0.38941834230865049166631175679570526459306018344396"), "0.4");
    CHECK(CMath::arcsin("0.47942553860420300027328793521557138808180336794060"), "0.5");
    CHECK(CMath::arcsin("0.56464247339503535720094544565865790710988808499415"), "0.6");
    CHECK(CMath::arcsin("0.64421768723769105367261435139872018306581384457369"), "0.7");
    CHECK(CMath::arcsin("0.71735609089952276162717461058138536619278523779142"), "0.8");
    CHECK_PRECISE(CMath::arcsin("0.0"), "0");
    CHECK_PRECISE(CMath::arcsin("0.1"), "0.10016742116155979634552317945269331856867597222963");
    CHECK_PRECISE(CMath::arcsin("0.2"), "0.20135792079033079145512555221762341024003808140223");
    CHECK_PRECISE(CMath::arcsin("0.3"), "0.30469265401539750797200296122752916695456003170678");
    CHECK_PRECISE(CMath::arcsin("0.4"), "0.41151684606748801938473789761733560485570113512703");

    CHECK(CMath::arccos("NaN"), "NaN");
    CHECK(CMath::arccos("-1"), "3.14159265358979323846");
    CHECK(CMath::arccos("0"), "1.57079632679489661923");
    CHECK(CMath::arccos("1"), "0");
    CHECK_PRECISE(CMath::arccos("0.1"), "1.47062890563333682288579851218705812352990872745792");
    CHECK_PRECISE(CMath::arccos("0.2"), "1.36943840600456582777619613942212803185854661828532");
    CHECK_PRECISE(CMath::arccos("0.3"), "1.26610367277949911125931873041222227514402466798078");
    CHECK_PRECISE(CMath::arccos("0.4"), "1.15927948072740859984658379402241583724288356456053");

    CHECK(CMath::sinh("NaN"), "NaN");
    CHECK_PRECISE(CMath::sinh("0.1"), "0.10016675001984402582372938352190502351492091687856");
    CHECK_PRECISE(CMath::sinh("0.2"), "0.20133600254109398762556824301031737297449484262574");
    CHECK_PRECISE(CMath::sinh("0.3"), "0.30452029344714261895843526700509522909802423268018");
    CHECK_PRECISE(CMath::sinh("0.4"), "0.41075232580281550854021001384469810435315092436331");
    CHECK_PRECISE(CMath::sinh("0.5"), "0.52109530549374736162242562641149155910592898261148");
    CHECK_PRECISE(CMath::sinh("0.6"), "0.63665358214824127112345437546514831902496342592790");
    CHECK_PRECISE(CMath::sinh("0.7"), "0.75858370183953350345987464759276815415493761421703");
    CHECK_PRECISE(CMath::sinh("0.8"), "0.88810598218762300657471757318975698055970959688815");
    CHECK_PRECISE(CMath::sinh("0.9"), "1.02651672570817527595833616197842235379403446513485");
    CHECK_PRECISE(CMath::sinh("1.0"), "1.17520119364380145688238185059560081515571798133410");

    CHECK(CMath::cosh("NaN"), "NaN");
    CHECK_PRECISE(CMath::cosh("0.1"), "1.00500416805580359898797844296834164470962627785896");
    CHECK_PRECISE(CMath::cosh("0.2"), "1.02006675561907584629550375162935679733308609889476");
    CHECK_PRECISE(CMath::cosh("0.3"), "1.04533851412886048502530904632291210128027546467919");
    CHECK_PRECISE(CMath::cosh("0.4"), "1.08107237183845480928464293899252417629013184957412");
    CHECK_PRECISE(CMath::cosh("0.5"), "1.12762596520638078522622516140267201254784711809867");
    CHECK_PRECISE(CMath::cosh("0.6"), "1.18546521824226770375191329269771619435727538261853");
    CHECK_PRECISE(CMath::cosh("0.7"), "1.25516900563094301816467474099029711586260477992884");
    CHECK_PRECISE(CMath::cosh("0.8"), "1.33743494630484459800481995820531977649392453816033");
    CHECK_PRECISE(CMath::cosh("0.9"), "1.43308638544877438784179040162404834162773784130523");
    CHECK_PRECISE(CMath::cosh("1.0"), "1.54308063481524377847790562075706168260152911236586");

    CHECK(CMath::tanh("NaN"), "NaN");
    CHECK_PRECISE(CMath::tanh("0.1"), "0.09966799462495581711830508367835218353896209577673");
    CHECK_PRECISE(CMath::tanh("0.2"), "0.19737532022490400073815731881101566838937268384235");
    CHECK_PRECISE(CMath::tanh("0.3"), "0.29131261245159090581822127282376592815359680491761");
    CHECK_PRECISE(CMath::tanh("0.4"), "0.37994896225522488526774812389687331051318437437140");
    CHECK_PRECISE(CMath::tanh("0.5"), "0.46211715726000975850231848364367254873028928033011");
    CHECK_PRECISE(CMath::tanh("0.6"), "0.53704956699803528586182530492689670598284198771923");
    CHECK_PRECISE(CMath::tanh("0.7"), "0.60436777711716349630868718310382647501556664741686");
    CHECK_PRECISE(CMath::tanh("0.8"), "0.66403677026784896368484465640024285007465529270892");
    CHECK_PRECISE(CMath::tanh("0.9"), "0.71629787019902442081144378305809486317486516460076");
    CHECK_PRECISE(CMath::tanh("1.0"), "0.76159415595576488811945828260479359041276859725794");

    CHECK(CMath::arcosh("NaN"), "NaN");
    CHECK_PRECISE(CMath::arcosh("1.00500416805580359898797844296834164470962627785896"), "0.10000000000000000000000000000000000000000000000000");
    CHECK_PRECISE(CMath::arcosh("1.02006675561907584629550375162935679733308609889476"), "0.20000000000000000000000000000000000000000000000000");
    CHECK_PRECISE(CMath::arcosh("1.04533851412886048502530904632291210128027546467919"), "0.30000000000000000000000000000000000000000000000001");
    CHECK_PRECISE(CMath::arcosh("1.08107237183845480928464293899252417629013184957412"), "0.40000000000000000000000000000000000000000000000000");
    CHECK_PRECISE(CMath::arcosh("1.12762596520638078522622516140267201254784711809867"), "0.50000000000000000000000000000000000000000000000000");
    CHECK_PRECISE(CMath::arcosh("1.18546521824226770375191329269771619435727538261853"), "0.60000000000000000000000000000000000000000000000000");
    CHECK_PRECISE(CMath::arcosh("1.25516900563094301816467474099029711586260477992884"), "0.70000000000000000000000000000000000000000000000000");
    CHECK_PRECISE(CMath::arcosh("1.33743494630484459800481995820531977649392453816033"), "0.79999999999999999999999999999999999999999999999999");
    CHECK_PRECISE(CMath::arcosh("1.43308638544877438784179040162404834162773784130523"), "0.90000000000000000000000000000000000000000000000000");
    CHECK_PRECISE(CMath::arcosh("1.54308063481524377847790562075706168260152911236586"), "1.00000000000000000000000000000000000000000000000000");

    CHECK(CMath::artanh("NaN"), "NaN");
    CHECK_PRECISE(CMath::artanh("0.09966799462495581711830508367835218353896209577673"), "0.10000000000000000000000000000000000000000000000000");
    CHECK_PRECISE(CMath::artanh("0.19737532022490400073815731881101566838937268384235"), "0.20000000000000000000000000000000000000000000000000");
    CHECK_PRECISE(CMath::artanh("0.29131261245159090581822127282376592815359680491761"), "0.30000000000000000000000000000000000000000000000000");
    CHECK_PRECISE(CMath::artanh("0.37994896225522488526774812389687331051318437437140"), "0.40000000000000000000000000000000000000000000000000");
    CHECK_PRECISE(CMath::artanh("0.46211715726000975850231848364367254873028928033011"), "0.50000000000000000000000000000000000000000000000000");
    CHECK_PRECISE(CMath::artanh("0.53704956699803528586182530492689670598284198771923"), "0.60000000000000000000000000000000000000000000000000");
    CHECK_PRECISE(CMath::artanh("0.60436777711716349630868718310382647501556664741686"), "0.70000000000000000000000000000000000000000000000000");
    CHECK_PRECISE(CMath::artanh("0.66403677026784896368484465640024285007465529270892"), "0.80000000000000000000000000000000000000000000000001");
    CHECK_PRECISE(CMath::artanh("0.71629787019902442081144378305809486317486516460076"), "0.90000000000000000000000000000000000000000000000000");
    CHECK_PRECISE(CMath::artanh("0.76159415595576488811945828260479359041276859725794"), "1.00000000000000000000000000000000000000000000000001");

    CHECK(CMath::decodeIeee754("NaN", "NaN", "NaN"), "NaN");
    CHECK(CMath::decodeIeee754("1", "-1", "1"), "NaN");
    CHECK(CMath::decodeIeee754("1", "1", "0"), "NaN");
    CHECK(CMath::decodeIeee754("1", "1.5", "1"), "NaN");
    CHECK(CMath::decodeIeee754("0", "5", "10"), "0");
    CHECK(CMath::decodeIeee754("0x7ff00000", "8", "23"), "NaN");
    CHECK(CMath::decodeIeee754("0xffc0feed", "8", "23"), "NaN");
    CHECK(CMath::decodeIeee754("0x7fffffff", "8", "23"), "NaN");
    CHECK(CMath::decodeIeee754("0x8000", "5", "10"), "0");
    CHECK_PRECISE(CMath::decodeIeee754("0x00800000", "8", "23"), "0.00000000000000000000000000000000000001175494350822");
    CHECK_PRECISE(CMath::decodeIeee754("0x006f6eed", "8", "23"), "0.00000000000000000000000000000000000001023353274603");
    CHECK(CMath::decodeIeee754("0x41200000", "8", "23"), "10");
    CHECK(CMath::decodeIeee754("0xc1200000", "8", "23"), "-10");
    CHECK(CMath::decodeIeee754("0x3fc00000", "8", "23"), "1.5");
    CHECK_PRECISE(CMath::decodeIeee754("0x3dcccccd", "8", "23"), "0.10000000149011611938476562500000000000000000000000");
    CHECK(CMath::decodeIeee754("0x7f7fffff", "8", "23"), "340282346638528859811704183484516925440");
    CHECK(CMath::decodeIeee754("0x418100b9", "8", "23"), "16.1253528594970703125");
    CHECK(CMath::decodeIeee754("0x4024000000000000", "11", "52"), "10");
    CHECK(CMath::decodeIeee754("0x01", "4", "3", "-2"), "1");
    CHECK(CMath::decodeIeee754("0x07", "4", "3", "-2"), "7");
    CHECK(CMath::decodeIeee754("0x08", "4", "3", "-2"), "8");
    CHECK(CMath::decodeIeee754("0xf1", "4", "3", "-2"), "-73728");
    CHECK(CMath::decodeIeee754("0x77", "4", "3", "-2"), "122880");
    CHECK(CMath::decodeIeee754("0x5", "2", "1"), "3");
    CHECK_PRECISE(CMath::decodeIeee754("0x3ffd5555555555555555555555555555", "15", "112"), "0.33333333333333333333333333333333331728391713010637");

    CHECK(CMath::encodeIeee754("NaN", "NaN", "NaN"), "NaN");
    CHECK(CMath::encodeIeee754("1", "NaN", "NaN"), "NaN");
    CHECK(CMath::encodeIeee754("1", "-1", "1"), "NaN");
    CHECK(CMath::encodeIeee754("1", "1", "0"), "NaN");
    CHECK_FORMAT(Format::Hexadecimal() + Format::Fixed(), CMath::encodeIeee754("NaN", "5", "10"), "0x7FFF");
    CHECK_FORMAT(Format::Hexadecimal() + Format::Fixed(), CMath::encodeIeee754("NaN", "11", "52"), "0x7FFFFFFFFFFFFFFF");
    CHECK_FORMAT(Format::Hexadecimal() + Format::Fixed(), CMath::encodeIeee754("0", "5", "10"), "0");
    CHECK_FORMAT(Format::Hexadecimal() + Format::Fixed(), CMath::encodeIeee754("1.17549435E-38", "8", "23"), "0x800000");
    CHECK_FORMAT(Format::Hexadecimal() + Format::Fixed(), CMath::encodeIeee754("10", "8", "23"), "0x41200000");
    CHECK_FORMAT(Format::Hexadecimal() + Format::Fixed(), CMath::encodeIeee754("-10", "8", "23"), "0xC1200000");
    CHECK_FORMAT(Format::Hexadecimal() + Format::Fixed(), CMath::encodeIeee754("1.5", "8", "23"), "0x3FC00000");
    CHECK_FORMAT(Format::Hexadecimal() + Format::Fixed(), CMath::encodeIeee754("340282346638528859811704183484516925440","8", "23"), "0x7F7FFFFF");
    CHECK_FORMAT(Format::Hexadecimal() + Format::Fixed(), CMath::encodeIeee754("9999999999999999999999999999999999", "5", "10"), "0x7C00");
    CHECK_FORMAT(Format::Hexadecimal() + Format::Fixed(), CMath::encodeIeee754("-0.1", "8", "23"), "0xBDCCCCCD");
    CHECK_FORMAT(Format::Hexadecimal() + Format::Fixed(), CMath::encodeIeee754("-0.1", "11", "52"), "0xBFB999999999999A");
    CHECK_FORMAT(Format::Hexadecimal() + Format::Fixed(), CMath::encodeIeee754("16.1253528594970703125", "8", "23"), "0x418100B9");
    CHECK_FORMAT(Format::Hexadecimal() + Format::Fixed(), CMath::encodeIeee754("1", "4", "3", "-2"), "0x1");
    CHECK_FORMAT(Format::Hexadecimal() + Format::Fixed(), CMath::encodeIeee754("7", "4", "3", "-2"), "0x7");
    CHECK_FORMAT(Format::Hexadecimal() + Format::Fixed(), CMath::encodeIeee754("8", "4", "3", "-2"), "0x8");
    CHECK_FORMAT(Format::Hexadecimal() + Format::Fixed(), CMath::encodeIeee754("-73728", "4", "3", "-2"), "0xF1");
    CHECK_FORMAT(Format::Hexadecimal() + Format::Fixed(), CMath::encodeIeee754("122880", "4", "3", "-2"), "0x77");
    CHECK_FORMAT(Format::Hexadecimal() + Format::Fixed(), CMath::encodeIeee754("3", "2", "1"), "0x5");
    CHECK_FORMAT(Format::Hexadecimal() + Format::Fixed(), CMath::encodeIeee754("1.5", "2", "1"), "0x3");
}

void test_branches()
{
    /* Test behavior of complex functions relative to branch cuts */

    /* artanh */
    /* Branch cuts: ]-inf;-1], [1;+inf[ */
    CHECK(CMath::artanh("1.5-0.0000001j"),  "0.8047189562170405873-1.57079624679489661923j");
    CHECK(CMath::artanh("1.5"),             "0.8047189562170501873-1.57079632679489661923j");
    CHECK(CMath::artanh("1.5+0.0000001j"),  "0.8047189562170405873+1.57079624679489661923j");
    CHECK(CMath::artanh("-1.5-0.0000001j"), "-0.8047189562170405873-1.57079624679489661923j");
    CHECK(CMath::artanh("-1.5"),            "-0.8047189562170501873+1.57079632679489661923j");
    CHECK(CMath::artanh("-1.5+0.0000001j"), "-0.8047189562170405873+1.57079624679489661923j");

    /* arcosh */
    /* Branch cut: ]-inf;-1] */
    CHECK(CMath::arcosh("-1.5-0.0000001j"), "0.96242365011921226156-3.14159256414707413847j");
    CHECK(CMath::arcosh("-1.5"),            "0.962423650119206895+3.14159265358979323846j");
    CHECK(CMath::arcosh("-1.5+0.0000001j"), "0.96242365011921226156+3.14159256414707413847j");
    CHECK(CMath::arcosh("1.5-0.0000001j"),  "0.96242365011921226156-0.00000008944271909999j");
    CHECK(CMath::arcosh("1.5"),             "0.962423650119206895");
    CHECK(CMath::arcosh("1.5+0.0000001j"),  "0.96242365011921226156+0.00000008944271909999j");

    /* arsinh */
    /* Branch cuts: ]-j*inf;-j], ]j;j*inf] */
    CHECK(CMath::arsinh("-0.0000001+1.5j"), "-0.96242365011921226156+1.57079623735217751924j");
    CHECK(CMath::arsinh("1.5j"),            "0.962423650119206895+1.57079632679489661923j");
    CHECK(CMath::arsinh("0.0000001+1.5j"),  "0.96242365011921226156+1.57079623735217751924j");
    CHECK(CMath::arsinh("-0.0000001-1.5j"), "-0.96242365011921226156-1.57079623735217751924j");
    CHECK(CMath::arsinh("-1.5j"),           "-0.962423650119206895-1.57079632679489661923j");
    CHECK(CMath::arsinh("0.0000001+1.5j"),  "0.96242365011921226156+1.57079623735217751924j");
}

int main(int, char**)
{
    cmath_total_tests  = 0;
    cmath_failed_tests = 0;

    floatmath_init();
    PI = CMath::pi();

    test_create();
    test_format();
    test_op();
    test_functions();
    test_branches();

    if (!cmath_failed_tests)
        return 0;
    cout << cmath_total_tests  << " total, "
         << cmath_failed_tests << " failed, "
         << cmath_new_failed_tests << " new" << endl;
    return cmath_new_failed_tests;
}
