// This file is part of the SpeedCrunch project
// Copyright (C) 2013 @heldercorreia
// Copyright (C) 2015 Pol Welter <polwelter@gmail.com>
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

#include "core/numberformatter.h"

#include "core/settings.h"
#include "math/quantity.h"

static const QChar g_dotChar = QString::fromUtf8("⋅")[0];
static const QChar g_minusChar = QString::fromUtf8("−")[0];

QString NumberFormatter::format(Quantity q)
{
    Settings* settings = Settings::instance();

    Quantity::Format format = q.format();
    if (format.base == Quantity::Format::Base::Null) {
        switch (settings->resultFormat) {
        case 'b':
            format.base = Quantity::Format::Base::Binary;
            break;
        case 'o':
            format.base = Quantity::Format::Base::Octal;
            break;
        case 'h':
            format.base = Quantity::Format::Base::Hexadecimal;
            break;
        case 'n':
        case 'f':
        case 'e':
        case 's':
        case 'g':
        default:
            format.base = Quantity::Format::Base::Decimal;
            break;
        }
    }

    if (format.mode == Quantity::Format::Mode::Null) {
        if (format.base == Quantity::Format::Base::Decimal) {
          switch (settings->resultFormat) {
          case 'n':
              format.mode = Quantity::Format::Mode::Engineering;
              break;
          case 'f':
              format.mode = Quantity::Format::Mode::Fixed;
              break;
          case 'e':
              format.mode = Quantity::Format::Mode::Scientific;
              break;
          case 's':
              format.mode = Quantity::Format::Mode::Sexagesimal;
              break;
          case 'g':
          default:
              format.mode = Quantity::Format::Mode::General;
              break;
          }
        } else {
            format.mode = Quantity::Format::Mode::Fixed;
        }
    }

    if (format.precision == Quantity::Format::PrecisionNull)
        format.precision = settings->resultPrecision;
    if (format.notation == Quantity::Format::Notation::Null) {
        if (settings->resultFormatComplex == 'c')
            format.notation = Quantity::Format::Notation::Cartesian;
        else if (settings->resultFormatComplex == 'p')
            format.notation = Quantity::Format::Notation::Polar;
    }

    bool time = false, arc = q.isDimensionless();
    if (settings->resultFormat == 's' && q.hasDimension()) {
        auto dimension = q.getDimension();
        if (dimension.count() == 1 && dimension.firstKey() == "time") {
            auto iterator = dimension.begin();
            if ( iterator->numerator() == 1 && iterator->denominator() == 1) {
                q.clearDimension(); // remove unit, formatting itself is unit
                time = true;
            }
        }
    }

    if (arc && settings->resultFormat == 's') {     // convert to arcseconds
        if (settings->angleUnit == 'r')
            q /= Quantity(HMath::pi() / HNumber(180));
        else if (settings->angleUnit == 'g')
            q /= Quantity(HNumber(200) / HNumber(180));
        q *= Quantity(3600);
    }

    bool negative = false;
    if (settings->resultFormat == 's' && q.isNegative()) {
        q *= Quantity(HNumber(-1));
        negative = true;
    }

    QString result = DMath::format(q, format);

    if (settings->resultFormat == 's' && (arc || time)) {   // sexagesimal
        int dotPos = result.indexOf('.');
        HNumber seconds(dotPos > 0 ? result.left(dotPos).toStdString().c_str() : result.toStdString().c_str());
        HNumber mains = HMath::floor(seconds / HNumber(3600));
        seconds -= (mains * HNumber(3600));
        HNumber minutes = HMath::floor(seconds / HNumber(60));
        seconds -= (minutes * HNumber(60));
        HNumber::Format fixed = HNumber::Format::Fixed();
        QString sexa = HMath::format(mains, fixed);
        sexa.append(time ? ':' : 0xB0).append(minutes < 10 ? "0" : "").append(HMath::format(minutes, fixed));
        sexa.append(time ? ':' : '\'').append(seconds < 10 ? "0" : "").append(HMath::format(seconds, fixed));
        if (dotPos > 0)     // append decimals
            sexa.append(result.mid(dotPos));
        result = sexa;
    }

    if (negative)
        result.insert(0, '-');

    if (settings->radixCharacter() == ',')
        result.replace('.', ',');

    result.replace('-', g_minusChar);

    // Replace all spaces between units with dot operator.
    int emptySpaces = 0;
    for (auto& ch : result) {
        if (ch.isSpace()) {
            ++emptySpaces;
            if (emptySpaces > 1)
                ch = g_dotChar;
        }
    }

    return result;
}
