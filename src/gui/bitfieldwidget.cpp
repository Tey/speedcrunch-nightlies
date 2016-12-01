// This file is part of the SpeedCrunch project
// Copyright (C) 2014 Sébastien Szymanski <seb.szymanski@gmail.com>
// Copyright (C) 2014-2016 @heldercorreia
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

#include "bitfieldwidget.h"

#include "math/quantity.h"

#include <cmath>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListIterator>
#include <QPaintEvent>
#include <QApplication>
#include <QPushButton>

BitWidget::BitWidget(int bitPosition, QWidget* parent)
    : QLabel(parent),
    m_state(false)
{
    HNumber number(HMath::raise(HNumber(2), bitPosition));
    setToolTip(QString("2<sup>%1</sup> = %2")
        .arg(bitPosition)
        .arg(HMath::format(number, Quantity::Format::Decimal())));

    setText(QString("%1").arg(bitPosition));
    setObjectName("BitWidget");
}

void BitWidget::setState(bool state)
{
    if (state != m_state) {
        m_state = state;
        setStyleSheet(
            QString("QLabel { background-color : %1; color : %2; }")
                .arg(QApplication::palette().color(state ? QPalette::WindowText : QPalette::Window).name(),
                     QApplication::palette().color(state ? QPalette::Window : QPalette::WindowText).name())
        );
        update();
    }
}

void BitWidget::mouseReleaseEvent(QMouseEvent*)
{
    setState(!m_state);
    emit stateChanged(m_state);
}

BitFieldWidget::BitFieldWidget(QWidget* parent) :
    QWidget(parent)
{
    // Build the CSS border color using 50% opacity (same result as previous method with painting)
    auto& borderColor = QApplication::palette().color(QPalette::WindowText);
    QString cssBorderColor = QString("rgba(%1, %2, %3, %4)")
                                .arg(borderColor.red())
                                .arg(borderColor.green())
                                .arg(borderColor.blue())
                                .arg(0.5);

    setStyleSheet(QString("QLabel#BitWidget, QLabel#FirstBitWidget {"
                          " qproperty-alignment: 'AlignHCenter | AlignVCenter';"
                          " border-top: 1px solid %3;"
                          " border-bottom: 1px solid %3;"
                          " border-right: 1px solid %3;"
                          " padding: 1px;"
                          " background-color : %1; color : %2;"
                          "}"
                          /* Extra style for first bit of each group */
                          "QLabel#FirstBitWidget {"
                          " border-left: 1px solid %3;"
                          "}"
                          )
                      .arg(QApplication::palette().color(QPalette::Window).name(),
                           QApplication::palette().color(QPalette::WindowText).name(),
                           cssBorderColor)
                 );

    m_bitWidgets.reserve(NumberOfBits);
    for (int i = 0; i < NumberOfBits; ++i) {
        BitWidget* bitWidget = new BitWidget(i);
        connect(bitWidget, SIGNAL(stateChanged(bool)), this, SLOT(onBitChanged()));
        m_bitWidgets.append(bitWidget);
    }

    m_byteLayouts.reserve(NumberOfBits / 8);
    for (int i = 0; i < NumberOfBits; i += 8) {
      QHBoxLayout* byteLayout(new QHBoxLayout);
      byteLayout->setSpacing(5);

      // Each byte is drawn over 3 UI elements (1 for each nibble and 1 for space)
      for (int nibble = 1 ; nibble >= 0 ; --nibble) {
          // Draw each nibble (4-bits) in a single box
          QHBoxLayout* nibbleLayout(new QHBoxLayout);

          // Disable items spacing so that it looks like a table
          nibbleLayout->setSpacing(0);

          for (int j = 3; j >= 0; --j) {
              const int bitIndex = i + (nibble * 4) + j;
              nibbleLayout->addWidget(m_bitWidgets.at(bitIndex));
          }

          // Change the name of first bits so that the proper style is applied
          nibbleLayout->itemAt(0)->widget()->setObjectName("FirstBitWidget");

          byteLayout->addLayout(nibbleLayout, Qt::AlignCenter);
        }

        // Draw the space between each byte
        QLabel* byteSpaceLabel = new QLabel;
        byteLayout->addWidget(byteSpaceLabel);

        m_byteLayouts.append(byteLayout);
    }

    m_resetButton = new QPushButton("0");
    m_resetButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(m_resetButton, SIGNAL(clicked()), this, SLOT(resetBits()));

    m_invertButton = new QPushButton("~");
    m_invertButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(m_invertButton, SIGNAL(clicked()), this, SLOT(invertBits()));

    m_shiftLeftButton = new QPushButton("<<");
    m_shiftLeftButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(m_shiftLeftButton, SIGNAL(clicked()), this, SLOT(shiftBitsLeft()));

    m_shiftRightButton = new QPushButton(">>");
    m_shiftRightButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(m_shiftRightButton, SIGNAL(clicked()), this, SLOT(shiftBitsRight()));

    m_buttonsLayout = new QGridLayout;
    m_buttonsLayout->addWidget(m_resetButton, 0, 0);
    m_buttonsLayout->addWidget(m_invertButton, 0, 1);
    m_buttonsLayout->addWidget(m_shiftLeftButton, 1, 0);
    m_buttonsLayout->addWidget(m_shiftRightButton, 1, 1);

    m_fieldLayout = new QGridLayout;

    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->addStretch();
    m_mainLayout->addLayout(m_fieldLayout);
    m_mainLayout->addLayout(m_buttonsLayout);
    m_mainLayout->addStretch();

    this->updateFieldLayout();

    // The following needs to be done AFTER the widgets have been added to BitFieldWidget
    // as the style sheet will not be applied otherwise
    this->updateSize();

    // Update the field layout again, because the widgets size has changed now
    this->updateFieldLayout();
}

/** Update the bitfield layout based on the size of BitFieldWidget and its children.
 * This method should be called during initialization (but only after all the BitFieldWidget
 * members have been set) and every time the size of BitFieldWidget is changed.
 */
void BitFieldWidget::updateFieldLayout()
{
    // Empty current field layout
    while (m_fieldLayout->count() > 0) {
        m_fieldLayout->removeItem(m_fieldLayout->itemAt(0));
    }

    // Compute how much horizontal space we have to draw the bitfield
    int widgetWidth = this->size().width();
    int buttonsWidth = m_buttonsLayout->sizeHint().width();
    int byteWidgetWidth = m_byteLayouts.at(0)->sizeHint().width();
    auto contentMargins = this->contentsMargins() + m_mainLayout->contentsMargins();
    int spacesWidth = contentMargins.left() + contentMargins.right() + m_mainLayout->spacing() * 3;
    int availableWidth = widgetWidth - (buttonsWidth + spacesWidth);

    // Find out how much bytes per row can be shown within availableWidth
    int bytesPerRow = 1;
    if (byteWidgetWidth > 0 && byteWidgetWidth <= availableWidth) {
        // Make it a power of 2 so that only 8/16/32/64 bits per row are possible
        bytesPerRow = pow(2, floor(log2(availableWidth / byteWidgetWidth)));
    }

    // Populate the field layout
    int maxRows = NumberOfBits / (8 * bytesPerRow);
    for (int col = 0 ; col < bytesPerRow ; ++col) {
        const int colOffset = (bytesPerRow - col - 1);

        for (int row = 0 ; row < maxRows ; ++row) {
            int byteIndex = (maxRows - row - 1) * bytesPerRow + colOffset;
            auto* byteLayout = m_byteLayouts.at(byteIndex);

            m_fieldLayout->addLayout(byteLayout, row, col);
        }
    }

    // Update the minimum widget size so that its width can be reduced by the user
    if (byteWidgetWidth + buttonsWidth > 0)
        this->setMinimumWidth(byteWidgetWidth + buttonsWidth + spacesWidth);
}

/** Update the size of each BitWidget so that it is the same, as well as the size of the buttons.
 * This method should be called during initialization, and every time the UI font is changed.
 */
void BitFieldWidget::updateSize()
{
    // Compute bit widgets max size and apply it to all bit widgets
    QSize maxSize(0, 0);

    for (auto& bitWidget : m_bitWidgets) {
        auto widgetSize = bitWidget->sizeHint();
        if (maxSize.width() < widgetSize.width())
            maxSize.setWidth(widgetSize.width());
        if (maxSize.height() < widgetSize.height())
            maxSize.setHeight(widgetSize.height());
    }

    // Make the box be a square
    if (maxSize.width() < maxSize.height())
        maxSize.setWidth(maxSize.height());
    else
        maxSize.setHeight(maxSize.width());

    // Apply maxSize to all bit widgets
    for (auto& widget : m_bitWidgets)
        widget->setFixedSize(maxSize);

    // TODO: find some more justifiable size calculation.
    int buttonHeight = maxSize.height() * 4 / 3;
    int buttonWidth = buttonHeight * 2;

    m_resetButton->setFixedSize(buttonWidth, buttonHeight);
    m_invertButton->setFixedSize(buttonWidth, buttonHeight);
    m_shiftLeftButton->setFixedSize(buttonWidth, buttonHeight);
    m_shiftRightButton->setFixedSize(buttonWidth, buttonHeight);
}

void BitFieldWidget::wheelEvent(QWheelEvent* we)
{
    if (we->angleDelta().y() > 0)
        shiftBitsLeft();
    else
        shiftBitsRight();
}

void BitFieldWidget::resizeEvent(QResizeEvent* re)
{
  this->updateFieldLayout();
}

void BitFieldWidget::updateBits(const Quantity& number)
{
    // Create a binary copy of number
    // (simply converting it to binary will not work if its base is defined)
    Quantity binNumber(number);
    binNumber.setFormat(Quantity::Format::Fixed() + Quantity::Format::Binary() + Quantity(number).format());
    QString binaryNumberString = DMath::format(binNumber);
    QListIterator<BitWidget*> bitsIterator(m_bitWidgets);

    if (number.isZero() || !number.isInteger())
        binaryNumberString.clear();
    else if (number.isNegative())
        binaryNumberString.remove(0, 3); // Remove '-0b'.
    else
        binaryNumberString.remove(0, 2); // Remove '0b'.

    auto iterator = binaryNumberString.end();
    while (bitsIterator.hasNext()) {
        if (iterator != binaryNumberString.begin()) {
            --iterator;
            bitsIterator.next()->setState(*iterator == '1');
        } else
            bitsIterator.next()->setState(false);
    }
}

void BitFieldWidget::onBitChanged()
{
    QListIterator<BitWidget*> bitsIterator(m_bitWidgets);
    QString expression;

    while (bitsIterator.hasNext())
        expression.prepend(bitsIterator.next()->state() ? "1" : "0");

    expression.remove(QRegExp(QString("^0{,%1}").arg(NumberOfBits - 1)));
    expression.prepend("0b");

    emit bitsChanged(expression);
}

void BitFieldWidget::invertBits()
{
    foreach (BitWidget* w, m_bitWidgets)
        w->setState(!w->state());

    onBitChanged();
}

void BitFieldWidget::clear()
{
    for (auto& w : m_bitWidgets)
        w->setState(false);
}

void BitFieldWidget::resetBits()
{
    clear();
    onBitChanged();
}

void BitFieldWidget::shiftBitsLeft()
{
    auto it = m_bitWidgets.constEnd();
    auto itBegin = m_bitWidgets.constBegin();

    --it;
    while (it != itBegin) {
        (*it)->setState((*(it-1))->state());
        --it;
    }

    (*itBegin)->setState(false);
    onBitChanged();
}

void BitFieldWidget::shiftBitsRight()
{
    auto it = m_bitWidgets.constBegin();
    auto itEnd = m_bitWidgets.constEnd();

    --itEnd;
    while (it != itEnd) {
        (*it)->setState((*(it+1))->state());
        it++;
    }

    (*itEnd)->setState(false);
    onBitChanged();
}
