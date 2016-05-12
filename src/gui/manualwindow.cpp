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

#include "gui/manualwindow.h"
#include "core/settings.h"
#include "core/manualserver.h"

#include <QtCore/QEvent>
#include <QKeyEvent>
#include <QtHelp/QHelpEngineCore>
#include <QDesktopServices>
#include <QIcon>


ManualWindow::ManualWindow(QWidget* parent)
    : QTextBrowser(parent), m_scrollUpdated(false)
{
    setWindowFlags(Qt::Window);
    setWindowIcon(QPixmap(":/speedcrunch.png"));
    setStyleSheet(QStringLiteral("QTextBrowser { background-color: #FFFFFF; }"));
    QFont f("Helvetica");
    f.setStyleHint(QFont::SansSerif);
    f.setPointSize(10);
    setFont(f);

    //Disable automatic opening of links. We handle them ourselves.
    this->setOpenLinks(false);
    connect(this, SIGNAL(anchorClicked(const QUrl&)), SLOT(handleAnchorClick(const QUrl&)));
    connect(this, SIGNAL(sourceChanged(const QUrl&)), SLOT(handleSourceChanged(const QUrl&)));

    m_server = ManualServer::instance();
    showHelpForKeyword("home");
    retranslateText();
}

void ManualWindow::showHelpForKeyword(const QString &id)
{
    QUrl url;
    if(m_server->URLforKeyword(id, url))
        openPage(url);
}

void ManualWindow::openPage(const QUrl& url)
{
    setSource(url);
}

void ManualWindow::retranslateText()
{
    QString docTitle = documentTitle();
    if (docTitle.isEmpty())
        setWindowTitle(tr("SpeedCrunch Manual"));
    else
        setWindowTitle(tr("%1 - SpeedCrunch Manual").arg(docTitle));
}


void ManualWindow::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        m_server->ensureCorrectLanguage();
        this->reload();
        retranslateText();
    }
    else
        QTextBrowser::changeEvent(event);
}

void ManualWindow::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Escape) {
        ev->accept();
        this->close();
    }
    else if(ev->key() == Qt::Key_Back
            || ev->key() == Qt::Key_Backspace) {
        ev->accept();
        this->backward();
    }
    else if(ev->key() == Qt::Key_Forward) {
        ev->accept();
        this->forward();
    }
    else if(ev->key() == Qt::Key_Home) {
        ev->accept();
        this->home();
    }
    else
        QTextBrowser::keyPressEvent(ev);
}

void ManualWindow::mouseReleaseEvent(QMouseEvent* ev)
{
    if (ev->button() == Qt::BackButton) {
        ev->accept();
        backward();
    }
    else if (ev->button() == Qt::ForwardButton) {
        ev->accept();
        forward();
    }
    else
        QTextBrowser::mouseReleaseEvent(ev);
}

void ManualWindow::closeEvent(QCloseEvent* event)
{
    emit windowClosed();
    QTextBrowser::closeEvent(event);
}

void ManualWindow::paintEvent(QPaintEvent* e)
{
    /* So the issue and reasoning here is:
     * - When opening the manual window via the context help feature *and it was maximized before*,
     *   it doesn't correctly scroll to the right anchor on the page.
     * - QTextBrowser keeps its scrolling offset in pixels; when the widget is resized, the text on
     *   the page is laid out again and the pixel offset of anchors may change. This means when
     *   resizing the widget, the formerly-scrolled-to anchor may not be at the top anymore.
     * - For maximized windows, the page is loaded first, -- including scrolling the widget to the
     *   anchor -- then there's a resizeEvent that sets the actual, maximized window size. This throws
     *   off the page position.
     * - We work around this here by updating the scroll position on the very first paintEvent, which
     *   happens after the window's maximized state has finally properly registered; late enough to fix
     *   the scrolling.
     *
     * Yes, it's rather hackish.
     */
    if (!m_scrollUpdated && source().isValid()) {
        scrollToAnchor(source().fragment());
        m_scrollUpdated = true;
    }
    QTextBrowser::paintEvent(e);
}

void ManualWindow::handleAnchorClick(const QUrl &url)
{
    if (url.scheme() == "qthelp") 
        openPage(url);
    else
        QDesktopServices::openUrl(url);
}

void ManualWindow::handleSourceChanged(const QUrl&)
{
    // This updates the window title with the new document title.
    retranslateText();
}

QVariant ManualWindow::loadResource(int type, const QUrl &name)
{
    QByteArray ba;
    if (type < 4 && name.scheme()=="qthelp") {
        QUrl url(name);
        if (name.isRelative())
            url = source().resolved(name);
        ba = m_server->fileData(url);
    }
    return ba;
}
