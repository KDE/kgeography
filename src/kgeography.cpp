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

#include "kgeography.h"
#include "mapchooser.h"
#include "mapparser.h"
#include "mapwidget.h"
#include "map.h"

kgeography::kgeography() : KMainWindow(), p_popupManager(this)
{
	p_map = 0;
	p_mapWidget = new mapWidget(this);
	setCentralWidget(p_mapWidget);
	
	KStdAction::open(this, SLOT(openMap()), actionCollection(), "openMap");
	KStdAction::quit(this, SLOT(close()), actionCollection(), "quit");
	p_consult = new KToggleAction(i18n("&Consult mode"), 0, this, SLOT(consult()), actionCollection(), "consult");
	p_question = new KToggleAction(i18n("&Question mode"), 0, this, SLOT(question()), actionCollection(), "question");
	p_consult -> setChecked(true);
	connect(p_mapWidget, SIGNAL(clicked(QRgb, const QPoint&)), this, SLOT(handleMapClick(QRgb, const QPoint&)));
	createGUI();
	show();
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
		p_map = mp.getMap();
		p_mapWidget -> setMapImage(p_map -> getFile());
		consult();
	}
}

void kgeography::consult()
{
	if (p_consult -> isChecked())
	{
		p_question -> setChecked(false);
	}
	else p_consult -> setChecked(true);
}

void kgeography::question()
{
	if (p_question -> isChecked())
	{
		p_consult -> setChecked(false);
		if (p_map)
		{
			p_asked.clear();
			p_popupManager.clear();
			nextDivision();
		}
	}
	else p_question -> setChecked(true);
}

void kgeography::handleMapClick(QRgb c, const QPoint &p)
{
	QString aux;
	aux = p_map -> getWhatIs(c, p_consult -> isChecked());
	if (aux == "nothing") KMessageBox::error(this, i18n("You have found a bug in a map. Please contact the author and tell the %1 map has nothing associated to color %2,%3,%4.").arg(p_map -> getName()).arg(qRed(c)).arg(qGreen(c)).arg(qBlue(c)));
	else if (p_consult -> isChecked())
	{
		p_popupManager.show(aux, p);
	}
	else
	{
		if (aux != "")
		{
			if (aux == p_asked.last())
			{
				KMessageBox::information(this, i18n("Correct"));
			}
			else
				{
				KMessageBox::information(this, i18n("You have failed"));
			}
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
		KMessageBox::information(this, i18n("You must click on %1").arg(aux));
	}
}

#include "kgeography.moc"
