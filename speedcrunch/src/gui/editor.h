// This file is part of the SpeedCrunch project
// Copyright (C) 2004, 2005, 2007 Ariya Hidayat <ariya@kde.org>
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

#ifndef GUI_EDITOR_H
#define GUI_EDITOR_H

#include "core/sessionhistory.h"

#include <QPlainTextEdit>

struct Constant;
class ConstantCompletion;
class EditorCompletion;
class Evaluator;
class Session;
class CNumber;
class SyntaxHighlighter;

class QEvent;
class QKeyEvent;
class QMimeData;
class QTimeLine;
class QTreeWidget;
class QWheelEvent;
class QWidget;

class Editor : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit Editor(QWidget* parent = nullptr);

    bool isAutoCalcEnabled() const;
    bool isAutoCompletionEnabled() const;
    void clearHistory();
    int cursorPosition() const;
    void doBackspace();
    char radixChar() const;
    void setAnsAvailable(bool);
    void setAutoCalcEnabled(bool);
    void setAutoCompletionEnabled(bool);
    void setCursorPosition(int pos);
    void setText(const QString&);
    void stopAutoCalc();
    void stopAutoComplete();
    void wrapSelection();
    QString text() const;
    QStringList matchFragment(const QString&) const;
    QString getKeyword() const;

signals:
    void autoCalcMessageAvailable(const QString&);
    void autoCalcQuantityAvailable(const Quantity&);
    void autoCalcDisabled();
    void controlPageDownPressed();
    void controlPageUpPressed();
    void copySequencePressed();
    void pageDownPressed();
    void pageUpPressed();
    void returnPressed();
    void shiftDownPressed();
    void shiftUpPressed();
    void shiftPageDownPressed();
    void shiftPageUpPressed();

public slots:
    void autoCalcSelection(const QString& custom = QString::null);
    void cancelConstantCompletion();
    void evaluate();
    void decreaseFontPointSize();
    void increaseFontPointSize();
    void insert(const QString&);
    void insertConstant(const QString&);
    void rehighlight();
    void updateHistory();

protected slots:
    void insertFromMimeData(const QMimeData*) override;
    void autoCalc();
    void autoComplete(const QString&);
    void checkAutoCalc();
    void checkAutoComplete();
    void checkMatching();
    void checkSelectionAutoCalc();
    void doMatchingLeft();
    void doMatchingPar();
    void doMatchingRight();
    void historyBack();
    void historyForward();
    void triggerAutoComplete();
    void triggerEnter();

protected:
    void changeEvent(QEvent*) override;
    void focusOutEvent(QFocusEvent*) override;
    void keyPressEvent(QKeyEvent*) override;
    void paintEvent(QPaintEvent*) override;
    void scrollContentsBy(int, int) override;
    QSize sizeHint() const override;
    void wheelEvent(QWheelEvent*) override;

private:
    Q_DISABLE_COPY(Editor)

    bool m_isAnsAvailable;
    bool m_isAutoCalcEnabled;
    bool m_shouldBlockAutoCompletionOnce = false;
    bool m_isAutoCompletionEnabled;
    EditorCompletion* m_completion;
    QTimer* m_completionTimer;
    ConstantCompletion* m_constantCompletion;
    Evaluator* m_evaluator;
    SyntaxHighlighter* m_highlighter;
    QList<HistoryEntry> m_history;
    QString m_savedCurrentEditor;
    int m_currentHistoryIndex;
    QTimer* m_matchingTimer;
    bool m_shouldPaintCustomCursor;
    const Session * m_session;
};

class EditorCompletion : public QObject {
    Q_OBJECT

public:
    EditorCompletion(Editor*);
    ~EditorCompletion();

    bool eventFilter(QObject*, QEvent*);
    void showCompletion(const QStringList&);

signals:
    void selectedCompletion(const QString&);

public slots:
    void doneCompletion();
    void selectItem(const QString&);

private:
    Q_DISABLE_COPY(EditorCompletion)

    Editor* m_editor;
    QTreeWidget* m_popup;
};

class ConstantCompletion : public QObject {
    Q_OBJECT

public:
    ConstantCompletion(Editor*);
    ~ConstantCompletion();

    bool eventFilter(QObject*, QEvent*);
    void showCompletion();

signals:
    void canceledCompletion();
    void selectedCompletion(const QString&);

public slots:
    void doneCompletion();

protected slots:
    void setHorizontalPosition(int);
    void showCategory();
    void showConstants();

private:
    Q_DISABLE_COPY(ConstantCompletion)

    QTreeWidget* m_categoryWidget;
    QList<Constant> m_constantList;
    Editor* m_editor;
    QString m_lastCategory;
    QTreeWidget* m_constantWidget;
    QFrame* m_popup;
    QTimeLine* m_slider;
};

#endif
