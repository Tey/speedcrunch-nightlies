// This file is part of the SpeedCrunch project
// Copyright (C) 2009, 2011, 2013, 2014 @heldercorreia
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

#ifndef GUI_RESULTDISPLAY_H
#define GUI_RESULTDISPLAY_H

#include <QBasicTimer>
#include <QPlainTextEdit>

class Quantity;
class SyntaxHighlighter;
class HistoryEntry;
class Session;

class ResultDisplay : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit ResultDisplay(QWidget* parent = 0);

    void append(const QString& expr, Quantity& value);
    void appendHistory(const QStringList& expressions, const QStringList& results);
    int count() const;
    bool isEmpty() const { return m_count==0; }
    QString exportHtml() const;

signals:
    void shiftWheelDown();
    void shiftWheelUp();
    void shiftControlWheelDown();
    void shiftControlWheelUp();
    void controlWheelDown();
    void controlWheelUp();
    void expressionSelected(const QString&);

public slots:
    void clear();
    void decreaseFontPointSize();
    void increaseFontPointSize();
    void rehighlight();
    void refresh();
    void scrollLines(int);
    void scrollLineUp();
    void scrollLineDown();
    void scrollPageUp();
    void scrollPageDown();
    void scrollToBottom();
    void scrollToTop();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent*);
    virtual void wheelEvent(QWheelEvent*);
    virtual void timerEvent(QTimerEvent*);
    void fullContentScrollEvent();
    float linesPerPage() const { return static_cast<float>(viewport()->height()) / fontMetrics().height(); }
    void pageScrollEvent();
    void scrollToDirection(int);
    void stopActiveScrollingAnimation();
    void updateScrollBarStyleSheet();

private:
    Q_DISABLE_COPY(ResultDisplay)

    SyntaxHighlighter* m_highlighter;
    QBasicTimer m_scrollTimer;
    int m_scrolledLines;
    int m_scrollDirection;
    bool m_isScrollingPageOnly;
    int m_count;
};

#endif
