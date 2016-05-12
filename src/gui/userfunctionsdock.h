// This file is part of the SpeedCrunch project
// Copyright (C) 2007 Ariya Hidayat <ariya@kde.org>
// Copyright (C) 2008, 2009, 2011, 2013 @heldercorreia
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

#ifndef GUI_USERFUNCTIONSDOCK_H
#define GUI_USERFUNCTIONSDOCK_H

#include <QDockWidget>

class QTreeWidgetItem;
class UserFunctionListWidget;

class UserFunctionsDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit UserFunctionsDock(QWidget* parent = 0);

signals:
    void userFunctionSelected(const QString&);
    void userFunctionEdited(const QString&);

public slots:
    void handleRadixCharacterChange();
    void retranslateText();
    void updateList();

protected:
    virtual void changeEvent(QEvent*);

private:
    Q_DISABLE_COPY(UserFunctionsDock)

    UserFunctionListWidget* m_userFunctionsWidget;
};

#endif

