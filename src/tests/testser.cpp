// This file is part of the SpeedCrunch project
// Copyright (C) 2015-2016 Hadrien Theveneau <theveneau@gmail.com>
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

#include "math/hmath.h"
#include "math/cmath.h"
#include "math/units.h"
#include "math/quantity.h"
#include "tests/testcommon.h"

#include <cstdlib>
#include <string>
#include <iostream>
#include <QJsonDocument>

using namespace std;

typedef Quantity::Format Format;

#define CHECK_SER(x,y) check_ser(__FILE__, __LINE__, #x, x, y)
#define CHECK_DESER_HNUMBER(x,y) check_deser_hnumber(__FILE__, __LINE__, #x, x, y)
#define CHECK_DESER_CNUMBER(x,y) check_deser_cnumber(__FILE__, __LINE__, #x, x, y)
#define CHECK_DESER_QUANTITY(x,y) check_deser_quantity(__FILE__, __LINE__, #x, x, y)
#define CHECK_SER_DESER_HNUMBER(x,y) check_ser_deser_hnumber(__FILE__, __LINE__, #x, x, y)
#define CHECK_SER_DESER_CNUMBER(x,y) check_ser_deser_cnumber(__FILE__, __LINE__, #x, x, y)

static int ser_total_tests = 0;
static int ser_failed_tests = 0;
static int ser_new_failed_tests = 0;

template <class T>
void check_ser(const char* file, int line, const char* msg, const T& num, const char* expected, int issue = 0) {
    /* Serialization */
    QJsonObject obj;
    num.serialize(obj);
    QJsonDocument doc(obj);
    auto array = doc.toJson(QJsonDocument::Compact);
#if QT_VERSION < QT_VERSION_CHECK(5, 3, 0)
    // toJson(Compact) inserts a space after colons in Qt 5.2, see QTBUG-36682.
    array.replace(": ", ":");
#endif
    string result(array.data());
    /* Test result and display info */
    ++ser_total_tests;
    DisplayErrorOnMismatch(file, line, msg, result, expected, ser_failed_tests, ser_new_failed_tests, issue);
}

void check_deser_hnumber(const char* file, int line, const char* msg, const char* str,  const char* expected, int issue = 0) {
    /* Deserialization */
    QJsonDocument doc = QJsonDocument::fromJson(str);
    HNumber num(doc.object());
    /* Test result and display info */
    ++ser_total_tests;
    string result = HMath::format(num, Format::Fixed()).toStdString();
    DisplayErrorOnMismatch(file, line, msg, result, expected, ser_failed_tests, ser_new_failed_tests, issue);
}

void check_deser_cnumber(const char* file, int line, const char* msg, const char* str,  const char* expected, int issue = 0) {
    /* Deserialization */
    QJsonDocument doc = QJsonDocument::fromJson(str);
    CNumber num(doc.object());
    /* Test result and display info */
    ++ser_total_tests;
    string result = CMath::format(num, Format::Fixed()).toStdString();
    DisplayErrorOnMismatch(file, line, msg, result, expected, ser_failed_tests, ser_new_failed_tests, issue);
}

void check_deser_quantity(const char* file, int line, const char* msg, const char* str,  const char* expected, int issue = 0) {
    /* Deserialization */
    QJsonDocument doc = QJsonDocument::fromJson(str);
    Quantity q(doc.object());
    /* Test result and display info */
    ++ser_total_tests;
    string result = DMath::format(q, Format::Fixed()).toStdString();
    DisplayErrorOnMismatch(file, line, msg, result, expected, ser_failed_tests, ser_new_failed_tests, issue);
}

void check_ser_deser_hnumber(const char* file, int line, const char* msg, const HNumber& src, const char* expected, int issue = 0) {
    /* Serialization + deserialization */
    QJsonObject obj;
    src.serialize(obj);
    HNumber dest = HNumber::deSerialize(obj);
    /* Test result and display info */
    string result = HMath::format(dest, Format::General() + Format::Precision(50)).toStdString();
    ++ser_total_tests;
    DisplayErrorOnMismatch(file, line, msg, result, expected, ser_failed_tests, ser_new_failed_tests, issue);
}

void check_ser_deser_cnumber(const char* file, int line, const char* msg, const CNumber& src, const char* expected, int issue = 0) {
    /* Serialization + deserialization */
    QJsonObject obj;
    src.serialize(obj);
    CNumber dest = CNumber::deSerialize(obj);
    /* Test result and display info */
    string result = CMath::format(dest, Format::General() + Format::Precision(50)).toStdString();
    DisplayErrorOnMismatch(file, line, msg, result, expected, ser_failed_tests, ser_new_failed_tests, issue);
}

int main(int, char**)
{
    /* Serialization tests */
    CHECK_SER(HNumber("3"), "{\"value\":\"3.000000000000000000000000000000000000000000000000000000000000000000000000000000\"}");

    CHECK_SER(CNumber("3"), "{\"value\":\"3.000000000000000000000000000000000000000000000000000000000000000000000000000000\"}");
    CHECK_SER(CNumber("3+1j"), "{\"value\":\"3.000000000000000000000000000000000000000000000000000000000000000000000000000000+1.000000000000000000000000000000000000000000000000000000000000000000000000000000j\"}");

    Quantity a(CNumber("3"));
    a.setFormat(Format::Binary() + Format::Fixed());
    a *= Units::meter();
    a.setDisplayUnit(CNumber("0.3"), QString("foot"));
    const char q_json_blob[] = "{\"dimension\":{\"length\":\"1\"},\"format\":{\"base\":\"Binary\",\"mode\":\"Fixed\"},\"numeric_value\":{\"value\":\"3.000000000000000000000000000000000000000000000000000000000000000000000000000000\"},\"unit\":{\"value\":\"0.300000000000000000000000000000000000000000000000000000000000000000000000000000\"},\"unit_name\":\"foot\"}";
    CHECK_SER(a, q_json_blob);

    Quantity b(CNumber("1+1j"));
    b.setFormat(Format::Polar());
    const char q_json_blob2[] = "{\"format\":{\"form\":\"Polar\"},\"numeric_value\":{\"value\":\"1.000000000000000000000000000000000000000000000000000000000000000000000000000000+1.000000000000000000000000000000000000000000000000000000000000000000000000000000j\"}}";
    CHECK_SER(b, q_json_blob2);

    /* HNumber deserialization tests */
    CHECK_DESER_HNUMBER("{\"format\": \"g\",\"value\": \"1\"}", "1");
    CHECK_DESER_HNUMBER("{\"format\": \"g\",""\"value\": \"0.1\"}", "0.1");
    /* CNumber deserialization tests */
    CHECK_DESER_CNUMBER("{\"format\": \"g\",\"value\": \"1\"}", "1");
    CHECK_DESER_CNUMBER("{\"format\": \"g\",\"value\": \"0.1\"}", "0.1");
    CHECK_DESER_CNUMBER("{\"format\": \"g\",\"value\": \"0.0+1.0j\"}", "1j");
    /* Quantity deserialization tests */
    CHECK_DESER_QUANTITY(q_json_blob, "0b1010 foot");
    CHECK_DESER_QUANTITY(q_json_blob2, "1.4142135623730950488 * exp(j*0.78539816339744830962)");

    /* Serialization + deserialization tests */
    CHECK_SER_DESER_HNUMBER(HNumber("3"), "3.00000000000000000000000000000000000000000000000000");
    CHECK_SER_DESER_CNUMBER(CNumber("3"), "3.00000000000000000000000000000000000000000000000000");
    CHECK_SER_DESER_CNUMBER(CNumber("3+1j"), "3.00000000000000000000000000000000000000000000000000+1.00000000000000000000000000000000000000000000000000j");


    /* Output test satistics */
    if (!ser_failed_tests)
        return 0;
    cout << ser_total_tests  << " total, "
         << ser_failed_tests << " failed, "
         << ser_new_failed_tests << " new" << endl;
    return ser_new_failed_tests;
}

