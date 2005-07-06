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

myPopup::myPopup(QWidget *parent, const QString &text, const QString &text2, const QString &flagFile) : QPopupMenu(parent)
{
	setLineWidth(2);
	setFrameStyle(QFrame::Box | QFrame::Plain);

	myPopupItem *p = new myPopupItem(this, text, text2, flagFile);
	insertItem(p, 0);

	connect(p, SIGNAL(click()), this, SLOT(popupClick()));
	connect(this, SIGNAL(activated(int)), this, SLOT(slotActivated(int)));

}

void myPopup::popupClick()
{
	slotActivated(0);
}

void myPopup::slotActivated(int)
{
	emit deleteMe();
}


myPopupItem::myPopupItem(myPopup *parent, const QString &text, const QString &text2, const QString &flagFile) : QFrame(parent, 0, WStyle_NoBorder)
{
	QHBoxLayout *lay = new QHBoxLayout(this, 1, 1);
	lay -> setAutoAdd(true);
	
	QVBox *vbox = new QVBox(this);
	QLabel *l = new QLabel(text, vbox);
	
	if (!text2.isNull())
	{
		QLabel *l2 = new QLabel(text2, vbox);
		l2 -> setAlignment(Qt::AlignCenter);
	}
	
	if (!flagFile.isNull())
	{
		QLabel *flag = new QLabel(this);
		QImage flagImg(flagFile);
		flag -> setPixmap(flagImg.smoothScale(flagImg.width() / 5, flagImg.height() / 5));
		flag -> setAlignment(Qt::AlignCenter);
	}
	
	l -> setAlignment(Qt::AlignCenter);
	QFont f = l -> font();
	f.setBold(true);
	l -> setFont(f);
	
	setFixedSize(sizeHint());
	show();
}

void myPopupItem::mousePressEvent(QMouseEvent *)
{
	emit click();
}

#include "mypopup.moc"
