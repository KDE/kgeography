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

#include "capitaldivisionasker.h"
#include "divisioncapitalasker.h"
#include "divisionflagasker.h"
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
	p_askCapitalDivisions = new KPushButton(i18n("Guess division from its &capital"), holder);
	p_askDivisionCapitals = new KPushButton(i18n("Guess the capital of a &division"), holder);
	p_askFlagDivisions = new KPushButton(i18n("&Guess division from its flag"), holder);
	p_askDivisionFlags = new KPushButton(i18n("G&uess the flag of a division"), holder);
	
	p_goToMenu = new KAction(i18n("&Go to menu"), 0, this, SLOT(goToMenu()), actionCollection(), "goToMenu");
	p_goToMenu -> setEnabled(false);
	
	connect(p_consult, SIGNAL(clicked()), this, SLOT(consult()));
	connect(p_askMap, SIGNAL(clicked()), this, SLOT(askMap()));
	connect(p_askCapitalDivisions, SIGNAL(clicked()), this, SLOT(askCapitalDivisions()));
	connect(p_askDivisionCapitals, SIGNAL(clicked()), this, SLOT(askDivisionCapitals()));
	connect(p_askFlagDivisions, SIGNAL(clicked()), this, SLOT(askFlagDivisions()));
	connect(p_askDivisionFlags, SIGNAL(clicked()), this, SLOT(askDivisionFlags()));
	
	setCentralWidget(p_stack);
	
	KStdAction::open(this, SLOT(openMap()), actionCollection(), "openMap");
	KStdAction::quit(this, SLOT(close()), actionCollection(), "quit");
	
	p_zoom = new KToggleAction(i18n("&Zoom"), "viewmag", 0, 0, 0, actionCollection(), "zoom");
	p_zoom -> setEnabled(false);
	
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
	if (!p_map)
	{
		p_currentMap -> setText(i18n("There is no current map"));
		p_consult -> setEnabled(false);
		p_askMap -> setEnabled(false);
		p_askFlagDivisions -> setEnabled(false);
		p_askDivisionFlags -> setEnabled(false);
		p_askCapitalDivisions -> setEnabled(false);
		p_askDivisionCapitals -> setEnabled(false);
	}
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
	connect(p_zoom, SIGNAL(toggled(bool)), p_askWidget, SLOT(setZoom(bool)));
	p_zoom -> setEnabled(true);
	putAskWidget();
}

void kgeography::askCapitalDivisions()
{
	int i;
	bool ok;
	i = KInputDialog::getInteger(i18n("Number of questions"), i18n("How many questions do you want? (1 to %1)").arg(p_map -> count()), 1, 1, p_map -> count(), 1, &ok);
	if (ok)
	{
		p_askWidget = new capitalDivisionAsker(p_stack, p_map, i);
		putAskWidget();
	}
}

void kgeography::askDivisionCapitals()
{
	int i;
	bool ok;
	i = KInputDialog::getInteger(i18n("Number of questions"), i18n("How many questions do you want? (1 to %1)").arg(p_map -> count()), 1, 1, p_map -> count(), 1, &ok);
	if (ok)
	{
		p_askWidget = new divisionCapitalAsker(p_stack, p_map, i);
		putAskWidget();
	}
}

void kgeography::askMap()
{
	int i;
	bool ok;
	i = KInputDialog::getInteger(i18n("Number of questions"), i18n("How many questions do you want? (1 to %1)").arg(p_map -> count()), 1, 1, p_map -> count(), 1, &ok);
	if (ok)
	{
		p_askWidget = new mapAsker(p_stack, p_map, true, i);
		connect(p_zoom, SIGNAL(toggled(bool)), p_askWidget, SLOT(setZoom(bool)));
		p_zoom -> setEnabled(true);
		putAskWidget();
	}
}

void kgeography::askFlagDivisions()
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

void kgeography::askDivisionFlags()
{
	int i;
	bool ok;
	i = KInputDialog::getInteger(i18n("Number of questions"), i18n("How many questions do you want? (1 to %1)").arg(p_map -> count()), 1, 1, p_map -> count(), 1, &ok);
	if (ok)
	{
		p_askWidget = new divisionFlagAsker(p_stack, p_map, i);
		putAskWidget();
	}
}

void kgeography::putMenu()
{
	p_stack -> raiseWidget(0);
	p_stack -> removeWidget(p_askWidget);
	delete p_askWidget;
	p_askWidget = 0;
	p_zoom -> setEnabled(false);
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
	p_consult -> setEnabled(true);
	p_askMap -> setEnabled(true);
	p_askFlagDivisions -> setEnabled(m -> hasAllFlags());
	p_askDivisionFlags -> setEnabled(m -> hasAllFlags());
	p_askCapitalDivisions -> setEnabled(true);
	p_askDivisionCapitals -> setEnabled(true);
	putMenu();
}

#include "kgeography.moc"
