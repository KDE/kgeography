/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <tqlabel.h>
#include <tqlayout.h>
#include <tqimage.h>
#include <tqpixmap.h>

#include "mypopup.h"


myPopup::myPopup(TQWidget *parent, const TQString &text, const TQString &text2, const TQString &flagFile) : TQFrame(parent)
{
	TQHBoxLayout *lay = new TQHBoxLayout(this);
	lay -> setMargin(4);
	lay -> setSpacing(4);
	
	TQWidget *vbox = new TQWidget(this);
	lay -> addWidget(vbox);
	TQVBoxLayout *vboxLayout = new TQVBoxLayout(vbox);
	vboxLayout -> setMargin(0);
	vboxLayout -> setSpacing(0);
	TQLabel *l = new TQLabel(text, vbox);
	vboxLayout -> addWidget(l);
	
	if (!text2.isNull())
	{
		TQLabel *l2 = new TQLabel(text2, vbox);
		l2 -> setAlignment(Qt::AlignCenter);
		vboxLayout -> addWidget(l2);
	}
	
	if (!flagFile.isNull())
	{
		TQLabel *flag = new TQLabel(this);
		lay -> addWidget(flag);
		TQImage flagImg(flagFile);
		flag -> setPixmap(flagImg.smoothScale(flagImg.width() / 5, flagImg. height() / 5));
		flag -> setAlignment(Qt::AlignCenter);
	}
	
	l -> setAlignment(Qt::AlignCenter);
	TQFont f = l -> font();
	f.setBold(true);
	l -> setFont(f);
	
	
	setFrameStyle(TQFrame::Box | TQFrame::Plain);
	setLineWidth(2);
	
	setFixedSize(sizeHint());
}

void myPopup::mousePressEvent(TQMouseEvent *)
{
	emit deleteMe();
}

#include "mypopup.moc"
