// This file is part of the SpeedCrunch project
// Copyright (C) 2016 Pol Welter <polwelter@gmail.com>
// Copyright (C) 2016 @heldercorreia
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

#include "math/quantity.h"
#include "math/rational.h"
#include "math/units.h"
#include "tests/testcommon.h"

#include <QtCore/QCoreApplication>
#include <QString>

#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

typedef Quantity::Format Format;

#define CHECK(x,y) check_value(__FILE__,__LINE__,#x,x,y)
#define CHECK_FORMAT(f,x,y) check_format(__FILE__,__LINE__,#x,x,f,y)
#define CHECK_PRECISE(x,y) check_precise(__FILE__,__LINE__,#x,x,y)
#define CHECK_KNOWN_ISSUE(x,y,n) check_value(__FILE__,__LINE__,#x,x,y,n)
#define CHECK_STRING(x,y) {++dmath_total_tests; DisplayErrorOnMismatch(__FILE__,__LINE__,#x,x,y,dmath_failed_tests,dmath_new_failed_tests);}

static int dmath_total_tests  = 0;
static int dmath_failed_tests = 0;
static int dmath_new_failed_tests = 0;

static void check_value(const char* file, int line, const char* msg, const Quantity& q, const char* expected, int issue = 0)
{
    ++dmath_total_tests;
    string result = DMath::format(q, Format::Fixed()).toStdString();
    DisplayErrorOnMismatch(file, line, msg, result, expected, dmath_failed_tests, dmath_new_failed_tests, issue);
}

static void check_format(const char* file, int line, const char* msg, const Quantity& q, Format format, const char* expected)
{
    ++dmath_total_tests;
    string result = DMath::format(q, format).toStdString();
    DisplayErrorOnMismatch(file, line, msg, result, expected, dmath_failed_tests, dmath_new_failed_tests, 0);
}

//static void check_precise(const char* file, int line, const char* msg, const Quantity& q, const char* expected)
//{
//    ++dmath_total_tests;
//    string result = DMath::format(q, Format::Fixed() + Format::Precision(50)).toStdString();
//    DisplayErrorOnMismatch(file, line, msg, result, expected, dmath_failed_tests, dmath_new_failed_tests, 0);
//}

void test_rational()
{
    CHECK_STRING(HMath::format(Rational(123,456).toHNumber()).toStdString(), "0.26973684210526315789");
    CHECK_STRING(Rational(22./7).toString().toStdString(), "22/7");
    CHECK_STRING(Rational(-12345./96457).toString().toStdString(), "-12345/96457");
    CHECK_STRING(Rational(HNumber("-1234")/HNumber("7895")).toString().toStdString(), "-1234/7895");
    CHECK_STRING(Rational(HNumber("-1235000")/HNumber("78950000")).toString().toStdString(), "-247/15790");
    CHECK_STRING(Rational(0.).toString().toStdString(), "0");
    CHECK_STRING(Rational(HNumber(0)).toString().toStdString(), "0");
}

void test_create()
{
    CHECK(Units::meter(), "1 meter");
    CHECK(Quantity(CNumber("123.45+654j")), "123.45+654j");
    CHECK(Quantity(HNumber("123.45")), "123.45");
}

void test_basic()
{
    CHECK(Units::meter(), "1 meter");
    CHECK(Quantity(5) * Units::meter(), "5 meter");
    CHECK(Units::candela() + Units::second(), "NaN");
    CHECK(Quantity(3)*Units::mole() - Quantity(HNumber("2.5"))*Units::mole(), "0.5 mole");
    CHECK(Units::kilogram()/Units::second(), "1 kilogram second⁻¹");
    CHECK(Units::meter()*Units::meter(), "1 meter²");
    CHECK(-Quantity(5)*Units::meter(), "-5 meter");
    CNumber foot = CNumber("0.3");
    Quantity a(Quantity(123)*Units::meter());
    a.setDisplayUnit(foot, "foot");
    CHECK(a, "410 foot");
    CHECK(a*Units::second(), "123 meter second");       // Issue 615
    CHECK(a/Units::second(), "123 meter second⁻¹");    //
    CHECK(a*HNumber(5), "2050 foot");                   //
    CHECK(a/HNumber(5), "82 foot");                     //

    CHECK(DMath::raise(Units::meter(), 0),"1");
    CHECK(DMath::raise(Units::meter(), Quantity(0)),"1");
    CHECK(DMath::raise(Units::meter(), 0) + DMath::raise(Units::second(), 0),"2");
    CHECK(DMath::raise(Units::meter(), Quantity(0)) + DMath::raise(Units::second(), Quantity(0)),"2");
}

void test_functions()
{
    CHECK(DMath::abs(Quantity(CNumber("3+4j"))*Units::meter()), "5 meter");
    CHECK(DMath::round(CNumber("1.234"), 1), "1.2");
    CHECK(DMath::round(Quantity(CNumber("1.234"))*Units::joule(), 0), "NaN");

    CHECK(DMath::trunc(CNumber("1.274"), 1), "1.2");
    CHECK(DMath::trunc(Quantity(CNumber("1.234"))*Units::joule(), 0), "NaN");

    CHECK(DMath::real(Quantity(CNumber("3+4j"))*Units::meter()), "3 meter");
    CHECK(DMath::imag(Quantity(CNumber("3+4j"))*Units::meter()), "4 meter");

    CHECK(DMath::sqrt(Quantity(CNumber("36"))*Units::second()), "6 second^(1/2)");
    CHECK(DMath::cbrt(Quantity(CNumber("125"))*Units::second()), "5 second^(1/3)");

    CHECK(DMath::raise(Quantity(CNumber("2")), DMath::pi()), "8.82497782707628762386");
    CHECK(DMath::raise(Quantity(CNumber("2"))*Units::ampere(), DMath::pi()), "NaN");
    CHECK(DMath::raise(Quantity(CNumber("-2"))*Units::ampere(), Quantity(CNumber("1.5"))), "NaN");

    DMath::complexMode = false;
    CHECK(DMath::raise(Quantity(CNumber("-2"))*Units::ampere(), Quantity(CNumber("0.6"))), "-1.51571656651039808235 ampere^(3/5)");
    DMath::complexMode = true;
    CHECK(DMath::raise(Quantity(CNumber("-2"))*Units::ampere(), Quantity(CNumber("0.6"))), "(-0.46838217770735830743+1.44153211743623063689j) ampere^(3/5)");


    // this should do it for all wrapped functions that do not accept dimensional arguments...
    CHECK(DMath::sin(Quantity(DMath::pi())), "0");
    CHECK(DMath::sin(Units::meter()), "NaN");
}

void test_format()
{
    Quantity a = Quantity(CNumber("12365234.45647"));
    CHECK_FORMAT(Format::Binary() + Format::Fixed() + Format::Precision(10), a, "0b101111001010110110110010.0111010011");


    a *= Units::coulomb();
    CHECK_FORMAT(Format::Binary() + Format::Fixed() + Format::Precision(10), a, "0b101111001010110110110010.0111010011 coulomb");
}


int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    dmath_total_tests = 0;
    dmath_failed_tests = 0;

    test_rational();

    test_create();
    test_basic();
    test_functions();
    test_format();

    cerr.flush();
    if (!dmath_failed_tests)
        return 0;
    cout << dmath_total_tests  << " total, "
         << dmath_failed_tests << " failed, "
         << dmath_new_failed_tests << " new" << endl;
    return dmath_new_failed_tests;
}
