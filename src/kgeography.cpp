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
#include <kdeversion.h>
#include <kdialog.h>
#include <klocale.h>
#include <kinputdialog.h>
#include <kmessagebox.h>
#include <kpushbutton.h>
#include <kstdaction.h>

#include <qlabel.h>
#include <qlayout.h>
#include <qvbox.h>

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

	p_bigWidget = new QHBox(this);

	QVBox *p_leftWidget = new QVBox(p_bigWidget);
	p_currentMap = new QLabel(p_leftWidget);
	p_currentMap -> setAlignment(AlignCenter);
	p_consult = new KPushButton(i18n("&Browse the map"), p_leftWidget);
	p_askMap = new KPushButton(i18n("&Click division in the map"), p_leftWidget);
	p_askCapitalDivisions = new KPushButton(i18n("Guess division from its &capital"), p_leftWidget);
	p_askDivisionCapitals = new KPushButton(i18n("Guess the capital of a &division"), p_leftWidget);
	p_askFlagDivisions = new KPushButton(i18n("&Guess division from its flag"), p_leftWidget);
	p_askDivisionFlags = new KPushButton(i18n("G&uess the flag of a division"), p_leftWidget);
	p_underLeftWidget = new QVBox(p_leftWidget);
	p_underLeftWidget -> layout() -> setSpacing(KDialog::spacingHint());
	p_underLeftWidget -> layout() -> setMargin(KDialog::marginHint());
	p_leftWidget -> setStretchFactor(p_underLeftWidget, 1);

	setCentralWidget(p_bigWidget);

	connect(p_consult, SIGNAL(clicked()), this, SLOT(consult()));
	connect(p_askMap, SIGNAL(clicked()), this, SLOT(askMap()));
	connect(p_askCapitalDivisions, SIGNAL(clicked()), this, SLOT(askCapitalDivisions()));
	connect(p_askDivisionCapitals, SIGNAL(clicked()), this, SLOT(askDivisionCapitals()));
	connect(p_askFlagDivisions, SIGNAL(clicked()), this, SLOT(askFlagDivisions()));
	connect(p_askDivisionFlags, SIGNAL(clicked()), this, SLOT(askDivisionFlags()));

	KStdAction::open(this, SLOT(openMap()), actionCollection(), "openMap");
	KStdAction::quit(this, SLOT(close()), actionCollection(), "quit");

	p_zoom = new KToggleAction(i18n("&Zoom"), "viewmag", 0, 0, 0, actionCollection(), "zoom");
	p_zoom -> setEnabled(false);

	p_move = new KToggleAction(i18n("&Move"), "move", 0, 0, 0, actionCollection(), "move");
	p_move -> setEnabled(false);

	new KAction(i18n("Disclaimer"), 0, this, SLOT(disclaimer()), actionCollection(), "disclaimer");

	file = kgeographySettings::self() -> lastMap();

#if KDE_IS_VERSION(3,2,90)
	setupGUI(Keys | ToolBar | Save | Create);
#else
	createGUI();
#endif


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

	show();
}

kgeography::~kgeography()
{
	delete p_askWidget;
	delete p_map;
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
	removeOldAskWidget();
	p_askWidget = new mapAsker(p_bigWidget, p_map, p_underLeftWidget, false);
	p_zoom -> setEnabled(true);
	putAskWidget();
}

void kgeography::askCapitalDivisions()
{
	int i;
	bool ok;
	p_askWidget -> showAnswersMessageBox();
	i = KInputDialog::getInteger(i18n("Number of questions"), i18n("How many questions do you want? (1 to %1)").arg(p_map -> count()), 1, 1, p_map -> count(), 1, &ok);
	if (ok)
	{
		removeOldAskWidget();
		p_askWidget = new capitalDivisionAsker(p_bigWidget, p_map, p_underLeftWidget, i);
		putAskWidget();
	}
	else consult();
}

void kgeography::askDivisionCapitals()
{
	int i;
	bool ok;
	p_askWidget -> showAnswersMessageBox();
	i = KInputDialog::getInteger(i18n("Number of questions"), i18n("How many questions do you want? (1 to %1)").arg(p_map -> count()), 1, 1, p_map -> count(), 1, &ok);
	if (ok)
	{
		removeOldAskWidget();
		p_askWidget = new divisionCapitalAsker(p_bigWidget, p_map, p_underLeftWidget, i);
		putAskWidget();
	}
	else consult();
}

void kgeography::askMap()
{
	int i;
	bool ok;
	p_askWidget -> showAnswersMessageBox();
	i = KInputDialog::getInteger(i18n("Number of questions"), i18n("How many questions do you want? (1 to %1)").arg(p_map -> count()), 1, 1, p_map -> count(), 1, &ok);
	if (ok)
	{
		removeOldAskWidget();
		p_askWidget = new mapAsker(p_bigWidget, p_map, p_underLeftWidget, true, i);
		p_zoom -> setEnabled(true);
		putAskWidget();
	}
	else consult();
}

void kgeography::askFlagDivisions()
{
	int i;
	bool ok;
	p_askWidget -> showAnswersMessageBox();
	i = KInputDialog::getInteger(i18n("Number of questions"), i18n("How many questions do you want? (1 to %1)").arg(p_map -> count()), 1, 1, p_map -> count(), 1, &ok);
	if (ok)
	{
		removeOldAskWidget();
		p_askWidget = new flagDivisionAsker(p_bigWidget, p_map, p_underLeftWidget, i);
		putAskWidget();
	}
	else consult();
}

void kgeography::askDivisionFlags()
{
	int i;
	bool ok;
	p_askWidget -> showAnswersMessageBox();
	i = KInputDialog::getInteger(i18n("Number of questions"), i18n("How many questions do you want? (1 to %1)").arg(p_map -> count()), 1, 1, p_map -> count(), 1, &ok);
	if (ok)
	{
		removeOldAskWidget();
		p_askWidget = new divisionFlagAsker(p_bigWidget, p_map, p_underLeftWidget, i);
		putAskWidget();
	}
	else consult();
}

void kgeography::removeOldAskWidget()
{
	delete p_askWidget;
	p_zoom -> setEnabled(false);
	p_move -> setEnabled(false);
	p_zoom -> setChecked(false);
	p_move -> setChecked(false);

}

void kgeography::putAskWidget()
{
	p_bigWidget -> setStretchFactor(p_askWidget, 1);
	p_askWidget -> show();
	connect(p_askWidget, SIGNAL(setZoomActionChecked(bool)), p_zoom, SLOT(setChecked(bool)));
	connect(p_zoom, SIGNAL(toggled(bool)), p_askWidget, SLOT(setZoom(bool)));
	connect(p_askWidget, SIGNAL(setMoveActionEnabled(bool)), p_move, SLOT(setEnabled(bool)));
	connect(p_askWidget, SIGNAL(setMoveActionChecked(bool)), p_move, SLOT(setChecked(bool)));
	connect(p_move, SIGNAL(toggled(bool)), p_askWidget, SLOT(setMovement(bool)));
}

void kgeography::setMap(map *m)
{
	kgeographySettings *set = kgeographySettings::self();
	set -> setLastMap(m -> getFile());
	set -> writeConfig();
	p_map = m;
	p_currentMap -> setText(i18n("Current map:\n%1").arg(p_map -> getName()));
	p_consult -> setEnabled(true);
	p_askMap -> setEnabled(true);
	p_askFlagDivisions -> setEnabled(m -> hasAllFlags());
	p_askDivisionFlags -> setEnabled(m -> hasAllFlags());
	p_askCapitalDivisions -> setEnabled(true);
	p_askDivisionCapitals -> setEnabled(true);
	consult();
}

void kgeography::disclaimer()
{
	KMessageBox::information(this, i18n("Maps, flags, translations, etc. are as accurate as their respective authors could achieve, but KGeography should not be taken as a authoritative source."), i18n("Disclaimer"));
}

#include "kgeography.moc"
