/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <qlabel.h>

#include "mypopup.h"
 
myPopup::myPopup(QString text, const QPoint &p, QWidget *parent) : QHBox(parent, 0, WStyle_NoBorder)
{
	QLabel *l = new QLabel(text, this);
	l -> setAlignment(Qt::AlignCenter);
	move(p);
	setFrameStyle(QFrame::Box | QFrame::Plain);
	setLineWidth(2);
	setFixedSize(l -> sizeHint() + QSize(5, 5));
	show();
}

void myPopup::mousePressEvent(QMouseEvent *)
{
	emit deleteMe();
}
