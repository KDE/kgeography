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

#include <qlabel.h>
#include <qlayout.h>

#include "map.h"
#include "mapasker.h"
#include "mapwidget.h"

mapAsker::mapAsker(QWidget *parent, map *m, QWidget *w, bool asker, uint count) : askWidget(parent, m, w, count, asker), p_popupManager(this), p_asker(asker)
{
	QVBoxLayout *lay = new QVBoxLayout(this);
	
	p_mapWidget = new mapWidget(this, p_map -> getMapFile());
	lay -> addWidget(p_mapWidget);
	
	p_shouldClearPopup = false;
	
	connect(p_mapWidget, SIGNAL(clicked(QRgb, const QPoint&)), this, SLOT(handleMapClick(QRgb, const QPoint&)));
	connect(p_mapWidget, SIGNAL(setMoveActionChecked(bool)), this, SIGNAL(setMoveActionChecked(bool)));
	connect(p_mapWidget, SIGNAL(setZoomActionChecked(bool)), this, SIGNAL(setZoomActionChecked(bool)));
	connect(p_mapWidget, SIGNAL(setMoveActionEnabled(bool)), this, SIGNAL(setMoveActionEnabled(bool)));
	 
	if (asker)
	{
		p_next = new QLabel(w);
		p_next -> setAlignment(AlignTop | AlignHCenter);
		p_next -> show();
		nextQuestion();
	}
}

void mapAsker::clean()
{
	p_shouldClearPopup = true;
	p_asker = false;
	p_next -> setText("");
}

void mapAsker::mousePressEvent(QMouseEvent*)
{
	p_popupManager.clear();
}

void mapAsker::setMovement(bool b)
{
	p_mapWidget -> setMapMove(b);
	p_popupManager.clear();
}

void mapAsker::setZoom(bool b)
{
	p_mapWidget -> setMapZoom(b);
	p_popupManager.clear();
}

void mapAsker::handleMapClick(QRgb c, const QPoint &p)
{
	QString aux, cap;
	aux = p_map -> getWhatIs(c, !p_asker);
	if (aux == "nothing") KMessageBox::error(this, i18n("You have found a bug in a map. Please contact the author and tell the %1 map has nothing associated to color %2,%3,%4.").arg(p_map -> getName()).arg(qRed(c)).arg(qGreen(c)).arg(qBlue(c)));
	else if (p_shouldClearPopup)
	{
		p_popupManager.clear();
		p_shouldClearPopup = false;
	}
	else if (!p_asker)
	{
		cap = p_map -> getDivisionCapital(aux);
		if (cap != "") aux = i18n("%1 (%2)").arg(aux).arg(cap);
		p_popupManager.show(aux, p);
	}
	else if (aux != "")
	{
		questionAnswered(aux == lastDivisionAsked());
		nextQuestion();
	}
}

void mapAsker::nextQuestionHook(QString division)
{
	p_next -> setText(i18n("You must click on %1").arg(division));
}

#include "mapasker.moc"
