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
#include <kmessagebox.h>
#include <kstdaction.h>

#include <qsizepolicy.h> 
#include <qvbox.h>

#include "flagdivisionasker.h"
#include "kgeography.h"
#include "settings.h"
#include "mapchooser.h"
#include "infowidget.h"
#include "mapparser.h"
#include "mapwidget.h"
#include "map.h"

kgeography::kgeography() : KMainWindow(), p_popupManager(this)
{
	QString file;
	
	p_map = 0;
	p_shouldClearPopup = false;
	
	p_holder = new QVBox(this);
	p_mapWidget = new mapWidget(p_holder);
	p_infoWidget = new infoWidget(p_holder);
	setCentralWidget(p_holder);
	
	KStdAction::open(this, SLOT(openMap()), actionCollection(), "openMap");
	KStdAction::quit(this, SLOT(close()), actionCollection(), "quit");
	p_consult = new KToggleAction(i18n("&Consult mode"), 0, this, SLOT(consult()), actionCollection(), "consult");
	p_questionMap = new KToggleAction(i18n("&Map question mode"), 0, this, SLOT(questionMap()), actionCollection(), "questionMap");
	p_questionFlagDivision = new KToggleAction(i18n("&Flag to division question mode"), 0, this, SLOT(questionFlagDivision()), actionCollection(), "questionFlagDivision");
	p_consult -> setChecked(true);
	connect(p_mapWidget, SIGNAL(clicked(QRgb, const QPoint&)), this, SLOT(handleMapClick(QRgb, const QPoint&)));
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
			KMessageBox::error(this, i18n("Could not open last saved map. Error parsing %1: %2").arg(file).arg(reader.getError()));
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

void kgeography::openMap()
{
	mapChooser mp(this);
	if (mp.exec() == mapChooser::Accepted)
	{
		delete p_map;
		p_popupManager.clear();
		setMap(mp.getMap());
	}
}

void kgeography::consult()
{
	p_consult -> setChecked(true);
	p_questionMap -> setChecked(false);
	p_questionFlagDivision -> setChecked(false);
	
	if (!p_mapWidget)
	{
		delete p_flagDivisionAsker;
		p_flagDivisionAsker = 0;
		p_mapWidget = new mapWidget(p_holder);
		p_infoWidget = new infoWidget(p_holder);
		p_mapWidget -> show();
		p_infoWidget -> show();
	}
	
	p_infoWidget -> setQuestionMode(false);
}

void kgeography::questionMap()
{
	if (p_map && p_questionMap -> isChecked())
	{
		// p_questionMap -> isChecked when it was not checked and the user clicks on it
		// if it was already checked the program reaches here with p_questionMap unchecked
		
		if (!p_mapWidget)
		{
			delete p_flagDivisionAsker;
			p_flagDivisionAsker = 0;
			p_mapWidget = new mapWidget(p_holder);
			p_infoWidget = new infoWidget(p_holder);
			p_mapWidget -> show();
			p_infoWidget -> show();
		}
		
		p_infoWidget -> setQuestionMode(true);
		p_asked.clear();
		p_popupManager.clear();
		nextDivision();
	}
	p_consult -> setChecked(false);
	p_questionMap -> setChecked(true);
	p_questionFlagDivision -> setChecked(false);
}

void kgeography::questionFlagDivision()
{
	if (p_map && p_questionFlagDivision -> isChecked())
	{
		// p_questionMapFlagDivion -> isChecked when it was not checked and the user clicks on it
		// if it was already checked the program reaches here with p_questionMapFlagDivion unchecked
		p_asked.clear();
		p_popupManager.clear();
		
		delete p_mapWidget;
		delete p_infoWidget;
		p_mapWidget = 0;
		p_infoWidget = 0;
		
		p_flagDivisionAsker = new flagDivisionAsker(p_holder, p_map);
		p_flagDivisionAsker->show();
	}
	p_consult -> setChecked(false);
	p_questionMap -> setChecked(false);
	p_questionFlagDivision -> setChecked(true);
}

void kgeography::handleMapClick(QRgb c, const QPoint &p)
{
	QString aux;
	aux = p_map -> getWhatIs(c, p_consult -> isChecked());
	if (aux == "nothing") KMessageBox::error(this, i18n("You have found a bug in a map. Please contact the author and tell the %1 map has nothing associated to color %2,%3,%4.").arg(p_map -> getName()).arg(qRed(c)).arg(qGreen(c)).arg(qBlue(c)));
	else if (p_shouldClearPopup)
	{
		p_popupManager.clear();
		p_shouldClearPopup = false;
	}
	else if (p_consult -> isChecked())
	{
		p_popupManager.show(aux, p);
	}
	else
	{
		if (aux != "")
		{
			p_infoWidget -> addResult(aux == p_asked.last());
			nextDivision();
		}
	}
}

void kgeography::mousePressEvent(QMouseEvent*)
{
	p_popupManager.clear();
}

void kgeography::nextDivision()
{
	QString aux;
	if (p_asked.count() < p_map -> count())
	{
		aux = p_map -> getRandomDivision();
		while (p_asked.find(aux) != p_asked.end()) aux = p_map -> getRandomDivision();
		p_asked << aux;
		p_infoWidget -> setNext(i18n("You must click on %1").arg(aux));
	}
	else
	{
		p_popupManager.show(i18n("You have answered correctly %1 of the %2 questions about this map").arg(p_infoWidget -> getCorrect()). arg(p_map -> count()));
		p_shouldClearPopup = true;
		consult();
	}
}

void kgeography::setMap(map *m)
{
	kgeographySettings *set = kgeographySettings::self();
	set -> setLastMap(m -> getFile());
	set -> writeConfig();
	p_map = m;
	p_mapWidget -> setMapImage(p_map -> getMapFile());
	p_infoWidget -> setName(p_map -> getName());
	consult();
}

#include "kgeography.moc"
