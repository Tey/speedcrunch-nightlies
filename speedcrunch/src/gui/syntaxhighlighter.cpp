// This file is part of the SpeedCrunch project
// Copyright (C) 2007 Ariya Hidayat <ariya@kde.org>
// Copyright (C) 2007-2016 @heldercorreia
// Copyright (c) 2013 Larswad
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
//

#include "gui/syntaxhighlighter.h"

#include "core/evaluator.h"
#include "core/functions.h"
#include "core/settings.h"

#include <QtCore/QDir>
#include <QtCore/QJsonObject>
#include <QLatin1String>
#include <QApplication>
#include <QPalette>
#include <QPlainTextEdit>
#include <QTextDocument>
#include <QTextDocumentFragment>

static const QVector<QString> colorSchemeSearchPaths()
{
    static QVector<QString> searchPaths;
    if (searchPaths.isEmpty()) {
        // By only populating the paths in a function when they're used, we ensure all the QApplication
        // fields that are used by QStandardPaths are set.
        searchPaths.append(QString("%1/color-schemes").arg(Settings::getDataPath()));
        searchPaths.append(QStringLiteral(":/color-schemes"));
    }
    return searchPaths;
}

QColor getFallbackColor(ColorScheme::Role role)
{
    switch (role) {
    case ColorScheme::Background:
    case ColorScheme::EditorBackground:
        return QApplication::palette().color(QPalette::Base);
    default:
        return QApplication::palette().color(QPalette::Text);
    }
}

ColorScheme::ColorScheme(const QJsonDocument& doc)
    : m_valid(false)
{
    static const QVector<std::pair<QString, ColorScheme::Role>> RoleNames {
        { QStringLiteral("cursor"), ColorScheme::Cursor },
        { QStringLiteral("number"), ColorScheme::Number },
        { QStringLiteral("parens"), ColorScheme::Parens },
        { QStringLiteral("result"), ColorScheme::Result },
        { QStringLiteral("comment"), ColorScheme::Comment },
        { QStringLiteral("matched"), ColorScheme::Matched },
        { QStringLiteral("function"), ColorScheme::Function },
        { QStringLiteral("operator"), ColorScheme::Operator },
        { QStringLiteral("variable"), ColorScheme::Variable },
        { QStringLiteral("scrollbar"), ColorScheme::ScrollBar },
        { QStringLiteral("separator"), ColorScheme::Separator },
        { QStringLiteral("background"), ColorScheme::Background },
        { QStringLiteral("editorbackground"), ColorScheme::EditorBackground },
    };
    if (!doc.isObject())
        return;
    auto obj = doc.object();
    for (auto& role : RoleNames) {
        auto v = obj.value(role.first);
        if (v.isUndefined())
            // Having a key missing is fine...
            continue;
        auto color = QColor(v.toString());
        if (!color.isValid())
            // ...having one that's not a color is not.
            return;
        m_colors.insert(role.second, color);
    }
    m_valid = true;
}

QColor ColorScheme::colorForRole(Role role) const
{
    QColor color = m_colors[role];
    if (!color.isValid())
        return getFallbackColor(role);
    else
        return color;
}

QStringList ColorScheme::enumerate()
{
    QMap<QString, void*> colorSchemes;
    for (auto& searchPath : colorSchemeSearchPaths()) {
        QDir dir(searchPath);
        dir.setFilter(QDir::Files | QDir::Readable);
        dir.setNameFilters({ QString("*.%1").arg(m_colorSchemeExtension) });
        const auto infoList = dir.entryInfoList();
        for (auto& info : infoList) // TODO: Use Qt 5.7's qAsConst().
            colorSchemes.insert(info.completeBaseName(), nullptr);
    }
    // Since this is a QMap, the keys are already sorted in ascending order.
    return colorSchemes.keys();
}

ColorScheme ColorScheme::loadFromFile(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return ColorScheme();
    // TODO: Better error handling.
    return ColorScheme(QJsonDocument::fromJson(file.readAll()));
}

ColorScheme ColorScheme::loadByName(const QString& name)
{
    for (auto& path : colorSchemeSearchPaths()) {
        auto fileName = QString("%1/%2.%3").arg(path, name, m_colorSchemeExtension);
        auto colorScheme = loadFromFile(fileName);
        if (colorScheme.isValid())
            return colorScheme;
    }
    return ColorScheme();
}


SyntaxHighlighter::SyntaxHighlighter(QPlainTextEdit* edit)
    : QSyntaxHighlighter(edit)
{
    setDocument(edit->document());
    update();
}

void SyntaxHighlighter::setColorScheme(ColorScheme&& colorScheme) {
    m_colorScheme = colorScheme;
}

void SyntaxHighlighter::highlightBlock(const QString& text)
{
    // Default color for the text
    setFormat(0, text.length(), colorForRole(ColorScheme::Number));

    if (!Settings::instance()->syntaxHighlighting)
        return;

    if (text.startsWith(QLatin1String("="))) {
        setFormat(0, 1, colorForRole(ColorScheme::Operator));
        setFormat(1, text.length(), colorForRole(ColorScheme::Result));
        if (Settings::instance()->digitGrouping > 0)
            groupDigits(text, 1, text.length() - 1);
        return;
    }

    int questionMarkIndex = text.indexOf('?');
    if (questionMarkIndex != -1)
        setFormat(questionMarkIndex, text.length(), colorForRole(ColorScheme::Comment));

    Tokens tokens = Evaluator::instance()->scan(text);

    for (int i = 0; i < tokens.count(); ++i) {
        const Token& token = tokens.at(i);
        const QString tokenText = token.text().toLower();
        QStringList functionNames = FunctionRepo::instance()->getIdentifiers();
        QColor color;

        switch (token.type()) {
        case Token::stxNumber:
        case Token::stxUnknown:
            color = colorForRole(ColorScheme::Number);
            // TODO: color thousand separators differently? It might help troubleshooting issues
            break;

        case Token::stxOperator:
            color = colorForRole(ColorScheme::Operator);
            break;

        case Token::stxSep:
            color = colorForRole(ColorScheme::Separator);
            break;

        case Token::stxOpenPar:
        case Token::stxClosePar:
            color = colorForRole(ColorScheme::Parens);
            break;

        case Token::stxIdentifier:
            color = colorForRole(ColorScheme::Variable);
            if (Evaluator::instance()->hasUserFunction(token.text())
                || functionNames.contains(tokenText, Qt::CaseInsensitive))
                color = colorForRole(ColorScheme::Function);
            break;

        default:
            break;
        };

        setFormat(token.pos(), token.size(), color);
        if (token.type() == Token::stxNumber && Settings::instance()->digitGrouping > 0)
            groupDigits(text, token.pos(), token.size());
    }
}

void SyntaxHighlighter::update()
{
    QString name = Settings::instance()->colorScheme;
    setColorScheme(ColorScheme::loadByName(name));

    QColor backgroundColor = colorForRole(ColorScheme::Background);
    QWidget* parentWidget = static_cast<QWidget*>(parent());
    QPalette pal = parentWidget->palette();
    pal.setColor(QPalette::Active, QPalette::Base, backgroundColor);
    pal.setColor(QPalette::Inactive, QPalette::Base, backgroundColor);
    parentWidget->setPalette(pal);

    rehighlight();
}

void SyntaxHighlighter::formatDigitsGroup(const QString& text, int start, int end, bool invert, int size)
{
    Q_ASSERT(start <= end);
    Q_ASSERT(size > 0);

    qreal spacing = 100; // Size of the space between groups (100 means no space).
    spacing += 40 * Settings::instance()->digitGrouping;
    int inc = !invert ? -1 : 1;
    if(!invert)
    {
        int tmp = start;
        start = end - 1;
        end = tmp - 1;

        // Skip the first digit so that we add the spacing to the first digit of the next group.
        while (start != end && Evaluator::isSeparatorChar(text[start].unicode()))
            --start;
        if (start == end)
            return; // Bug ?
        --start;
    }

    for (int count = 0 ; start != end ; start += inc)
    {
        // When there are separators in the number, we must not count them as part of the group.
        if (!Evaluator::isSeparatorChar(text[start].unicode()))
        {
            ++count;
            if (count == size)
            {
                // Only change the letter spacing from the format and keep the other properties.
                QTextCharFormat fmt = format(start);
                fmt.setFontLetterSpacing(spacing);
                setFormat(start, 1, fmt);
                count = 0; // Reset
                // TODO: if the next character is a separator, do not add spacing?
            }
        }
    }
}

void SyntaxHighlighter::groupDigits(const QString& text, int pos, int length)
{
    // Used to find out which characters belong to which radixes.
    static int charType[128] = { 0 };
    static const int BIN_CHAR = (1 << 0);
    static const int OCT_CHAR = (1 << 1);
    static const int DEC_CHAR = (1 << 2);
    static const int HEX_CHAR = (1 << 3);

    if (charType[int('0')] == 0) { // Initialize the table on first call (not thread-safe!).
        charType[int('0')] = HEX_CHAR | DEC_CHAR | OCT_CHAR | BIN_CHAR;
        charType[int('1')] = HEX_CHAR | DEC_CHAR | OCT_CHAR | BIN_CHAR;
        charType[int('2')] = HEX_CHAR | DEC_CHAR | OCT_CHAR;
        charType[int('3')] = HEX_CHAR | DEC_CHAR | OCT_CHAR;
        charType[int('4')] = HEX_CHAR | DEC_CHAR | OCT_CHAR;
        charType[int('5')] = HEX_CHAR | DEC_CHAR | OCT_CHAR;
        charType[int('6')] = HEX_CHAR | DEC_CHAR | OCT_CHAR;
        charType[int('7')] = HEX_CHAR | DEC_CHAR | OCT_CHAR;
        charType[int('8')] = HEX_CHAR | DEC_CHAR;
        charType[int('9')] = HEX_CHAR | DEC_CHAR;
        charType[int('a')] = HEX_CHAR;
        charType[int('b')] = HEX_CHAR;
        charType[int('c')] = HEX_CHAR;
        charType[int('d')] = HEX_CHAR;
        charType[int('e')] = HEX_CHAR;
        charType[int('f')] = HEX_CHAR;
        charType[int('A')] = HEX_CHAR;
        charType[int('B')] = HEX_CHAR;
        charType[int('C')] = HEX_CHAR;
        charType[int('D')] = HEX_CHAR;
        charType[int('E')] = HEX_CHAR;
        charType[int('F')] = HEX_CHAR;
    }

    int s = -1; // Index of the first digit (most significant).
    bool invertGroup = false; // If true, group digits from the most significant digit.
    int groupSize = 3; // Number of digits to group (depends on the radix).
    int allowedChars = DEC_CHAR; // Allowed characters for the radix of the current number being parsed.

    int endPos = pos + length;
    if (endPos > text.length())
        endPos = text.length();
    for (int i = pos; i < endPos; ++i) {
        ushort c = text[i].unicode();
        bool isDigit = c < 128 && (charType[c] & allowedChars);

        if (s >= 0) {
            if (!isDigit) {
                bool endOfNumber = true;
                // If this is a separator and next character is a digit or a separator,
                // the next character is part of the same number expression
                if (Evaluator::isSeparatorChar(c) && i<endPos-1) {
                    ushort nextC = text[i+1].unicode();
                    if ((nextC < 128 && (charType[nextC] & allowedChars))
                         || Evaluator::isSeparatorChar(nextC))
                        endOfNumber = false;
                }

                if (endOfNumber) {
                    // End of current number found, start grouping the digits.
                    formatDigitsGroup(text, s, i, invertGroup, groupSize);
                    s = -1; // Reset.
                }
            }
        } else {
            if (isDigit) // Start of number found.
                s = i;
        }

        if (!isDigit) {
            if (Evaluator::isRadixChar(c)) {
                // Invert the grouping for the fractional part.
                invertGroup = true;
            } else if (!Evaluator::isSeparatorChar(c)){
                // Look for a radix prefix.
                invertGroup = false;
                if (i > 0 && text[i - 1] == '0') {
                    if (c == 'x') {
                        groupSize = 4;
                        allowedChars = HEX_CHAR;
                    } else if (c == 'o') {
                        groupSize = 3;
                        allowedChars = OCT_CHAR;
                    } else if (c == 'b') {
                        groupSize = 4;
                        allowedChars = BIN_CHAR;
                    } else {
                        groupSize = 3;
                        allowedChars = DEC_CHAR;
                    }
                } else {
                    groupSize = 3;
                    allowedChars = DEC_CHAR;
                }
            }
        }
    }

    // Group the last digits if the string finishes with the number.
    if (s >= 0) {
        formatDigitsGroup(text, s, endPos, invertGroup, groupSize);
    }
}


// Original code snippet from StackOverflow.
// http://stackoverflow.com/questions/15280452/how-can-i-get-highlighted-text-from-a-qsyntaxhighlighter-into-an-html-string
void SyntaxHighlighter::asHtml(QString& html)
{
    // Create a new document from all the selected text document.
    QTextCursor cursor(document());
    cursor.select(QTextCursor::Document);
    QTextDocument* tempDocument(new QTextDocument);
    Q_ASSERT(tempDocument);
    QTextCursor tempCursor(tempDocument);

    tempCursor.insertFragment(cursor.selection());
    tempCursor.select(QTextCursor::Document);
    // Set the default foreground for the inserted characters.
    QTextCharFormat textfmt = tempCursor.charFormat();
    textfmt.setForeground(Qt::gray);
    tempCursor.setCharFormat(textfmt);

    // Apply the additional formats set by the syntax highlighter
    QTextBlock start = document()->findBlock(cursor.selectionStart());
    QTextBlock end = document()->findBlock(cursor.selectionEnd());
    end = end.next();
    const int selectionStart = cursor.selectionStart();
    const int endOfDocument = tempDocument->characterCount() - 1;
    for(QTextBlock current = start; current.isValid() && current != end; current = current.next()) {
        const QTextLayout* layout(current.layout());

        foreach(const QTextLayout::FormatRange &range, layout->additionalFormats()) {
            const int start = current.position() + range.start - selectionStart;
            const int end = start + range.length;
            if(end <= 0 || start >= endOfDocument)
                continue;
            tempCursor.setPosition(qMax(start, 0));
            tempCursor.setPosition(qMin(end, endOfDocument), QTextCursor::KeepAnchor);
            tempCursor.setCharFormat(range.format);
        }
    }

    // Reset the user states since they are not interesting
    for(QTextBlock block = tempDocument->begin(); block.isValid(); block = block.next())
        block.setUserState(-1);

    // Make sure the text appears pre-formatted, and set the background we want.
    tempCursor.select(QTextCursor::Document);
    QTextBlockFormat blockFormat = tempCursor.blockFormat();
    blockFormat.setNonBreakableLines(true);
    blockFormat.setBackground(colorForRole(ColorScheme::Background));
    tempCursor.setBlockFormat(blockFormat);

    // Finally retreive the syntax higlighted and formatted html.
    html = tempCursor.selection().toHtml("UTF-8");
    delete tempDocument;

    // Inject CSS, so to avoid a white margin
    html.replace("<head>", QString("<head> <style> body {background-color: %1;}</style>")
                                    .arg(colorForRole(ColorScheme::Background).name()));
}
