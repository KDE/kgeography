/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <qlabel.h>
#include <qlayout.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qvbox.h>

#include "mypopup.h"

#include <kdebug.h>

myPopup::myPopup(const QString &text, const QString &text2, const QString &flagFile, QWidget *parent) : QFrame(parent, 0, WStyle_NoBorder)
{
	QHBoxLayout *lay = new QHBoxLayout(this, 4, -1);
	lay -> setAutoAdd(true);
	
	QVBox *vbox = new QVBox(this);
	QLabel *l = new QLabel(text, vbox);
	QLabel *l2 = new QLabel(text2, vbox);
	QLabel *flag = new QLabel(this);
	QImage *flagImg = new QImage(flagFile);
	flag -> setPixmap(flagImg -> smoothScale(flagImg -> width() / 5, flagImg -> height() / 5));
	l -> setAlignment(Qt::AlignCenter);
	QFont f = l -> font();
	f.setBold(true);
	l -> setFont(f);
	l2 -> setAlignment(Qt::AlignCenter);
	flag -> setAlignment(Qt::AlignCenter);
	setFrameStyle(QFrame::Box | QFrame::Plain);
	setLineWidth(2);
	
	setFixedSize(sizeHint());
	show();
}

myPopup::myPopup(const QString &text1, const QString &text2, QWidget *parent) : QFrame(parent, 0, WStyle_NoBorder)
{
	QVBoxLayout *lay = new QVBoxLayout(this, 4, -1);
	lay -> setAutoAdd(true);

	QLabel *l = new QLabel(text1, this);
	l -> setAlignment(Qt::AlignCenter);
	QFont f = l -> font();
	f.setBold(true);
	l -> setFont(f);
	QLabel *l2 = new QLabel(text2, this);
	l2 -> setAlignment(Qt::AlignCenter);
	setFrameStyle(QFrame::Box | QFrame::Plain);
	setLineWidth(2);
	setFixedSize(sizeHint());
	show();
}

myPopup::myPopup(const QString &text, QWidget *parent) : QFrame(parent, 0, WStyle_NoBorder)
{
	QHBoxLayout *lay = new QHBoxLayout(this, 4, -1);
	lay -> setAutoAdd(true);

	QLabel *l = new QLabel(text, this);
	l -> setAlignment(Qt::AlignCenter);
	setFrameStyle(QFrame::Box | QFrame::Plain);
	setLineWidth(2);
	setFixedSize(sizeHint());
	show();
}

void myPopup::mousePressEvent(QMouseEvent *)
{
	emit deleteMe();
}

#include "mypopup.moc"
