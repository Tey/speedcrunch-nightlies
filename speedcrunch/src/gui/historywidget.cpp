// This file is part of the SpeedCrunch project
// Copyright (C) 2007 Ariya Hidayat <ariya@kde.org>
// Copyright (C) 2008, 2009, 2011 @heldercorreia
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

#include "gui/historywidget.h"
#include "core/sessionhistory.h"
#include "core/evaluator.h"
#include "core/session.h"

#include <QEvent>
#include <QListWidget>
#include <QVBoxLayout>


HistoryWidget::HistoryWidget(QWidget *parent)
    : QWidget(parent)
    , m_list(new QListWidget(this))
{
    m_list->setAlternatingRowColors(true);
    m_list->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_list->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_list->setCursor(QCursor(Qt::PointingHandCursor));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(3);
    layout->addWidget(m_list);
    setLayout(layout);

    connect(m_list, SIGNAL(itemActivated(QListWidgetItem *)), SLOT(handleItem(QListWidgetItem *)));

    updateHistory();
}

void HistoryWidget::updateHistory()
{
    QList<HistoryEntry> hist = Evaluator::instance()->session()->historyToList();
    m_list->clear();
    m_list->clearSelection();
    QStringList l;

    for(int i=0; i<hist.size(); ++i)
        l.append(hist[i].expr());

    m_list->addItems(l);
    m_list->scrollToBottom();
}

void HistoryWidget::handleItem(QListWidgetItem *item)
{
    m_list->clearSelection();
    emit expressionSelected(item->text());
}

void HistoryWidget::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
        setLayoutDirection(Qt::LeftToRight);
    else
        QWidget::changeEvent(e);
}

