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
#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QColor>
#include <QPushButton>
#include <QPoint>
class QPaintEvent;

class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;

/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/
class ColorButton : public QPushButton
{
Q_OBJECT
public:
    ColorButton(QWidget *parent = 0);

    ~ColorButton();
    QColor color() const { return m_cColor; };
    void setColor(const QColor& newColor);
public slots:
    void getColorFromDialog();
signals:
    void colorChanged(QColor newColor);
protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
private:
    QColor m_cColor;
    QPoint m_cDragStartPosition;
};

#endif
