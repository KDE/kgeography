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
#include <kmessagebox.h>

#include <qlayout.h>

#include "infowidget.h"
#include "map.h"
#include "mapasker.h"
#include "mapwidget.h"

mapAsker::mapAsker(QWidget *parent, map *m, bool asker, uint count) : askWidget(parent, m, count), p_popupManager(this), p_asker(asker)
{
	QVBoxLayout *lay = new QVBoxLayout(this);
	
	p_mapWidget = new mapWidget(this);
	p_infoWidget = new infoWidget(this);
	lay -> addWidget(p_mapWidget);
	lay -> addWidget(p_infoWidget);
	
	p_shouldClearPopup = false;
	
	connect(p_mapWidget, SIGNAL(clicked(QRgb, const QPoint&)), this, SLOT(handleMapClick(QRgb, const QPoint&)));
	 
	p_infoWidget -> setQuestionMode(asker);
	p_mapWidget -> setMapImage(p_map -> getMapFile());
	p_infoWidget -> setName(p_map -> getName());
	
	if (asker) nextDivision();
}

void mapAsker::mousePressEvent(QMouseEvent*)
{
	p_popupManager.clear();
}


void mapAsker::goToMenu()
{
	if (p_asker) KMessageBox::information(this, i18n("You have answered correctly %1 of the %2 questions about this map").arg(p_infoWidget -> getCorrect()). arg(p_infoWidget -> getTotal()));
}

void mapAsker::handleMapClick(QRgb c, const QPoint &p)
{
	QString aux;
	aux = p_map -> getWhatIs(c, !p_asker);
	if (aux == "nothing") KMessageBox::error(this, i18n("You have found a bug in a map. Please contact the author and tell the %1 map has nothing associated to color %2,%3,%4.").arg(p_map -> getName()).arg(qRed(c)).arg(qGreen(c)).arg(qBlue(c)));
	else if (p_shouldClearPopup)
	{
		p_popupManager.clear();
		p_shouldClearPopup = false;
	}
	else if (!p_asker)
	{
		p_popupManager.show(aux, p);
	}
	else if (aux != "")
	{
		p_infoWidget -> addResult(aux == p_asked.last());
		nextDivision();
	}
}

void mapAsker::nextDivision()
{
	QString aux;
	if (p_asked.count() < p_count)
	{
		aux = p_map -> getRandomDivision();
		while (p_asked.find(aux) != p_asked.end()) aux = p_map -> getRandomDivision();
		p_asked << aux;
		p_infoWidget -> setNext(i18n("You must click on %1").arg(aux));
	}
	else
	{
		p_popupManager.show(i18n("You have answered correctly %1 of the %2 questions about this map").arg(p_infoWidget -> getCorrect()). arg(p_infoWidget -> getTotal()));
		p_shouldClearPopup = true;
		p_asker = false;
	}
}

#include "mapasker.moc"
