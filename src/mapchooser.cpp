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
#include <qlayout.h>

#include "mapchooser.h"

mapChooser::mapChooser(QWidget *parent) : KDialogBase(parent, 0, true, i18n("Choose Map to Use"), KDialogBase::Ok | KDialogBase::Cancel, KDialogBase::Ok, true)
{
	QHBox *mainHB;
	QStringList list;
	KGmap *m;
	QWidget *mapArea;
	QGridLayout *mapLay;
	
	mainHB = new QHBox(this);
	mainHB -> setSpacing(KDialog::spacingHint());
	list = KGlobal::dirs() -> findAllResources("appdata", "*.kgm");
	p_listBox = new QListBox(mainHB);
	QStringList::iterator it;
	for(it = list.begin(); it != list.end(); ++it)
	{
		if (!p_reader.parseMap(*it))
		{
			KMessageBox::error(this, i18n("Error parsing %1: %2").arg(*it).arg(p_reader.getError()));
			delete p_reader.getMap();
		}
		else
		{
			m = p_reader.getMap();
			QString text = i18n(m -> getFileName().utf8(), m -> getName().utf8());
			p_listBox -> insertItem(text);
			p_maps.insert(text, m);
		}
	}
	
	mapArea = new QWidget(mainHB);
	mapArea -> setFixedSize(300, 225);
	
	mapLay = new QGridLayout(mapArea, 3, 3);
	
	p_image = new QWidget(mapArea);
	mapLay -> addWidget(p_image, 1, 1);
	
	connect(p_listBox, SIGNAL(highlighted(const QString&)), this, SLOT(putImage(const QString&)));
	connect(p_listBox, SIGNAL(selected(int)), this, SLOT(slotOk()));
	
	setMainWidget(mainHB);
	
	p_listBox -> sort();
	if (p_listBox -> count() > 0) p_listBox -> setCurrentItem(0);
	else enableButtonOK(false);
}

mapChooser::~mapChooser()
{
	p_maps.setAutoDelete(true);
	p_maps.clear();
}

KGmap *mapChooser::getMap()
{
	KGmap *m;
	m = p_maps[p_listBox -> currentText()];
	p_maps.remove(p_listBox -> currentText());
	return m;
}

void mapChooser::putImage(const QString &mapName)
{
	KGmap *m;
	m = p_maps[mapName];
	QImage image(m -> getMapFile());
	image = image.smoothScale(300, 225, QImage::ScaleMin);
	p_image -> setPaletteBackgroundPixmap(image);
	p_image -> setFixedSize(image.size());
}

#include "mapchooser.moc"
