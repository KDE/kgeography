/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
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

void popupManager::show(QString text, const QPoint &p)
{
	delete p_mp;
	p_mp = new myPopup(text, p, p_parent);
	connect(p_mp, SIGNAL(deleteMe()), this, SLOT(clear()));
}

void popupManager::clear()
{
	delete p_mp;
	p_mp = 0;
}
