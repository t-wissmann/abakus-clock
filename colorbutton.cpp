/***************************************************************************
 *   Copyright (C) 2007 by Thorsten Wissmann                               *
 *   E-Mail: kollegstufe@thorsten-wissmann.de                              *
 *   Homepage: www.thorsten-wissmann.de                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "colorbutton.h"
#include <QSizePolicy>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QPalette>
#include <QRect>
#include <QColorDialog>
// for drag&drop
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QMimeData>
#include <QPixmap>

ColorButton::ColorButton(QWidget *parent)
 : QPushButton(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(this, SIGNAL(clicked()), this, SLOT(getColorFromDialog()));
    setAcceptDrops(TRUE);
}


ColorButton::~ColorButton()
{
}

void ColorButton::paintEvent(QPaintEvent* event)
{
    QPushButton::paintEvent(event);
    QBrush   bgBrush(Qt::SolidPattern);
    QPen     pen;
    pen.setColor(palette().highlight().color());
    pen.setWidth(1);
    
    int marginleft, margintop, marginright, marginbottom;
    getContentsMargins(&marginleft, &margintop, &marginright, &marginbottom);
    //qDebug("%d, %d, %d, %d", marginleft, margintop, marginright, marginbottom);
    
    // simulate border
    marginleft   += 5; // 5px space on the left
    margintop    += 5;
    marginright  += 5;
    marginbottom += 5;
    
    QRect rect(marginleft, margintop, width()-marginleft-marginright, height()-margintop-marginbottom);
    
    QPainter painter(this);
    // white background
    bgBrush = palette().base();
    if(isDown())
    {
        bgBrush.setColor(bgBrush.color().darker(120));
    }
    else if(underMouse())
    {
        bgBrush.setColor(bgBrush.color().lighter(120));
    }
    painter.setPen(Qt::NoPen);
    painter.setBrush(bgBrush);
    painter.drawRect(rect);
    
    // highlighted border
    rect.adjust(1, 1, -2, -2);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(rect);
    
    // draw current m_cColor
    QColor colWithAlpha = m_cColor;
    colWithAlpha.setAlpha(255); // set alpha to 100%, so that button is shown solid
    rect.adjust(2, 2, -1, -1);
    bgBrush.setColor(colWithAlpha);
    
    painter.setPen(Qt::NoPen);
    if(isDown())
    {
        bgBrush.setColor(bgBrush.color().darker(120));
    }
    else if(underMouse())
    {
        bgBrush.setColor(bgBrush.color().lighter(120));
    }
    painter.setBrush(bgBrush);
    painter.drawRect(rect);
    
                     
}



void ColorButton::getColorFromDialog()
{
    QColor newColor = QColorDialog::getColor(m_cColor, this);
    if(newColor.isValid())
    {// only apply changes if color is valid, i.e. user didn't cancel the color dialog
        setColor(newColor);
    }
}


void ColorButton::setColor(const QColor& newColor)
{
    m_cColor = newColor;
    update();
    emit colorChanged(m_cColor);
}


void ColorButton::dragEnterEvent(QDragEnterEvent *event)
{
    if(!event) return;
    if(event->mimeData()->hasText() ||
       event->mimeData()->hasColor())
    { // only accept plain text
        event->acceptProposedAction();
    }
}

void ColorButton::dropEvent(QDropEvent *event)
{
    QString colorname = event->mimeData()->text();
    QColor newColor(colorname);
    setColor(newColor);
}

void ColorButton::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton){
        m_cDragStartPosition = event->pos();
    }
    QPushButton::mousePressEvent(event);
}

void ColorButton::mouseMoveEvent(QMouseEvent* event)
{
    if(event->buttons() & Qt::LeftButton){
        int distance = (event->pos() - m_cDragStartPosition).manhattanLength();
        if(distance >= QApplication::startDragDistance()){
            // start drag
            QMimeData* mimeData = new QMimeData;
            mimeData->setText(m_cColor.name());
            QDrag* drag = new QDrag(this);
            QPixmap colorrect(width()-4, height()-4);
            colorrect.fill(m_cColor);
            drag->setMimeData(mimeData);
            drag->setPixmap(colorrect);
            drag->start(Qt::MoveAction);
        }
    }
    else
    {
        m_cDragStartPosition = QPoint(-33, -33);
    }
    QPushButton::mouseMoveEvent(event);
}


