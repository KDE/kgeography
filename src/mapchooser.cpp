/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <klocale.h>
#include <kglobal.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>

#include <qhbox.h>
#include <qimage.h>

#include "mapchooser.h"

mapChooser::mapChooser(QWidget *parent) : KDialogBase(parent, 0, true, i18n("Choose map to use"), KDialogBase::Ok | KDialogBase::Cancel, KDialogBase::Ok, true)
{
	QHBox *hb;
	QStringList list;
	map *m;
	hb = new QHBox(this);
	list = KGlobal::dirs() -> findAllResources("appdata", "*.kgm");
	p_listBox = new QListBox(hb);
	QStringList::iterator it;
	for(it = list.begin(); it != list.end(); ++it)
	{
		if (!p_reader.parseMap(*it))
		{
			KMessageBox::error(this, i18n("Error parsing %1: %2").arg(*it).arg(p_reader.getError()));
		}
		else
		{
			m = p_reader.getMap();
			p_listBox -> insertItem(m -> getName());
			p_maps.insert(m -> getName(), m);
		}
	}
	p_image = new QWidget(hb);
	p_image -> setFixedSize(300, 225);
	connect(p_listBox, SIGNAL(highlighted(const QString&)), this, SLOT(putImage(const QString&)));
	connect(p_listBox, SIGNAL(selected(int)), this, SLOT(slotOk()));
	p_listBox -> setCurrentItem(0);
	setMainWidget(hb);
}

mapChooser::~mapChooser()
{
	p_maps.setAutoDelete(true);
	p_maps.clear();
}

map *mapChooser::getMap()
{
	map *m;
	m = p_maps[p_listBox -> currentText()];
	p_maps.remove(p_listBox -> currentText());
	return m;
}

void mapChooser::putImage(const QString &mapName)
{
	map *m;
	m = p_maps[mapName];
	p_image -> setPaletteBackgroundPixmap(QImage(m -> getMapFile()).scale(300, 225));
}

#include "mapchooser.moc"
