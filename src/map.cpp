/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <stdlib.h> // for RAND_MAX

#include <klocale.h>
#include <krandom.h>

#include <qfile.h>
#include <qfileinfo.h>

#include "division.h"
#include "map.h"

KGmap::KGmap()
{
	p_hasAllFlags = true;
}

KGmap::~KGmap()
{
	qDeleteAll(p_divisions);
}

bool KGmap::addDivision(division *d)
{
	bool b;
	if ( ( getDivision(d -> getName()) == NULL && getDivision(d -> getRGB()) == NULL ) || 
	     ( getDivision(d -> getName()) != NULL && getDivision(d -> getRGB()) == NULL && !d->canAsk(false) && !getDivision(d -> getName())->canAsk(false) ) )
	{
		p_divisions.append(d);
		b = true;
		if (d -> canAsk(false)) p_hasAllFlags = p_hasAllFlags && !d -> getFlagFile().isNull();
	}
	else b = false;
	return b;
}

void KGmap::setFile(const QString &s)
{
	p_file = s;
}

bool KGmap::setMapFile(const QString &s)
{
	p_mapFile = s;
	return QFile::exists(s);
}

void KGmap::setName(const QString &s)
{
	p_name = s;
}

uint KGmap::count(bool clickDivisionMode) const
{
	uint count = 0;
	foreach(const division *d, p_divisions)
	{
		if (d->canAsk(clickDivisionMode)) count++;
	}
	return count;
}

bool KGmap::hasAllFlags() const
{
	return p_hasAllFlags;
}

QString KGmap::getDivisionFlagFile(const QString &s) const
{
	return getDivision(s) -> getFlagFile();
}

QString KGmap::getDivisionCapital(const QString &s) const
{
	return getDivision(s) -> getCapital();
}

QString KGmap::getFile() const
{
	return p_file;
}

QString KGmap::getFileName() const
{
	QFileInfo fi(p_file);
	return fi.fileName();
}

QString KGmap::getMapFile() const
{
	return p_mapFile;
}

QString KGmap::getName() const
{
	return p_name;
}

QString KGmap::getRandomDivision(bool clickDivisionMode) const
{
	QList<division*> aux;
	int i = (int)((float)p_divisions.size() * KRandom::random() / (RAND_MAX + 1.0));
	if (!p_divisions[i] -> canAsk(clickDivisionMode)) return getRandomDivision(clickDivisionMode);
	else return p_divisions[i] -> getName();
}

QString KGmap::getWhatIs(QRgb c, bool all) const
{
	// this is only asked from mapasker.cpp hence the true in canAsk
	division *d = getDivision(c);
	if (d == NULL) return "nothing";
	else
	{
		if (all) return d -> getName();
		else if (d -> canAsk(true)) return d -> getName();
		else return "";
	}
}

QColor KGmap::getColor(const QString &s) const
{
	return QColor(getDivision(s)->getRGB());
}

const QList<division*> KGmap::getIgnoredDivisions(bool clickDivisionMode) const
{
	QList<division*> ignoredDivisions;
	foreach (division* div, p_divisions)
		if (!div->canAsk(clickDivisionMode)) 
			ignoredDivisions << div;
	return ignoredDivisions;
}

division *KGmap::getDivision(const QString &divisionName) const
{
	foreach (division* div, p_divisions)
	{
		if (div->getName() == divisionName) return div;
	}
	return NULL;
}

division *KGmap::getDivision(QRgb c) const
{
	foreach (division* div, p_divisions)
	{
		if (div->getRGB() == c) return div;
	}
	return NULL;
}
