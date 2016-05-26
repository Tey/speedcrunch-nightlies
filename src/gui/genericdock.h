// This file is part of the SpeedCrunch project
// Copyright (C) 2016 @fk
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

#ifndef GUI_GENERICDOCK_H
#define GUI_GENERICDOCK_H

#include <QCoreApplication>
#include <QDockWidget>


template <class Widget> class GenericDock : public QDockWidget {
    // Can't have Q_OBJECT in templates. That means no signals, slots, or properties in this class.

public:
    explicit GenericDock(const char* i18nContext, const char* name, QWidget* parent = nullptr)
        : QDockWidget(parent),
          m_name(name),
          m_i18nContext(i18nContext),
          m_widget(new Widget(this))
    {
        setWidget(m_widget);
        setMinimumWidth(200);
        retranslateText();
    }

    void retranslateText()
    {
        setWindowTitle(QCoreApplication::translate(m_i18nContext, m_name));
    }

    Widget* widget() const
    {
        return m_widget;
    }

    void setWidget(Widget* widget)
    {
        QDockWidget::setWidget(widget);
        m_widget = widget;
        setFocusProxy(widget);
    }

protected:
    void changeEvent(QEvent* e) override
    {
        if (e->type() == QEvent::LanguageChange)
            retranslateText();
        else
            QDockWidget::changeEvent(e);
    }

private:
    const char* m_name;
    const char* m_i18nContext;
    Widget* m_widget;
};

#endif
