/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <kaction.h>
#include <klocale.h>
#include <kinputdialog.h>
#include <kmessagebox.h>
#include <kpushbutton.h>
#include <kstdaction.h>

#include <qlabel.h>
#include <qsizepolicy.h> 
#include <qvbox.h>
#include <qwidgetstack.h>

#include "flagdivisionasker.h"
#include "kgeography.h"
#include "settings.h"
#include "mapasker.h"
#include "mapchooser.h"
#include "mapparser.h"
#include "map.h"

kgeography::kgeography() : KMainWindow()
{
	QString file;
	
	p_map = 0;
	p_askWidget = 0;
	
	p_stack = new QWidgetStack(this);
	QVBox *holder = new QVBox(p_stack);
	p_stack -> addWidget(holder, 0);
	p_currentMap = new QLabel(holder);
	p_currentMap -> setAlignment(AlignCenter);
	p_consult = new KPushButton(i18n("&Browse the map"), holder);
	p_askMap = new KPushButton(i18n("&Click division in the map"), holder);
	p_askFlags = new KPushButton(i18n("&Guess division from its flag"), holder);
	
	p_goToMenu = new KAction(i18n("&Go to menu"), 0, this, SLOT(goToMenu()), actionCollection(), "goToMenu");
	p_goToMenu -> setEnabled(false);
	
	connect(p_consult, SIGNAL(clicked()), this, SLOT(consult()));
	connect(p_askMap, SIGNAL(clicked()), this, SLOT(askMap()));
	connect(p_askFlags, SIGNAL(clicked()), this, SLOT(askFlags()));
	
	setCentralWidget(p_stack);
	
	KStdAction::open(this, SLOT(openMap()), actionCollection(), "openMap");
	KStdAction::quit(this, SLOT(close()), actionCollection(), "quit");
	
	createGUI();
	show();
	
	file = kgeographySettings::self() -> lastMap();
	if (QFile::exists(file))
	{
		mapReader reader;
		if (reader.parseMap(file))
		{
			setMap(reader.getMap());
		}
		else
		{
			KMessageBox::error(this, i18n("Could not open last used map. Error parsing %1: %2").arg(file).arg(reader.getError()));
			delete reader.getMap();
			openMap();
		}
	}
	else openMap();
}

kgeography::~kgeography()
{
	delete p_map;
}

void kgeography::goToMenu()
{
	p_askWidget -> disconnect();
	p_askWidget -> goToMenu();
	putMenu();
}

void kgeography::openMap()
{
	mapChooser mp(this);
	if (mp.exec() == mapChooser::Accepted)
	{
		delete p_map;
		setMap(mp.getMap());
	}
}

void kgeography::consult()
{
	p_askWidget = new mapAsker(p_stack, p_map, false);
	putAskWidget();
}

void kgeography::askMap()
{
	int i;
	bool ok;
	i = KInputDialog::getInteger(i18n("Number of questions"), i18n("How many questions do you want? (1 to %1)").arg(p_map -> count()), 1, 1, p_map -> count(), 1, &ok);
	if (ok)
	{
		p_askWidget = new mapAsker(p_stack, p_map, true, i);
		putAskWidget();
	}
}

void kgeography::askFlags()
{
	int i;
	bool ok;
	i = KInputDialog::getInteger(i18n("Number of questions"), i18n("How many questions do you want? (1 to %1)").arg(p_map -> count()), 1, 1, p_map -> count(), 1, &ok);
	if (ok)
	{
		p_askWidget = new flagDivisionAsker(p_stack, p_map, i);
		putAskWidget();
	}
}

void kgeography::putMenu()
{
	p_stack -> raiseWidget(0);
	p_stack -> removeWidget(p_askWidget);
	delete p_askWidget;
	p_goToMenu -> setEnabled(false);
}

void kgeography::putAskWidget()
{
	p_stack -> raiseWidget(p_askWidget);
	connect(p_askWidget, SIGNAL(finished()), this, SLOT(putMenu()));
	p_goToMenu -> setEnabled(true);
}

void kgeography::setMap(map *m)
{
	kgeographySettings *set = kgeographySettings::self();
	set -> setLastMap(m -> getFile());
	set -> writeConfig();
	p_map = m;
	p_currentMap -> setText(i18n("Current map is %1").arg(p_map -> getName()));
	p_askFlags -> setEnabled(m -> hasAllFlags());
	putMenu();
}

#include "kgeography.moc"
