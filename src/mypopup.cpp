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
#include <qimage.h>
#include <qlayout.h> 
#include <qpixmap.h>
#include <qimage.h>

#include "mypopup.h"

myPopup::myPopup(const QString &text, const QString &mapFile, QWidget *parent) : QHBox(parent, 0, WStyle_NoBorder)
{
	QLabel *l = new QLabel(text, this);
	QLabel *flag = new QLabel (this);
	QImage *flagImg = new QImage (mapFile);
	flag -> setPixmap (flagImg -> smoothScale(flagImg -> width() / 5, flagImg -> height() / 5));
	l -> setAlignment(Qt::AlignCenter);
	setFrameStyle(QFrame::Box | QFrame::Plain);
	setLineWidth(2);
	setFixedSize(this -> sizeHint() + QSize(5, 5));
	show();
}
myPopup::myPopup(const QString &text, QWidget *parent) : QHBox(parent, 0, WStyle_NoBorder)
{
	QLabel *l = new QLabel(text, this);
	l -> setAlignment(Qt::AlignCenter);
	setFrameStyle(QFrame::Box | QFrame::Plain);
	setLineWidth(2);
	setFixedSize(l -> sizeHint() + QSize(5, 5));
	show();
}

void myPopup::mousePressEvent(QMouseEvent *)
{
	emit deleteMe();
}

#include "mypopup.moc"
