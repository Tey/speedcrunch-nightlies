// This file is part of the SpeedCrunch project
// Copyright (C) 2004-2006 Ariya Hidayat <ariya@kde.org>
// Copyright (C) 2007-2009, 2013, 2016 @heldercorreia
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

#include "core/evaluator.h"
#include "core/settings.h"
#include "tests/testcommon.h"

#include <QtCore/QCoreApplication>

#include <string>
#include <iostream>

using namespace std;

typedef Quantity::Format Format;

static Evaluator* eval = 0;
static int eval_total_tests = 0;
static int eval_failed_tests = 0;
static int eval_new_failed_tests = 0;

#define CHECK_AUTOFIX(s,p) checkAutoFix(__FILE__,__LINE__,#s,s,p)
#define CHECK_DIV_BY_ZERO(s) checkDivisionByZero(__FILE__,__LINE__,#s,s)
#define CHECK_EVAL(x,y) checkEval(__FILE__,__LINE__,#x,x,y)
#define CHECK_EVAL_KNOWN_ISSUE(x,y,n) checkEval(__FILE__,__LINE__,#x,x,y,n)
#define CHECK_EVAL_PRECISE(x,y) checkEvalPrecise(__FILE__,__LINE__,#x,x,y)
#define CHECK_EVAL_FAIL(x) checkEval(__FILE__,__LINE__,#x,x,"",0,true)
#define CHECK_USERFUNC_SET(x) checkEval(__FILE__,__LINE__,#x,x,"NaN")
#define CHECK_USERFUNC_SET_FAIL(x) checkEval(__FILE__,__LINE__,#x,x,"",0,true)

static void checkAutoFix(const char* file, int line, const char* msg, const char* expr, const char* fixed)
{
    ++eval_total_tests;

    string r = eval->autoFix(QString(expr)).toStdString();
    DisplayErrorOnMismatch(file, line, msg, r, fixed, eval_failed_tests, eval_new_failed_tests);
}

static void checkDivisionByZero(const char* file, int line, const char* msg, const QString& expr)
{
    ++eval_total_tests;

    eval->setExpression(expr);
    Quantity rn = eval->evalUpdateAns();

    if (eval->error().isEmpty()) {
        ++eval_failed_tests;
        cerr << file << "[" << line << "]\t" << msg << endl
             << "\tError: " << "division by zero not caught" << endl;
    }
}

static void checkEval(const char* file, int line, const char* msg, const QString& expr, const char* expected, int issue = 0, bool shouldFail = false)
{
    ++eval_total_tests;

    eval->setExpression(expr);
    Quantity rn = eval->evalUpdateAns();

    if (!eval->error().isEmpty()) {
        if (!shouldFail) {
            ++eval_failed_tests;
            cerr << file << "[" << line << "]\t" << msg;
            if (issue)
                cerr << "\t[ISSUE " << issue << "]";
            else {
                cerr << "\t[NEW]";
                ++eval_new_failed_tests;
            }
            cerr << endl;
            cerr << "\tError: " << qPrintable(eval->error()) << endl;
        }
    } else {
        QString result = DMath::format(rn, Format::Fixed());
        if (shouldFail || result != expected) {
            ++eval_failed_tests;
            cerr << file << "[" << line << "]\t" << msg;
            if (issue)
                cerr << "\t[ISSUE " << issue << "]";
            else {
                cerr << "\t[NEW]";
                ++eval_new_failed_tests;
            }
            cerr << endl;
            cerr << "\tResult   : " << result.toLatin1().constData() << endl
                 << "\tExpected : " << (shouldFail ? "should fail" : expected) << endl;
        }
    }
}

static void checkEvalPrecise(const char* file, int line, const char* msg, const QString& expr, const char* expected)
{
    ++eval_total_tests;

    eval->setExpression(expr);
    Quantity rn = eval->evalUpdateAns();

    // We compare up to 50 decimals, not exact number because it's often difficult
    // to represent the result as an irrational number, e.g. PI.
    string result = DMath::format(rn, Format::Fixed() + Format::Precision(50)).toStdString();
    DisplayErrorOnMismatch(file, line, msg, result, expected, eval_failed_tests, eval_new_failed_tests, 0);
}

void test_constants()
{
    CHECK_EVAL("1", "1");
}

void test_exponentiation()
{
    CHECK_EVAL("2e10", "20000000000");
    CHECK_EVAL("2E10", "20000000000");
    CHECK_EVAL("2e-10", "0.0000000002");
    CHECK_EVAL("2E0xa", "20000000000");
    CHECK_EVAL("2e-0xa", "0.0000000002");
    CHECK_EVAL("0b10b10", "8");
    CHECK_EVAL("0b10B10", "8");
    CHECK_EVAL("0b10b-10", "0.5");
    CHECK_EVAL("0b10b0d2", "8");
    CHECK_EVAL("0b10b-0d2", "0.5");
    CHECK_EVAL("0o2o10", "33554432");
    CHECK_EVAL("0o2O10", "33554432");
    CHECK_EVAL("0o2C10", "33554432");
    CHECK_EVAL("0o2o-10", "0.00000011920928955078");
    CHECK_EVAL("0o2o0d8", "33554432");
    CHECK_EVAL("0o2o-0d8", "0.00000011920928955078");
    CHECK_EVAL("0x2h10", "36893488147419103232");
    CHECK_EVAL("0x2H10", "36893488147419103232");
    CHECK_EVAL("0x2h-10", "0.00000000000000000011");
    CHECK_EVAL("0x2h0d16", "36893488147419103232");
    CHECK_EVAL("0x2h-0d16", "0.00000000000000000011");
    CHECK_EVAL_FAIL("0x2o11");
    CHECK_EVAL_FAIL("0b2C11");
    CHECK_EVAL_FAIL("0o2b11");
}

void test_unary()
{
    CHECK_EVAL("-0", "0");
    CHECK_EVAL("-1", "-1");
    CHECK_EVAL("--1", "1");
    CHECK_EVAL("---1", "-1");
    CHECK_EVAL("----1", "1");
    CHECK_EVAL("-----1", "-1");
    CHECK_EVAL("------1", "1");

    CHECK_EVAL("-ABS(1)", "-1");
    CHECK_EVAL("-ABS(-2)", "-2");
    CHECK_EVAL("--ABS(-3)", "3");
    CHECK_EVAL("---ABS(-4)", "-4");

    // Operator ^ has higher precedence than unary minus.
    CHECK_EVAL("-1^0", "-1");
    CHECK_EVAL("-1^1", "-1");
    CHECK_EVAL("-1^2", "-1");
    CHECK_EVAL("-1^3", "-1");

    CHECK_EVAL("-2^0", "-1");
    CHECK_EVAL("-2^1", "-2");
    CHECK_EVAL("-2^2", "-4");
    CHECK_EVAL("-2^3", "-8");
    CHECK_EVAL("-2^4", "-16");
    CHECK_EVAL("-2^5", "-32");
    CHECK_EVAL("-2^6", "-64");
    CHECK_EVAL("-2^7", "-128");
    CHECK_EVAL("-2^8", "-256");
    CHECK_EVAL("-2^9", "-512");
    CHECK_EVAL("-2^10", "-1024");

    CHECK_EVAL("(5-7)^2",  "4");
    CHECK_EVAL("-(5-7)^2", "-4");

    CHECK_EVAL("-cos(pi)^3", "1");
    CHECK_EVAL("1*(-cos(pi)^2)", "-1");

    CHECK_EVAL("3^3^3", "7625597484987");

    CHECK_EVAL("1/-1^2", "-1");
    CHECK_EVAL("1*-1^2", "-1");

    // Factorial has higher precedence than unary minus.
    CHECK_EVAL("-1!", "-1");
    CHECK_EVAL("-2!", "-2");
    CHECK_EVAL("-3!", "-6");

    // Unicode minus sign
    CHECK_EVAL("−1", "-1");
    CHECK_EVAL("−(2*3)", "-6");
    CHECK_EVAL("2*−1", "-2");
    CHECK_EVAL("2e−1", "0.2");
}

void test_binary()
{
    // See http://en.wikipedia.org/wiki/Empty_product.
    CHECK_EVAL("0^0", "NaN");

    CHECK_EVAL("1^0", "1");
    CHECK_EVAL("1^1", "1");
    CHECK_EVAL("1^2", "1");
    CHECK_EVAL("1^3", "1");

    CHECK_EVAL("2^0", "1");
    CHECK_EVAL("2^1", "2");
    CHECK_EVAL("2^2", "4");
    CHECK_EVAL("2^3", "8");
    CHECK_EVAL("2^4", "16");
    CHECK_EVAL("2^5", "32");
    CHECK_EVAL("2^6", "64");
    CHECK_EVAL("2^7", "128");
    CHECK_EVAL("2^8", "256");
    CHECK_EVAL("2^9", "512");
    CHECK_EVAL("2^10", "1024");

    CHECK_EVAL("0+0", "0");
    CHECK_EVAL("1+0", "1");
    CHECK_EVAL("0+1", "1");
    CHECK_EVAL("1+1", "2");

    CHECK_EVAL("0-0", "0");
    CHECK_EVAL("1-0", "1");
    CHECK_EVAL("0-1", "-1");
    CHECK_EVAL("1-1", "0");

    CHECK_EVAL("0−0", "0");
    CHECK_EVAL("1−0", "1");
    CHECK_EVAL("0−1", "-1");
    CHECK_EVAL("1−1", "0");

    CHECK_EVAL("2*3", "6");
    CHECK_EVAL("2×3", "6");
    CHECK_EVAL("2⋅3", "6"); // U+22C5 Dot operator.
    CHECK_EVAL("3*2", "6");
    CHECK_EVAL("3×2", "6");
    CHECK_EVAL("3⋅2", "6");

    CHECK_EVAL("10/2", "5");
    CHECK_EVAL("10÷2", "5");
    CHECK_EVAL("2/10", "0.2");
    CHECK_EVAL("2÷10", "0.2");

    // Check that parentheses are added in unit conversion results when needed
    CHECK_EVAL("1 meter -> 10 meter", "0.1 (10 meter)");
    CHECK_EVAL("1 meter -> .1 meter", "10 (.1 meter)");
    CHECK_EVAL("1 meter -> -1 meter", "-1 (-1 meter)");
    CHECK_EVAL("1 meter -> 0xa meter", "0.1 (0xa meter)");
    CHECK_EVAL("1 meter second -> 10 meter second", "0.1 (10 meter second)");
    CHECK_EVAL("1 meter second -> meter 10 second", "0.1 meter 10 second");
    CHECK_EVAL("1 meter second -> meter second 10", "0.1 (meter second 10)");
    CHECK_EVAL("1 meter -> meter + meter", "0.5 (meter + meter)");
    CHECK_EVAL("1 meter -> meter - 2meter", "-1 (meter - 2meter)");
    CHECK_EVAL("1 meter -> meter", "1 meter");
    CHECK_EVAL("1 (10 meter) -> meter", "10 meter");
}

void test_divide_by_zero()
{
    CHECK_DIV_BY_ZERO("1/0");
    CHECK_DIV_BY_ZERO("1 / sin 0");
    CHECK_DIV_BY_ZERO("1/sin(pi)");
    CHECK_DIV_BY_ZERO("1/sin (pi");
    CHECK_DIV_BY_ZERO("1/sin  pi");
    CHECK_DIV_BY_ZERO("1 / cos (pi/2)");
    CHECK_DIV_BY_ZERO("1 / cos(pi/2");
    CHECK_DIV_BY_ZERO("1/cos(pi/2)");
    CHECK_DIV_BY_ZERO("1/tan(0)");
    CHECK_DIV_BY_ZERO("1/tan 0");
    CHECK_DIV_BY_ZERO("-1/trunc(0.123)");
    CHECK_DIV_BY_ZERO("1/round(0.456)");
    CHECK_DIV_BY_ZERO("-1/binompmf(1;10;0)");
    CHECK_DIV_BY_ZERO("-345.3 / binompmf (1; 10; 0)");
}

void test_radix_char()
{
    // Backup current settings
    Settings* settings = Settings::instance();
    char radixCharacter = settings->radixCharacter();

    settings->setRadixCharacter('*');

    CHECK_EVAL("1+.5", "1.5");
    CHECK_EVAL("1+,5", "1.5");
    CHECK_EVAL(".5*,5", "0.25");
    CHECK_EVAL("1.01+2,02", "3.03");

    CHECK_EVAL("0b1.0 + 1", "2");
    CHECK_EVAL("0b1.1 * 1", "1.5");
    CHECK_EVAL("0b01.010 / 1", "1.25");
    CHECK_EVAL("-0b.010 - 1", "-1.25");

    CHECK_EVAL("0o.7 + 1", "1.875");
    CHECK_EVAL("-0o.7 + 1", "0.125");

    CHECK_EVAL("0x.f + 1", "1.9375");
    CHECK_EVAL("-0x.f + 1", "0.0625");

    CHECK_EVAL("1/.1", "10"); // ISSUE 151
    CHECK_EVAL("1/,1", "10"); // ISSUE 151

    // Test automatic detection of radix point when multiple choices are possible
    CHECK_EVAL("1,234.567", "1234.567");
    CHECK_EVAL("1.234,567", "1234.567");
    CHECK_EVAL("1,2,3", "123");
    CHECK_EVAL("1.2.3", "123");
    CHECK_EVAL("1,234,567.89", "1234567.89");
    CHECK_EVAL("1.234.567,89", "1234567.89");
    CHECK_EVAL("1,234.567,89", "1234.56789");
    CHECK_EVAL("1.234,567.89", "1234.56789");

    settings->setRadixCharacter('.');

    CHECK_EVAL("1+0.5", "1.5");
    CHECK_EVAL("1+0,5", "6");
    CHECK_EVAL("1/.1", "10");
    CHECK_EVAL("1/,1", "1");
    CHECK_EVAL("1,234.567", "1234.567");
    CHECK_EVAL("1.234,567", "1.234567");
    CHECK_EVAL("1,2,3", "123");
    CHECK_EVAL("1.2.3", "123");
    CHECK_EVAL("1,234,567.89", "1234567.89");
    CHECK_EVAL("1.234.567,89", "123456789");
    CHECK_EVAL("1,234.567,89", "1234.56789");
    CHECK_EVAL("1.234,567.89", "123456789");

    settings->setRadixCharacter(',');

    CHECK_EVAL("1+0.5", "6");
    CHECK_EVAL("1+0,5", "1.5");
    CHECK_EVAL("1/.1", "1");
    CHECK_EVAL("1/,1", "10");
    CHECK_EVAL("1,234.567", "1.234567");
    CHECK_EVAL("1.234,567", "1234.567");
    CHECK_EVAL("1,2,3", "123");
    CHECK_EVAL("1.2.3", "123");
    CHECK_EVAL("1,234,567.89", "123456789");
    CHECK_EVAL("1.234.567,89", "1234567.89");
    CHECK_EVAL("1,234.567,89", "123456789");
    CHECK_EVAL("1.234,567.89", "1234.56789");

    // Restore old settings
    settings->setRadixCharacter(radixCharacter);
}

void test_thousand_sep()
{
    CHECK_EVAL("12'345.678'9", "12345.6789");
    CHECK_EVAL("1234'5.67'89", "12345.6789");
    CHECK_EVAL("1234'56", "123456");
    CHECK_EVAL("'123456", "123456");
    CHECK_EVAL("123456'", "123456");
    CHECK_EVAL("123'''456", "123456");
    CHECK_EVAL(".'123456", "0.123456");

    CHECK_EVAL("12 345.678 9", "12345.6789");
    CHECK_EVAL("12_345.678_9", "12345.6789");
    CHECK_EVAL(QString::fromUtf8("12·345.678·9"), "12345.6789");
    CHECK_EVAL(QString::fromUtf8("12٫345.678٫9"), "12345.6789");
    CHECK_EVAL(QString::fromUtf8("12٬345.678٬9"), "12345.6789");
    CHECK_EVAL(QString::fromUtf8("12˙345.678˙9"), "12345.6789");
    CHECK_EVAL(QString::fromUtf8("12⎖345.678⎖9"), "12345.6789");

    CHECK_EVAL("12$345.678~9", "12345.6789");
    CHECK_EVAL("12`345.678@9", "12345.6789");
    CHECK_EVAL("$ 1234.567", "1234.567");
    CHECK_EVAL("1234.567 $", "1234.567");
    CHECK_EVAL("$-10", "-10");
    CHECK_EVAL("$+10", "10");
}

void test_function_basic()
{
    CHECK_EVAL("ABS(0)", "0");
    CHECK_EVAL("ABS(1)", "1");
    CHECK_EVAL("ABS(-1)", "1");
    CHECK_EVAL("ABS(--1)", "1");
    CHECK_EVAL("ABS(---1)", "1");
    CHECK_EVAL("ABS((1))", "1");
    CHECK_EVAL("ABS((-1))", "1");

    CHECK_EVAL("ABS(1/2)", "0.5");
    CHECK_EVAL("ABS(-1/2)", "0.5");
    CHECK_EVAL("ABS(-1/-2)", "0.5");
    CHECK_EVAL("ABS(1/-2)", "0.5");

    CHECK_EVAL("INT(0)", "0");
    CHECK_EVAL("INT(1)", "1");
    CHECK_EVAL("INT(-1)", "-1");
    CHECK_EVAL("INT(0.5)", "0");
    CHECK_EVAL("INT(-0.75)", "0");
    CHECK_EVAL("INT(-0.9999*1)", "0");
    CHECK_EVAL("INT(0.9999*1)", "0");
    CHECK_EVAL("INT(2.1)", "2");
    CHECK_EVAL("INT(-3.4)", "-3");

    CHECK_EVAL("log(0.123; 0.1234)", "0.99845065797473594741");
    CHECK_EVAL("lg(0.00000000001)", "-11");
    CHECK_EVAL("lg(1e-3)", "-3");
    CHECK_EVAL("lb(0.00000000001)", "-36.54120904376098582657");
    CHECK_EVAL("lb(32)", "5");
    CHECK_EVAL("ln(100)", "4.60517018598809136804");
    CHECK_EVAL("ln(4.0)", "1.38629436111989061883");

    CHECK_EVAL("0!", "1");
    CHECK_EVAL("1!", "1");
    CHECK_EVAL("2!", "2");
    CHECK_EVAL("3!", "6");
    CHECK_EVAL("4!", "24");
    CHECK_EVAL("5!", "120");
    CHECK_EVAL("6!", "720");
    CHECK_EVAL("7!", "5040");
    CHECK_EVAL("(1+1)!^2", "4");

    CHECK_EVAL("(-27)^(1/3)", "-3");
    CHECK_EVAL("(-27)^(-1/3)", "-0.33333333333333333333");

    CHECK_EVAL_PRECISE("exp((1)/2) + exp((1)/2)", "3.29744254140025629369730157562832714330755220142030");

    // Test functions composition
    CHECK_EVAL("log(10;log(10;1e100))", "2");
    CHECK_EVAL("log(10;abs(-100))", "2");
    CHECK_EVAL("abs(log(10;100))", "2");
    CHECK_EVAL("abs(abs(-100))", "100");
    CHECK_EVAL("sum(10;abs(-100);1)", "111");
    CHECK_EVAL("sum(abs(-100);10;1)", "111");
    CHECK_EVAL("sum(10;1;abs(-100))", "111");
}

void test_function_trig()
{
    CHECK_EVAL_PRECISE("pi", "3.14159265358979323846264338327950288419716939937511");

    CHECK_EVAL("sin(0)", "0");
    CHECK_EVAL("cos(0)", "1");
    CHECK_EVAL("tan(0)", "0");

    CHECK_EVAL("sin(pi)", "0");
    CHECK_EVAL("cos(pi)", "-1");
    CHECK_EVAL("tan(pi)", "0");

    CHECK_EVAL("sin(-pi)", "0");
    CHECK_EVAL("cos(-pi)", "-1");
    CHECK_EVAL("tan(-pi)", "0");

    CHECK_EVAL("sin(--pi)", "0");
    CHECK_EVAL("cos(--pi)", "-1");
    CHECK_EVAL("tan(--pi)", "0");

    CHECK_EVAL("sin(pi/2)", "1");
    CHECK_EVAL("cos(pi/2)", "0");

    CHECK_EVAL("sin(-pi/2)", "-1");
    CHECK_EVAL("cos(-pi/2)", "0");

    CHECK_EVAL("sin(-pi/2) + sin(pi/2)", "0");
    CHECK_EVAL("sin(-pi/2) - sin(pi/2)", "-2");
    CHECK_EVAL("cos(-pi/2) + cos(pi/2)", "0");
    CHECK_EVAL("cos(-pi/2) - cos(pi/2)", "0");

    CHECK_EVAL("arcsin(sin(1))", "1");
    CHECK_EVAL("arccos(cos(1))", "1");
    CHECK_EVAL("arctan(tan(1))", "1");
    CHECK_EVAL("arcsin(0)", "0");
    CHECK_EVAL("arccos(1)", "0");
    CHECK_EVAL("arctan(0)", "0");

    CHECK_EVAL("degrees(0)", "0");
    CHECK_EVAL("degrees(pi/2)", "90");
    CHECK_EVAL("degrees(pi)", "180");
    CHECK_EVAL("degrees(3*pi/2)", "270");
    CHECK_EVAL("degrees(2*pi)", "360");

    CHECK_EVAL("radians(0)", "0");
    CHECK_EVAL("radians(90)/pi", "0.5");
    CHECK_EVAL("radians(180)/pi", "1");
    CHECK_EVAL("radians(270)/pi", "1.5");
    CHECK_EVAL("radians(360)/pi", "2");
}

void test_function_stat()
{
    CHECK_EVAL_FAIL("MIN(0)");
    CHECK_EVAL("MIN(0; 1)", "0");
    CHECK_EVAL("MIN(0; 2)", "0");
    CHECK_EVAL("MIN(-1; 0)", "-1");
    CHECK_EVAL("MIN(-1; 1)", "-1");
    CHECK_EVAL("MIN(-0.01; 0)", "-0.01");
    CHECK_EVAL("MIN(0; 1; 2)", "0");
    CHECK_EVAL("MIN(-1; 0; 1; 2)", "-1");
    CHECK_EVAL("MIN(-2; -1; 0; 1; 2)", "-2");

    CHECK_EVAL_FAIL("MAX(0)");
    CHECK_EVAL("MAX(0; 1)", "1");
    CHECK_EVAL("MAX(0; 2)", "2");
    CHECK_EVAL("MAX(-1; 0)", "0");
    CHECK_EVAL("MAX(-1; 1)", "1");
    CHECK_EVAL("MAX(0.01; 0)", "0.01");
    CHECK_EVAL("MAX(0; 1; 2)", "2");
    CHECK_EVAL("MAX(-1; 0; 1; 2)", "2");
    CHECK_EVAL("MAX(-2; -1; 0; 1; 2)", "2");

    CHECK_EVAL_FAIL("SUM(1)");
    CHECK_EVAL("SUM(100;1)", "101");
    CHECK_EVAL("SUM(-100;1)", "-99");
    CHECK_EVAL("SUM(0;0;0)", "0");
    CHECK_EVAL("SUM(100;-1)", "99");
    CHECK_EVAL("SUM(-100;-1)", "-101");
    CHECK_EVAL("SUM(1;2;3;4;5;6)", "21");
    CHECK_EVAL("SUM(1;-2;3;-4;5;-6)", "-3");

    CHECK_EVAL_FAIL("PRODUCT(-1)");
    CHECK_EVAL("PRODUCT(100;0)", "0");
    CHECK_EVAL("PRODUCT(100;1)", "100");
    CHECK_EVAL("PRODUCT(-100;1)", "-100");
    CHECK_EVAL("PRODUCT(-100;-1)", "100");
    CHECK_EVAL("PRODUCT(1;1;1)", "1");
    CHECK_EVAL("PRODUCT(1;2;3;4;5;6)", "720");
    CHECK_EVAL("PRODUCT(1;-2;3;-4;5;-6)", "-720");

    CHECK_EVAL_FAIL("AVERAGE(0)");
    CHECK_EVAL("AVERAGE(0;0)", "0");
    CHECK_EVAL("AVERAGE(0;0;0)", "0");
    CHECK_EVAL("AVERAGE(0;1)", "0.5");
    CHECK_EVAL("AVERAGE(0;1;2)", "1");
    CHECK_EVAL("AVERAGE(0;1;0)*3", "1");
    CHECK_EVAL("AVERAGE(1;1;1)", "1");
    CHECK_EVAL("AVERAGE(2;2;2)", "2");
    CHECK_EVAL("AVERAGE(3;3;3)", "3");
    CHECK_EVAL("AVERAGE(0.25;0.75)", "0.5");
    CHECK_EVAL("AVERAGE(2.25;4.75)", "3.5");
    CHECK_EVAL("AVERAGE(1/3;2/3)", "0.5");

    CHECK_EVAL_FAIL("GEOMEAN(-1e20;0;-1)");
    CHECK_EVAL_FAIL("GEOMEAN(5)");
    CHECK_EVAL("GEOMEAN(1;1)", "1");
    CHECK_EVAL("GEOMEAN(1;4)", "2");
    CHECK_EVAL("GEOMEAN(4;9)", "6");
    CHECK_EVAL("GEOMEAN(3.6;8.1)", "5.4");
    CHECK_EVAL("GEOMEAN(3;4;18)", "6");
    CHECK_EVAL("GEOMEAN(1;1;1)", "1");
    CHECK_EVAL("GEOMEAN(1;1;1;1)", "1");
    CHECK_EVAL_FAIL("GEOMEAN(1;1;1;-1)");

    CHECK_EVAL("VARIANCE(1;-1)", "1");
    CHECK_EVAL("VARIANCE(5 meter; 13 meter)", "16 meter²");
    // for complex tests of VARIANCE see test_complex
}

void test_function_logic()
{
    CHECK_EVAL_FAIL("and(1)");
    CHECK_EVAL_FAIL("or(2)");
    CHECK_EVAL_FAIL("xor(3)");

    CHECK_EVAL("and(0;0)", "0");
    CHECK_EVAL("and(0;1)", "0");
    CHECK_EVAL("and(1;0)", "0");
    CHECK_EVAL("and(1;1)", "1");

    CHECK_EVAL("or(0;0)", "0");
    CHECK_EVAL("or(0;1)", "1");
    CHECK_EVAL("or(1;0)", "1");
    CHECK_EVAL("or(1;1)", "1");

    CHECK_EVAL("xor(0;0)", "0");
    CHECK_EVAL("xor(0;1)", "1");
    CHECK_EVAL("xor(1;0)", "1");
    CHECK_EVAL("xor(1;1)", "0");
}

void test_function_discrete()
{
    CHECK_EVAL("gcd(12;18)", "6");
    CHECK_EVAL("gcd(36;56;210)", "2");
    CHECK_EVAL("gcd(28;120;126)", "2");

    CHECK_EVAL("ncr(-3;-1)", "0");
    CHECK_EVAL("ncr(-3;0)", "1");
    CHECK_EVAL("ncr(-3;1)", "-3");
    CHECK_EVAL("ncr(-3;2)", "6");
    CHECK_EVAL("ncr(-3;3)", "-10");
    CHECK_EVAL("ncr(-3;4)", "15");
    CHECK_EVAL("ncr(-3;5)", "-21");
    CHECK_EVAL("ncr(-2;-1)", "0");
    CHECK_EVAL("ncr(-2;0)", "1");
    CHECK_EVAL("ncr(-2;1)", "-2");
    CHECK_EVAL("ncr(-2;2)", "3");
    CHECK_EVAL("ncr(-2;3)", "-4");
    CHECK_EVAL("ncr(-2;4)", "5");
    CHECK_EVAL("ncr(-2;5)", "-6");

    CHECK_EVAL("ncr(-1;-1)", "0");
    CHECK_EVAL("ncr(-1;0)", "1");
    CHECK_EVAL("ncr(-1;1)", "-1");
    CHECK_EVAL("ncr(-1;2)", "1");
    CHECK_EVAL("ncr(-1;3)", "-1");
    CHECK_EVAL("ncr(-1;4)", "1");
    CHECK_EVAL("ncr(-1;5)", "-1");

    CHECK_EVAL("ncr(0;-1)", "0");
    CHECK_EVAL("ncr(0;0)", "1");
    CHECK_EVAL("ncr(0;1)", "0");

    CHECK_EVAL("ncr(1;-1)", "0");
    CHECK_EVAL("ncr(1;0)", "1");
    CHECK_EVAL("ncr(1;1)", "1");
    CHECK_EVAL("ncr(1;2)", "0");

    CHECK_EVAL("ncr(2;-1)", "0");
    CHECK_EVAL("ncr(2;0)", "1");
    CHECK_EVAL("ncr(2;1)", "2");
    CHECK_EVAL("ncr(2;2)", "1");
    CHECK_EVAL("ncr(2;3)", "0");

    CHECK_EVAL("ncr(3;-1)", "0");
    CHECK_EVAL("ncr(3;0)", "1");
    CHECK_EVAL("ncr(3;1)", "3");
    CHECK_EVAL("ncr(3;2)", "3");
    CHECK_EVAL("ncr(3;3)", "1");
    CHECK_EVAL("ncr(3;4)", "0");

    CHECK_EVAL("ncr(4;-1)", "0");
    CHECK_EVAL("ncr(4;0)", "1");
    CHECK_EVAL("ncr(4;1)", "4");
    CHECK_EVAL("ncr(4;2)", "6");
    CHECK_EVAL("ncr(4;3)", "4");
    CHECK_EVAL("ncr(4;4)", "1");
    CHECK_EVAL("ncr(4;5)", "0");
}

void test_function_simplified()
{
    /* Tests for standard functions */
    CHECK_EVAL("abs 123", "123");
    CHECK_EVAL("abs -123", "123");       /* (issue #600) */
    CHECK_EVAL("10 + abs 123", "133");
    CHECK_EVAL("10 + abs -123", "133");  /* (issue #600) */
    CHECK_EVAL("abs 123 + 10", "133");
    CHECK_EVAL("abs -123 + 10", "133");  /* (issue #600) */
    CHECK_EVAL("10 * abs 123", "1230");
    CHECK_EVAL("abs 123 * 10", "1230");
    /* Tests for user functions (issue #600, cf. discussion) */
    CHECK_USERFUNC_SET("func2(x) = abs(x)");
    CHECK_EVAL("func2 123", "123");
    CHECK_EVAL("func2 -123", "123");
    CHECK_EVAL("10 + func2 123", "133");
    CHECK_EVAL("10 + func2 -123", "133");
    CHECK_EVAL("func2 123 + 10", "133");
    CHECK_EVAL("func2 -123 + 10", "133");
    CHECK_EVAL("10 * func2 123", "1230");
    CHECK_EVAL("func2 123 * 10", "1230");
    CHECK_USERFUNC_SET("f(x) = 5 meter + x"); /* (issue #656)  */
    CHECK_EVAL("f(2 meter)", "7 meter");      /* (issue #656)  */
    CHECK_EVAL_FAIL("f(2)");                  /* (issue #656)  */
    /* Tests for priority management (issue #451) */
    CHECK_EVAL("lg 10^2", "2");
    CHECK_EVAL("frac 3!",  "0");
}

void test_auto_fix_parentheses()
{
    CHECK_AUTOFIX("sin(1)", "sin(1)");
    CHECK_AUTOFIX("sin(1", "sin(1)");

    CHECK_AUTOFIX("x+(8-2", "x+(8-2)");
    CHECK_AUTOFIX("x+(8-2)", "x+(8-2)");

    CHECK_AUTOFIX("x+(8-(2*1", "x+(8-(2*1))");
    CHECK_AUTOFIX("x+(8-(2*1)", "x+(8-(2*1))");
    CHECK_AUTOFIX("x+(8-(2*1))", "x+(8-(2*1))");

    CHECK_AUTOFIX("x + sin (pi", "x + sin (pi)");
}

void test_auto_fix_ans()
{
    CHECK_AUTOFIX("sin", "sin(ans)");
    CHECK_AUTOFIX("cos", "cos(ans)");
    CHECK_AUTOFIX("tan", "tan(ans)");
    CHECK_AUTOFIX("abs", "abs(ans)");
    CHECK_AUTOFIX("exp", "exp(ans)");
    CHECK_AUTOFIX("lg", "lg(ans)");
}

void test_auto_fix_trailing_equal()
{
    CHECK_AUTOFIX("1+2=", "1+2");
    CHECK_AUTOFIX("1==", "1");
    CHECK_AUTOFIX("1/3====", "1/3");
    CHECK_AUTOFIX("sin(x+y)=", "sin(x+y)");
}

void test_auto_fix_untouch()
{
    CHECK_AUTOFIX("sin(x)", "sin(x)");
    CHECK_AUTOFIX("sin((x/y))", "sin((x/y))");
    CHECK_AUTOFIX("ans", "ans");
    CHECK_AUTOFIX("sin(ans)", "sin(ans)");
    CHECK_AUTOFIX("tan(ans)", "tan(ans)");
    CHECK_AUTOFIX("x=1.2", "x=1.2");
    CHECK_AUTOFIX("1/sin pi", "1/sin pi");
}

void test_auto_fix_powers()
{
    CHECK_AUTOFIX("3¹", "3^1");
    CHECK_AUTOFIX("3⁻¹", "3^(-1)");
    CHECK_AUTOFIX("3¹²³⁴⁵⁶⁷⁸⁹", "3^123456789");
    CHECK_AUTOFIX("3²⁰", "3^20");
    CHECK_AUTOFIX("7 + 3²⁰ * 4", "7 + 3^20 * 4");
}

void test_comments()
{
    CHECK_EVAL("ncr(3;3) ? this is because foo",  "1");
}

void test_user_functions()
{
    // Check user functions can be defined and used
    CHECK_USERFUNC_SET("func1(a;b) = a * b + 10");
    CHECK_EVAL("func1(2;5)", "20"); // = 2 * 5 + 10
    // Check some expected error conditions
    CHECK_EVAL_FAIL("func1()");
    CHECK_EVAL_FAIL("func1(2)");
    CHECK_EVAL_FAIL("func1(2;5;1)");

    // Check user functions can call other user functions
    CHECK_USERFUNC_SET("func2(a;b) = func1(a;b) - 10");
    CHECK_EVAL("func2(2;5)", "10"); // = (2 * 5 + 10) - 10

    // Check user functions can be redefined
    CHECK_USERFUNC_SET("func2(a;b) = 10 + func1(a;b)");
    CHECK_EVAL("func2(2;5)", "30"); // = 10 + (2 * 5 + 10)

    // Check user functions can refer to other user functions not defined
    CHECK_USERFUNC_SET("func2(a;b) = 10 * a + func3(a;b)");
    CHECK_EVAL_FAIL("func2(2;5)");
    CHECK_USERFUNC_SET("func3(a;b) = a - b");
    CHECK_EVAL("func2(2;5)", "17"); // = 10 * 2 + (2 - 5)

    // Check user functions can call builtin functions
    CHECK_USERFUNC_SET("func2(a;b) = sum(func1(a;b);5)");
    CHECK_EVAL("func2(2;5)", "25"); // = sum((2 * 5 + 10);5)

    // Check recursive user functions are not allowed
    CHECK_USERFUNC_SET("func_r(a) = a * func_r(a)");
    CHECK_EVAL_FAIL("func_r(1)");
    CHECK_USERFUNC_SET("func_r1(a) = a * func_r2(a)");
    CHECK_USERFUNC_SET("func_r2(a) = a + func_r1(a)");
    CHECK_EVAL_FAIL("func_r1(1)");

    // Check user functions can refer to user variables
    CHECK_USERFUNC_SET("func1(a;b) = a * b - var1");
    CHECK_EVAL_FAIL("func1(2;5)");
    CHECK_EVAL("var1 = 5", "5");
    CHECK_EVAL("func1(2;5)", "5");  // = 2 * 5 - 5

    // Check conflicts between the names of user functions arguments and user variables
    CHECK_EVAL("arg1 = 10", "10");
    CHECK_USERFUNC_SET("func1(arg1;arg2) = arg1 * arg2");
    CHECK_EVAL("func1(2;5)", "10"); // = 2 * 5 (not 10 * 5)

    // Check user functions names can not mask builtin functions
    CHECK_USERFUNC_SET_FAIL("sum(a;b) = a * b");

    // Check user functions names can not mask user variables
    CHECK_EVAL("var1 = 5", "5");
    CHECK_USERFUNC_SET_FAIL("var1(a;b) = a * b");

    // Check user functions can take no argument
    CHECK_EVAL("func1() = 2 * 10", "20");
    // NOTE: we use CHECK_EVAL to define the function, because its result is known at definition
    //       time. CHECK_USERFUNC_SET expects NaN, which is what is returned when the function body
    //       contains at least one component whose value can not be known at definition time
    //       (e.g., reference to user function arguments or to user/builtin functions).
    CHECK_EVAL("func1()", "20");    // = 2 * 5
}

void test_complex()
{
    // Check for basic complex number processing
    CHECK_EVAL("1j", "1j");
    CHECK_EVAL("0.1j", "0.1j");
    CHECK_EVAL(".1j", "0.1j");
    CHECK_EVAL("1E12j", "1000000000000j");
    CHECK_EVAL("0.1E12j", "100000000000j");
    CHECK_EVAL("1E-12j", "0.000000000001j");
    CHECK_EVAL("0.1E-12j", "0.0000000000001j");
    // Check for some bugs introduced by first versions of complex number processing
    CHECK_EVAL("0.1", "0.1");
    // Check for basic complex number evaluation
    CHECK_EVAL("(1+1j)*(1-1j)", "2");
    CHECK_EVAL("(1+1j)*(1+1j)", "2j");


    CHECK_EVAL("VARIANCE(1j;-1j)", "1");
    CHECK_EVAL("VARIANCE(1j;-1j;1;-1)", "1");
    CHECK_EVAL("VARIANCE(2j;-2j;1;-1)", "2.5");
}

void test_angle_mode(Settings* settings)
{
    settings->angleUnit = 'r';
    Evaluator::instance()->initializeAngleUnits();
    CHECK_EVAL("sin(pi)", "0");
    CHECK_EVAL("arcsin(-1)", "-1.57079632679489661923");
    CHECK_EVAL("sin(1j)", "1.17520119364380145688j");
    CHECK_EVAL("arcsin(-2)", "-1.57079632679489661923+1.31695789692481670863j");
    CHECK_EVAL("radian","1");
    CHECK_EVAL("degree","0.01745329251994329577");


    settings->angleUnit = 'd';
    Evaluator::instance()->initializeAngleUnits();
    CHECK_EVAL("sin(180)", "0");
    CHECK_EVAL("arcsin(-1)", "-90");
    CHECK_EVAL_FAIL("sin(1j)");
    CHECK_EVAL_FAIL("arcsin(-2)");
    CHECK_EVAL("radian","57.2957795130823208768");
    CHECK_EVAL("degree","1");
}

void test_implicit_multiplication()
{
    CHECK_EVAL("a = 5", "5");
    CHECK_EVAL("5a", "25");
    CHECK_EVAL("5.a", "25");
    CHECK_EVAL("5.0a", "25");
    CHECK_EVAL("5e2a", "2500");
    CHECK_EVAL_FAIL("a5");
    CHECK_EVAL("a 5", "25");
    CHECK_EVAL("2a^3", "250");
    CHECK_EVAL("b=2", "2");
    CHECK_EVAL_FAIL("ab");
    CHECK_EVAL("a b", "10");
    CHECK_EVAL("eps = 10", "10");
    CHECK_EVAL("5 eps", "50");
    CHECK_EVAL("Eren = 1001", "1001");
    CHECK_EVAL("7 Eren", "7007");
    CHECK_EVAL("Eren 5", "5005");
    CHECK_EVAL("f() = 123", "123");
    CHECK_EVAL("2f()", "246");
    CHECK_EVAL("5   5", "55");

    // Check implicit multiplication between numbers fails
    // CHECK_EVAL_FAIL("10.   0.2");
    CHECK_EVAL_FAIL("10 0x10");
    CHECK_EVAL_FAIL("10 #10");
    CHECK_EVAL_FAIL("0b104");
    CHECK_EVAL_FAIL("0b10 4");
    CHECK_EVAL_FAIL("0b10 0x4");
    CHECK_EVAL_FAIL("0o109");
    CHECK_EVAL_FAIL("0o10 9");
    CHECK_EVAL_FAIL("0o10 0x9");
    // CHECK_EVAL_FAIL("12.12.12");
    CHECK_EVAL_FAIL("12e12.12");
    CHECK_EVAL("0b10a", "10");
    CHECK_EVAL("0o2a", "10");
    CHECK_EVAL("5(5)", "25");

    CHECK_EVAL("a sin(pi/2)", "5");
    CHECK_EVAL("a sqrt(4)",   "10");
    CHECK_EVAL("a sqrt(a^2)", "25");

    /* Tests issue 538 */
    /* 3 sin (3 pi) was evaluated but not 3 sin (3) */
    CHECK_EVAL("3 sin (3 pi)", "0");
    CHECK_EVAL("3 sin (3)",    "0.4233600241796016663");

    CHECK_EVAL("2 (2 + 1)", "6");
    CHECK_EVAL("2 (a)", "10");

    /* Tests issue 598 */
    CHECK_EVAL("2(a)^3", "250");

    CHECK_EVAL_KNOWN_ISSUE("6/2(2+1)", "9", 741);
    CHECK_EVAL_KNOWN_ISSUE("2^2(2)", "8", 741);
    CHECK_EVAL_KNOWN_ISSUE("2^2(2)(2)", "16", 741);
    CHECK_EVAL_KNOWN_ISSUE("2^2(2*2)", "16", 741);
    CHECK_EVAL_KNOWN_ISSUE("2^2(2)+3", "11", 741);
    CHECK_EVAL_FAIL("pi (2)");
}

void test_format()
{
    CHECK_EVAL("bin(123)", "0b1111011");
    CHECK_EVAL("oct(123)", "0o173");
    CHECK_EVAL("hex(123)", "0x7B");

    CHECK_EVAL("polar(3+4j)", "5 * exp(j*0.92729521800161223243)");
}


int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    Settings* settings = Settings::instance();
    settings->angleUnit = 'r';
    settings->setRadixCharacter('.');
    settings->complexNumbers = false;
    DMath::complexMode = false;

    eval = Evaluator::instance();

    eval->initializeBuiltInVariables();

    test_constants();
    test_exponentiation();
    test_unary();
    test_binary();

    test_divide_by_zero();
    test_radix_char();

    test_thousand_sep();

    test_function_basic();
    test_function_trig();
    test_function_stat();
    test_function_logic();
    test_function_discrete();
    test_function_simplified();

    test_auto_fix_parentheses();
    test_auto_fix_ans();
    test_auto_fix_trailing_equal();
    test_auto_fix_powers();
    test_auto_fix_untouch();

    test_comments();

    test_user_functions();

    test_implicit_multiplication();

    settings->complexNumbers = true;
    DMath::complexMode = true;
    eval->initializeBuiltInVariables();
    test_complex();
    test_format();

    test_angle_mode(settings);

    if (!eval_failed_tests)
        return 0;
    cout << eval_total_tests  << " total, "
         << eval_failed_tests << " failed, "
         << eval_new_failed_tests << " new" << endl;
    return eval_new_failed_tests;
}
