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

#include <qimage.h>
#include <qlayout.h>
#include <qlistwidget.h>
#include <qpainter.h>

#include "mapchooser.h"


class imageWidget : public QWidget
{
	public:
		imageWidget(QWidget *parent) : QWidget(parent)
		{
		}

		void paintEvent(QPaintEvent *)
		{
			QPainter p(this);
			p.drawPixmap(0, 0, pix);
		}
		
		QPixmap pix;
};

mapChooser::mapChooser(QWidget *parent) : KDialogBase(parent, 0, true, i18n("Choose Map to Use"), KDialogBase::Ok | KDialogBase::Cancel, KDialogBase::Ok, true)
{
	QWidget *mainHB;
	QHBoxLayout *mainHBLayout;
	QStringList list;
	KGmap *m;
	QWidget *mapArea;
	QGridLayout *mapLay;
	
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
		if (!p_reader.parseMap(*it))
		{
			KMessageBox::error(this, i18n("Error parsing %1: %2").arg(*it).arg(p_reader.getError()));
			delete p_reader.getMap();
		}
		else
		{
			m = p_reader.getMap();
			QString text = i18n(m -> getFileName().utf8(), m -> getName().utf8());
			p_listBox -> addItem(text);
			p_maps.insert(text, m);
		}
	}
	
	mapArea = new QWidget(mainHB);
	mapArea -> setFixedSize(300, 225);
	mainHBLayout -> addWidget(mapArea);
	
	mapLay = new QGridLayout(mapArea);
	mapLay -> setMargin(3);
	mapLay -> setSpacing(3);
	
	p_image = new imageWidget(mapArea);
	mapLay -> addWidget(p_image, 1, 1);
	
	connect(p_listBox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(putImage(const QString&)));
	connect(p_listBox, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slotOk()));
	
	setMainWidget(mainHB);
	
	p_listBox -> sortItems();
	if (p_listBox -> count() > 0) p_listBox -> setCurrentRow(0);
	else enableButtonOK(false);
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
	p_image -> pix = QPixmap::fromImage(image);
	p_image -> setFixedSize(image.size());
}

#include "mapchooser.moc"
