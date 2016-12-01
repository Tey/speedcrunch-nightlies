// This file is part of the SpeedCrunch project
// Copyright (C) 2007 Ariya Hidayat <ariya@kde.org>
// Copyright (C) 2007-2016 @heldercorreia
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

#ifndef GUI_SYNTAXHIGHLIGHTER_H
#define GUI_SYNTAXHIGHLIGHTER_H

#include <QtCore/QJsonDocument>
#include <QSyntaxHighlighter>

class QPlainTextEdit;

class ColorScheme {
public:
    enum Role {
        Cursor,
        Number,
        Parens,
        Result,
        Comment,
        Matched,
        Function,
        Operator,
        Variable,
        ScrollBar,
        Separator,
        Background,
        EditorBackground
    };

    ColorScheme() : m_valid(false) { }
    ColorScheme(const QJsonDocument& doc);
    bool isValid() const { return m_valid; }
    QColor colorForRole(Role role) const;

    static QStringList enumerate();
    static ColorScheme loadFromFile(const QString& path);
    static ColorScheme loadByName(const QString& name);

private:
    bool m_valid;
    QHash<Role, QColor> m_colors;
    static const constexpr char* m_colorSchemeExtension = "json";
};

class SyntaxHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    explicit SyntaxHighlighter(QPlainTextEdit*);

    void setColorScheme(ColorScheme&&);
    QColor colorForRole(ColorScheme::Role role) const { return m_colorScheme.colorForRole(role); }

    void update();
    virtual void highlightBlock(const QString&);
    void asHtml(QString& html);

private:
    Q_DISABLE_COPY(SyntaxHighlighter)
    SyntaxHighlighter();
    SyntaxHighlighter(QObject*);
    SyntaxHighlighter(QTextDocument*);
    void groupDigits(const QString& text, int pos, int length);
    void formatDigitsGroup(const QString& text, int start, int end, bool invert, int size);

    ColorScheme m_colorScheme;
};

#endif
