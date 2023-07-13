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


#include <QFile>
#include <QFileInfo>
#include <QRandomGenerator>

#include <KLazyLocalizedString>

KGmap::KGmap()
{
	p_capitalToDivisionPattern = kli18nc("@title:group", "%1 is the capital of...").untranslatedText();
	p_divisionToCapitalPattern = kli18nc("@title:group", "The capital of %1 is...").untranslatedText();
}

KGmap::~KGmap()
{
	qDeleteAll(p_divisions);
}

bool KGmap::addDivision(division *d)
{
	bool b;
	const division *divByName = getDivision(d -> getUntranslatedName());
	const division *divByColor = getDivision(d -> getRGB());
	if ( ( divByName == nullptr && divByColor == nullptr ) || 
	     ( divByName != nullptr && divByColor == nullptr && !d->canAsk(division::eClick) && !divByName->canAsk(division::eClick) ) )
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

KLocalizedString KGmap::getCapitalToDivisionQuestionPattern() const
{
	return ki18nc("@title:group", p_capitalToDivisionPattern.toUtf8());
}

KLocalizedString KGmap::getDivisionToCapitalQuestionPattern() const
{
	return ki18nc("@title:group", p_divisionToCapitalPattern.toUtf8());
}

QString KGmap::getCapitalToDivisionTitle() const
{
	if (p_capitalToDivisionTitle.isNull()) return i18n("%1 by Capital", getDivisionsString());
	else return p_capitalToDivisionTitle;
}

QString KGmap::getDivisionToCapitalTitle() const
{
	if (p_divisionToCapitalTitle.isNull()) return i18n("Capitals of %1", getDivisionsString());
	else return p_divisionToCapitalTitle;
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

division *KGmap::getRandomDivision(division::askMode am) const
{
	QList<division*> aux;
	const int i = QRandomGenerator::global()->bounded(p_divisions.size());
	if (!p_divisions[i] -> canAsk(am)) return getRandomDivision(am);
	else return p_divisions[i];
}

QString KGmap::getWhatIs(QRgb c, bool all) const
{
	// this is only asked from mapasker.cpp hence the division::eClick in canAsk
	division *d = getDivision(c);
	if (d == nullptr) return QStringLiteral("nothing");
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
	return nullptr;
}

division *KGmap::getDivision(QRgb c) const
{
	foreach (division* div, p_divisions)
	{
		if (div->getRGB() == c) return div;
	}
	return nullptr;
}
