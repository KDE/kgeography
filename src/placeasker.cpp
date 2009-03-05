/***************************************************************************
 *   Copyright (C) 2004-2007 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *   Copyright (C) 2006 by Isaac Clerencia                                 *
 *   isaac@warp.es                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "placeasker.h"

#include <cmath>
#include <math.h>

#include <klocale.h>
#include <kmessagebox.h>

#include <qlabel.h>
#include <qlayout.h>
#include <qscrollbar.h>
#include <qstring.h>

#include "map.h"
#include "placemapwidget.h"

placeAsker::placeAsker(QWidget *parent, KGmap *m, QWidget *w, uint count) : askWidget(parent, m, w, count, true), p_firstShow(true), p_currentDivisionImage(0)
{
	QVBoxLayout *lay = new QVBoxLayout(this);
	lay -> setMargin(0);
	lay -> setSpacing(0);

	p_mapImage = new QImage(p_map->getMapFile());
	p_mapWidget = new placeMapWidget(this);
	lay -> addWidget(p_mapWidget);
	
	connect(p_mapWidget, SIGNAL(clicked(QRgb, const QPoint&, const QPointF&)), this, SLOT(handleMapClick(QRgb, const QPoint&, const QPointF&)));
	connect(p_mapWidget, SIGNAL(setMoveActionChecked(bool)), this, SIGNAL(setMoveActionChecked(bool)));
	connect(p_mapWidget, SIGNAL(setZoomActionChecked(bool)), this, SIGNAL(setZoomActionChecked(bool)));
	connect(p_mapWidget, SIGNAL(setMoveActionEnabled(bool)), this, SIGNAL(setMoveActionEnabled(bool)));
	
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

placeAsker::~placeAsker()
{
	delete p_next;
	delete p_fill;
	delete p_mapImage;
}

bool placeAsker::isAsker() const
{
	return p_answers;
}

void placeAsker::mousePressEvent(QMouseEvent*)
{
}

void placeAsker::setMovement(bool b)
{
	p_mapWidget -> setMapMove(b);
}

void placeAsker::setZoom(bool b)
{
	askWidget::setZoom(b);
	p_mapWidget -> setMapZoom(b);
}

void placeAsker::setOriginalZoom()
{
	p_mapWidget -> setGameImage();
}

void placeAsker::setAutomaticZoom(bool automaticZoom)
{
	p_mapWidget -> setAutomaticZoom(automaticZoom);
}

void placeAsker::handleMapClick(QRgb c, const QPoint & , const QPointF &mapPoint)
{
	QString aux, cap;
	aux = p_map -> getWhatIs(c, false);
	if (aux == "nothing")
	{
		KMessageBox::error(this, i18nc("@info", "You have found a bug in a map."
					       " Please contact the author and tell the %1 map"
					       " has nothing associated to color %2,%3,%4.",
					       p_map -> getFile(), qRed(c), qGreen(c), qBlue(c)));
	}
	else
	{
		p_mapWidget->placeDivision(p_currentDivisionImage, p_currentDivisionRect);
		// the image is no longer needed
		delete p_currentDivisionImage;

		p_currentAnswer.setAnswer(QColor(c));
		double distX = p_currentDivisionRect.center().x() - mapPoint.x();
		double distY = p_currentDivisionRect.center().y() - mapPoint.y();
		double distance = sqrt((double)distX * distX + distY * distY); 
		// TODO: See if 5 is big enough or we should take into account the division size or what
		questionAnswered(distance < 5.0);
		nextQuestion();
	}
}

void placeAsker::nextQuestionHook(const QString &division)
{
	QString divisionName = i18nc(p_map -> getFileName().toUtf8(), division.toUtf8());
	p_next -> setText(i18nc("@info:status", "Please place in the map:<nl/>%1", divisionName));
	p_next -> show();
	p_currentAnswer.setQuestion(i18nc("@item:intable column Question, %1 is region name", "%1", i18nc(p_map -> getFileName().toUtf8(), division.toUtf8())));
	p_currentAnswer.setCorrectAnswer(p_map -> getColor(division));
	setCurrentDivision(division);
	p_mapWidget->setCurrentDivisionImage(p_currentDivisionImage);
}

QString placeAsker::getQuestionHook() const
{
	QString divisionType = i18nc(p_map -> getFileName().toUtf8(), p_map->getDivisionsString().toUtf8());
	return i18nc("@title", "Place %1 in Map", divisionType);
}

void placeAsker::showEvent(QShowEvent *)
{
	if (p_firstShow)
	{
		p_mapWidget -> init(p_map, p_mapImage);
		p_firstShow = false;
	}
}

QSize placeAsker::mapSize() const
{
	return p_mapWidget -> mapSize();
}

void placeAsker::setCurrentDivision(const QString& division)
{
	int width = p_mapImage->width();
	int height = p_mapImage->height();

	int minX = width;
	int maxX = 0;
	int minY = height;
	int maxY = 0;

	QRgb divColor = p_map->getColor(division).rgb();

	//first iteration, detect size required by the image
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (p_mapImage->pixel(x,y) == divColor)
			{
				if (x < minX) minX = x;
				if (x > maxX) maxX = x;
				if (y < minY) minY = y;
				if (y > maxY) maxY = y;
			}
		}
	}

	p_currentDivisionImage = new QImage(maxX - minX, maxY - minY, QImage::Format_ARGB32);
	p_currentDivisionRect.setCoords(minX, minY, maxX, maxY);
	QColor transparent(0,0,0,0);
	p_currentDivisionImage->fill(transparent.rgba());

	//second iteration, copy the color to the new image
	for (int x = minX; x < maxX; x++)
	{
		for (int y = minY; y < maxY; y++)
		{
			if (p_mapImage->pixel(x,y) == divColor)
				p_currentDivisionImage->setPixel(x - minX, y - minY, p_mapImage->pixel(x,y));
		}
	}
}

#include "placeasker.moc"
