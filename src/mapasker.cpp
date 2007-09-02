/***************************************************************************
 *   Copyright (C) 2004-2007 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "mapasker.h"

#include <klocale.h>
#include <kmessagebox.h>

#include <qlabel.h>
#include <qlayout.h>
#include <qscrollbar.h>
#include <qstring.h>

#include "map.h"
#include "mapwidget.h"

mapAsker::mapAsker(QWidget *parent, KGmap *m, QWidget *w, bool asker, uint count) : askWidget(parent, m, w, count, asker), p_asker(asker), p_firstShow(true)
{
	QVBoxLayout *lay = new QVBoxLayout(this);
	lay -> setMargin(0);
	lay -> setSpacing(0);

	p_mapWidget = new mapWidget(this);
	p_popupManager.setWidget(p_mapWidget);
	lay -> addWidget(p_mapWidget);

	p_shouldClearPopup = false;

	connect(p_mapWidget, SIGNAL(clicked(QRgb, const QPoint&)), this, SLOT(handleMapClick(QRgb, const QPoint&)));
	connect(p_mapWidget, SIGNAL(setMoveActionChecked(bool)), this, SIGNAL(setMoveActionChecked(bool)));
	connect(p_mapWidget, SIGNAL(setZoomActionChecked(bool)), this, SIGNAL(setZoomActionChecked(bool)));
	connect(p_mapWidget, SIGNAL(setMoveActionEnabled(bool)), this, SIGNAL(setMoveActionEnabled(bool)));

	if (asker)
	{
		QVBoxLayout *vbl = static_cast<QVBoxLayout*>(w -> layout());
		p_next = new QLabel(w);
		p_next -> setAlignment(Qt::AlignTop | Qt::AlignHCenter);
		p_next -> setWordWrap(true);
		p_fill = new QWidget(w);
		p_fill -> show();
		vbl -> addWidget(p_next);
		vbl -> addWidget(p_fill, 1);
		nextQuestion();
	}
	else
	{
		p_next = 0;
		p_fill = 0;
	}
}

mapAsker::~mapAsker()
{
	delete p_next;
	delete p_fill;
}

bool mapAsker::isAsker() const
{
	return p_answers;
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

void mapAsker::setOriginalZoom()
{
	p_mapWidget -> setOriginalImage();
	p_popupManager.clear();
}

void mapAsker::setAutomaticZoom(bool automaticZoom)
{
	p_mapWidget -> setAutomaticZoom(automaticZoom);
	p_popupManager.clear();
}

void mapAsker::handleMapClick(QRgb c, const QPoint &p)
{
	QString aux, cap;
	aux = p_map -> getWhatIs(c, !p_asker);
	if (aux == "nothing") KMessageBox::error(this, i18nc("@info", "You have found a bug in a map. Please contact the author and tell the %1 map has nothing associated to color %2,%3,%4.", p_map -> getFile(), qRed(c), qGreen(c), qBlue(c)));
	else if (p_shouldClearPopup)
	{
		p_popupManager.clear();
		p_shouldClearPopup = false;
	}
	else if (!p_asker)
	{
		QString flagFile = p_map -> getDivisionFlagFile(aux);
		cap = p_map -> getDivisionCapital(aux);
		if (!cap.isEmpty()) cap = i18nc("@item Capital name in map popup", "%1", i18nc(p_map -> getFileName().toUtf8(), cap.toUtf8()));

		aux = i18nc("@item Region name in map popup", "%1", i18nc(p_map -> getFileName().toUtf8(), aux.toUtf8()));

		if (!flagFile.isEmpty()) p_popupManager.show(aux, cap, p, flagFile);
		else if (!cap.isEmpty()) p_popupManager.show(aux, cap, p);
		else p_popupManager.show(aux, p);
	}
	else if (!aux.isEmpty())
	{
		p_currentAnswer.setAnswer(QColor(c));
		questionAnswered(aux == lastDivisionAsked());
		nextQuestion();
	}
}

void mapAsker::nextQuestionHook(const QString &division)
{
	QString divisionName = i18nc(p_map -> getFileName().toUtf8(), division.toUtf8());
	p_next -> setText(i18nc("@info:status", "<para>Please click on:<br/>%1</para>", divisionName));
	p_currentAnswer.setQuestion(i18nc("@item:intable column Question, %1 is region name", "%1", i18nc(p_map -> getFileName().toUtf8(), division.toUtf8())));
	p_next -> show();
	p_currentAnswer.setCorrectAnswer(p_map -> getColor(division));
}

QString mapAsker::getQuestionHook() const
{
	return i18nc("@title", "Division in Map");
}

void mapAsker::showEvent(QShowEvent *)
{
	if (p_firstShow)
	{
		p_mapWidget -> init(p_map -> getMapFile());
		p_firstShow = false;
	}
}

QSize mapAsker::mapSize() const
{
	return p_mapWidget -> mapSize();
}

#include "mapasker.moc"
