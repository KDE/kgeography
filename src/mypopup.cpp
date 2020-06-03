/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "mypopup.h"

#include <kiconloader.h>
#include <klocalizedstring.h>

#include <qevent.h>
#include <qicon.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qimage.h>
#include <qdesktopservices.h>

myPopup::myPopup(QWidget *parent, const QString &text, const QString &wikiLink, const QString &text2, const QString &flagFile) : QFrame(parent, Qt::FramelessWindowHint)
{
	wikipedia = wikiLink;
	QHBoxLayout *lay = new QHBoxLayout(this);
	lay -> setContentsMargins(4, 4, 4, 4);
	lay -> setSpacing(4);
	
	if (!wikiLink.isEmpty())
	{
		wiki = new QLabel(this);
		lay -> addWidget(wiki);
		wiki -> setPixmap(QIcon::fromTheme( QStringLiteral("dialog-information") ).pixmap(KIconLoader::SizeSmall));
		wiki -> setToolTip(i18n("Wikipedia page"));
		wiki -> setAlignment(Qt::AlignCenter);
		wiki -> installEventFilter(this);
	}
	
	QWidget *vbox = new QWidget(this);
	lay -> addWidget(vbox);
	
	QVBoxLayout *vboxLayout = new QVBoxLayout(vbox);
	vboxLayout -> setContentsMargins(0, 0, 0, 0);
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
		flag -> setPixmap(QPixmap::fromImage(flagImg.scaled(flagImg.width() / 5, flagImg. height() / 5, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
		flag -> setAlignment(Qt::AlignCenter);
	}
	
	l -> setAlignment(Qt::AlignCenter);
	QFont f = l -> font();
	f.setBold(true);
	l -> setFont(f);
	
	
	setFrameStyle(QFrame::Box | QFrame::Plain);
	setLineWidth(2);
	
	setFixedSize(sizeHint());
	setAutoFillBackground(true);
}

void myPopup::mousePressEvent(QMouseEvent *)
{
	emit deleteMe();
}

bool myPopup::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == wiki)
	{
		
		if (event -> type() == QEvent::MouseButtonPress)
		{
			QDesktopServices::openUrl(QUrl(wikipedia));
			return true;
		}
		else
		{
			return false;
		}
	}
	emit deleteMe();

	return false;
}

