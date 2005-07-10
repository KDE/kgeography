/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "mypopup.h"
#include "popupmanager.h"

popupManager::popupManager(QWidget *parent)
{
	p_parent = parent;
	p_mp = 0;
}

void popupManager::show(const QString &text, const QString &text2, const QPoint &p, const QString &flagFile)
{
	delete p_mp;

	p_mp = new myPopup(p_parent, text, text2, flagFile);
	init(p);
}

void popupManager::show(const QString &text, const QString &text2, const QPoint &p)
{
	delete p_mp;
	
	p_mp = new myPopup(p_parent, text, text2);
	init(p);
}

void popupManager::show(const QString &text, const QPoint &p)
{
	delete p_mp;
	
	p_mp = new myPopup(p_parent, text);
	init(p);
}

void popupManager::clear()
{
	if (p_mp)
	{
		p_mp -> deleteLater();
		p_mp = 0;
	}
}

void popupManager::init(const QPoint &p)
{
	int x, y, maxX, maxY;
	maxX = p_parent -> width() - p_mp -> width();
	maxY = p_parent -> height() - p_mp -> height();
	if (p.x() < maxX) x = p.x();
	else x = maxX;
	if (p.y() < maxY) y = p.y();
	else y = maxY;
	p_mp -> move(x, y);
	p_mp -> show();
	connect(p_mp, SIGNAL(deleteMe()), this, SLOT(clear()));
}

#include "popupmanager.moc"
