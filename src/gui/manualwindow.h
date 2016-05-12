// This file is part of the SpeedCrunch project
// Copyright (C) 2014 @heldercorreia
// Copyright (c) 2016 Pol Welter <polwelter@gmail.com
// Copyright (c) 2016 Felix Krull <f_krull@gmx.de>
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

#ifndef GUI_MANUALWINDOW_H
#define GUI_MANUALWINDOW_H

#include <QTextBrowser>

class QCloseEvent;
class QEvent;
class QUrl;
class ManualServer;

class ManualWindow : public QTextBrowser {
    Q_OBJECT

public:
    ManualWindow(QWidget* parent = 0);
    void showHelpForKeyword(const QString &id);

signals:
    void windowClosed();

public slots:
    void openPage(const QUrl&);
    void retranslateText();

protected:
    virtual void changeEvent(QEvent*);
    virtual void keyPressEvent(QKeyEvent * ev);
    virtual void mouseReleaseEvent(QMouseEvent* ev);
	virtual void closeEvent(QCloseEvent*);
    virtual void paintEvent(QPaintEvent* e);
private slots:
    void handleAnchorClick(const QUrl&url);
    void handleSourceChanged(const QUrl& url);

private:
    Q_DISABLE_COPY(ManualWindow)

    QVariant loadResource(int type, const QUrl &name);
    ManualServer *m_server;
    bool m_scrollUpdated;
};

#endif // GUI_MANUALWINDOW_H
