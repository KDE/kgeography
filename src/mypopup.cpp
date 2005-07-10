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

#include "mypopup.h"


myPopup::myPopup(QWidget *parent, const QString &text, const QString &text2, const QString &flagFile) : QFrame(parent)
{
	QHBoxLayout *lay = new QHBoxLayout(this);
	lay -> setMargin(4);
	lay -> setSpacing(4);
	
	QWidget *vbox = new QWidget(this);
	lay -> addWidget(vbox);
	QVBoxLayout *vboxLayout = new QVBoxLayout(vbox);
	vboxLayout -> setMargin(0);
	vboxLayout -> setSpacing(0);
	QLabel *l = new QLabel(text, vbox);
	vboxLayout -> addWidget(l);
	
	if (!text2.isNull())
	{
		QLabel *l2 = new QLabel(text2, vbox);
		l2 -> setAlignment(Qt::AlignCenter);
		vboxLayout -> addWidget(l2);
	}
	
	if (!flagFile.isNull())
	{
		QLabel *flag = new QLabel(this);
		lay -> addWidget(flag);
		QImage flagImg(flagFile);
		flag -> setPixmap(flagImg.smoothScale(flagImg.width() / 5, flagImg. height() / 5));
		flag -> setAlignment(Qt::AlignCenter);
	}
	
	l -> setAlignment(Qt::AlignCenter);
	QFont f = l -> font();
	f.setBold(true);
	l -> setFont(f);
	
	
	setFrameStyle(QFrame::Box | QFrame::Plain);
	setLineWidth(2);
	
	setFixedSize(sizeHint());
}

void myPopup::mousePressEvent(QMouseEvent *)
{
	emit deleteMe();
}

#include "mypopup.moc"
