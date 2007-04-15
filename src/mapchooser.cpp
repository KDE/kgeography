/***************************************************************************
 *   Copyright (C) 2004-2006 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "mapchooser.h"

#include <klocale.h>
#include <kglobal.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>

#include <qimage.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlistwidget.h>
#include <qpainter.h>

mapChooser::mapChooser(QWidget *parent) : KDialog(parent)
{
	setCaption(i18n("Choose Map to Use"));
	setButtons(KDialog::Ok | KDialog::Cancel);

	QWidget *mainHB;
	QHBoxLayout *mainHBLayout;
	QStringList list;
	KGmap *m;
	
	mainHB = new QWidget(this);
	mainHBLayout = new QHBoxLayout(mainHB);
	mainHBLayout -> setMargin(0);
	mainHBLayout -> setSpacing(KDialog::spacingHint());
	list = KGlobal::dirs() -> findAllResources("appdata", "*.kgm");
	p_listBox = new QListWidget(mainHB);
	mainHBLayout -> addWidget(p_listBox);
	QStringList::iterator it;
	for(it = list.begin(); it != list.end(); ++it)
	{
		m = p_reader.parseMap(*it);
		if (!m)
		{
			KMessageBox::error(this, i18n("Error parsing %1: %2", *it, p_reader.getError()));
		}
		else
		{
			QString text = i18nc(m -> getFileName().toUtf8(), m -> getName().toUtf8());
			p_listBox -> addItem(text);
			p_maps.insert(text, m);
		}
	}
	
	p_image = new QLabel(mainHB);
	p_image -> setFixedSize(300, 225);
	p_image -> setAlignment(Qt::AlignCenter);
	mainHBLayout -> addWidget(p_image);
	
	connect(p_listBox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(putImage(const QString&)));
	connect(p_listBox, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(accept()));
	
	setMainWidget(mainHB);
	
	p_listBox -> sortItems();
	if (p_listBox -> count() > 0) p_listBox -> setCurrentRow(0);
	else enableButtonOk(false);
	p_listBox -> setFocus();
}

mapChooser::~mapChooser()
{
	qDeleteAll(p_maps.values());
}

KGmap *mapChooser::getMap()
{
	KGmap *m;
	m = p_maps[p_listBox -> currentItem() -> text()];
	p_maps.remove(p_listBox -> currentItem() -> text());
	return m;
}

void mapChooser::putImage(const QString &mapName)
{
	KGmap *m;
	m = p_maps[mapName];
	QImage image(m -> getMapFile());
	image = image.scaled(300, 225, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	p_image -> setPixmap( QPixmap::fromImage(image) );
}

#include "mapchooser.moc"
