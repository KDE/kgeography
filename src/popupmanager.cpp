/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "popupmanager.h"

#include "mypopup.h"

popupManager::popupManager()
{
	p_parent = 0;
	p_mp = 0;
}

void popupManager::setWidget(QWidget *w)
{
	p_parent = w;
}

void popupManager::show(const QString &text, const QString &wikiLink, const QString &text2, const QPoint &p, const QString &flagFile)
{
	delete p_mp;

	p_mp = new myPopup(p_parent, text, wikiLink, text2, flagFile);
	init(p);
}

void popupManager::show(const QString &text, const QString &wikiLink, const QString &text2, const QPoint &p)
{
	delete p_mp;
	
	p_mp = new myPopup(p_parent, text, wikiLink, text2);
	init(p);
}

void popupManager::show(const QString &text, const QString &wikiLink, const QPoint &p)
{
	delete p_mp;
	
	p_mp = new myPopup(p_parent, text, wikiLink);
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
	connect(p_mp, &myPopup::deleteMe, this, &popupManager::clear);
}

#include "moc_popupmanager.cpp"
