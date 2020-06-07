/***************************************************************************
 *   Copyright (C) 2004-2007 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "map.h"

#include <stdlib.h> // for RAND_MAX

#include <klocalizedstring.h>
#include <krandom.h>

#include <qfile.h>
#include <qfileinfo.h>

#include "division.h"

KGmap::KGmap()
{
	p_capitalToDivisionPattern = I18N_NOOP2("@title:group", "%1 is the capital of...");
	p_divisionToCapitalPattern = I18N_NOOP2("@title:group", "The capital of %1 is...");
}

KGmap::~KGmap()
{
	qDeleteAll(p_divisions);
}

bool KGmap::addDivision(division *d)
{
	bool b;
	if ( ( getDivision(d -> getUntranslatedName()) == NULL && getDivision(d -> getRGB()) == NULL ) || 
	     ( getDivision(d -> getUntranslatedName()) != NULL && getDivision(d -> getRGB()) == NULL && !d->canAsk(division::eClick) && !getDivision(d -> getUntranslatedName())->canAsk(division::eClick) ) )
	{
		p_divisions.append(d);
		b = true;
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

void KGmap::setDivisionsString(const QString &s)
{
	p_divisionsString = s;
}

void KGmap::setAuthor(const QString &s)
{
	p_author = s;
}

void KGmap::setCapitalToDivisionQuestionPattern(const QString &str)
{
	p_capitalToDivisionPattern = str;
}

void KGmap::setDivisionToCapitalQuestionPattern(const QString &str)
{
	p_divisionToCapitalPattern = str;
}

void KGmap::setCapitalToDivisionTitle(const QString &str)
{
	p_capitalToDivisionTitle = str;
}

void KGmap::setDivisionToCapitalTitle(const QString &str)
{
	p_divisionToCapitalTitle = str;
}

uint KGmap::count(division::askMode am) const
{
	uint count = 0;
	foreach(const division *d, p_divisions)
	{
		if (d->canAsk(am)) count++;
	}
	return count;
}

QString KGmap::getDivisionFlagFile(const QString &s) const
{
	return getDivision(s) -> getFlagFile();
}

QString KGmap::getDivisionCapital(const QString &s) const
{
	return getDivision(s) -> getCapital();
}

QStringList KGmap::getDivisionFalseCapitals(const QString &divisionName) const
{
	return getDivision(divisionName) -> getFalseCapitals();
}

KLocalizedString KGmap::getCapitalToDivisionQuestionPattern() const
{
	return ki18nc("@title:group", p_capitalToDivisionPattern.toUtf8());
}

QString KGmap::getDivisionToCapitalQuestionPattern() const
{
	return p_divisionToCapitalPattern;
}

QString KGmap::getCapitalToDivisionTitle() const
{
	return p_capitalToDivisionTitle;
}

QString KGmap::getDivisionToCapitalTitle() const
{
	return p_divisionToCapitalTitle;
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

QString KGmap::getDivisionsString() const
{
	if (p_divisionsString.isNull()) return i18n("Regions");
	else return p_divisionsString;
}

QString KGmap::getRandomDivision(division::askMode am) const
{
	QList<division*> aux;
	int i = (int)((float)p_divisions.size() * KRandom::random() / (RAND_MAX + 1.0));
	if (!p_divisions[i] -> canAsk(am)) return getRandomDivision(am);
	else return p_divisions[i] -> getUntranslatedName();
}

QString KGmap::getWhatIs(QRgb c, bool all) const
{
	// this is only asked from mapasker.cpp hence the division::eClick in canAsk
	division *d = getDivision(c);
	if (d == NULL) return QStringLiteral("nothing");
	else
	{
		if (all) return d -> getUntranslatedName();
		else if (d -> canAsk(division::eClick)) return d -> getUntranslatedName();
		else return QLatin1String("");
	}
}

QString KGmap::getAuthor() const
{
	return p_author;
}

QColor KGmap::getColor(const QString &s) const
{
	return QColor(getDivision(s)->getRGB());
}

QList<const division*> KGmap::getIgnoredDivisions(division::askMode am) const
{
	QList<const division*> ignoredDivisions;
	foreach (division* div, p_divisions)
		if (!div->canAsk(am))
			ignoredDivisions << div;
	return ignoredDivisions;
}

QList<const division*> KGmap::getAllDivisionsOrdered() const
{
	QList<const division*> constDivisions;
	foreach (const division* div, p_divisions)
		constDivisions << div;
	return constDivisions;
}

division *KGmap::getDivision(const QString &divisionName) const
{
	foreach (division* div, p_divisions)
	{
		if (div->getUntranslatedName() == divisionName) return div;
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

bool KGmap::getDivisionCanAsk(const QString &div, division::askMode am) const
{
	return getDivision(div) -> canAsk(am);
}
