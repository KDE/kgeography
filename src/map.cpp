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

#include <kapp.h>
#include <klocale.h>

#include <qfile.h>

#include "division.h"
#include "map.h"

map::map()
{
	p_count = 0;
	p_hasAllFlags = true;
}

map::~map()
{
	QMap<QRgb, division*>::iterator it;
	it = p_colorMap.begin();
	while (it != p_colorMap.end())
	{
		delete it.data();
		p_colorMap.remove(it);
		it = p_colorMap.begin();
	}
}

bool map::addDivision(division *c)
{
	bool b;
	if (p_nameMap.find(c -> getName()) == p_nameMap.end() && 
		p_colorMap.find(c -> getRGB()) == p_colorMap.end())
	{
		if (c -> canAsk()) p_count++;
		p_colorMap.insert(c -> getRGB(), c);
		p_nameMap.insert(c -> getName(), c);
		b = true;
		if (c -> canAsk()) p_hasAllFlags = p_hasAllFlags && !c -> getFlagFile().isNull();
	}
	else b = false;
	return b;
}

void map::setFile(const QString &s)
{
	p_file = s;
}

bool map::setMapFile(const QString &s)
{
	p_mapFile = s;
	return QFile::exists(s);
}

void map::setName(const QString &s)
{
	p_name = s;
}

uint map::count() const
{
	return p_count;
}

bool map::hasAllFlags() const
{
	return p_hasAllFlags;
}

QString map::getDivisionFlagFile(const QString &s) const
{
	return p_nameMap[s] -> getFlagFile();
}

QString map::getFile() const
{
	return p_file;
}

QString map::getMapFile() const
{
	return p_mapFile;
}

QString map::getName() const
{
	return i18n(p_name);
}

QString map::getRandomDivision() const
{
	QValueList<division*> aux;
	int i = (int)((float)p_nameMap.size() * kapp -> random() / (RAND_MAX + 1.0));
	aux = p_nameMap.values();
	if (!aux[i] -> canAsk()) return getRandomDivision();
	else return aux[i] -> getName();
}

QString map::getWhatIs(QRgb c, bool all) const
{
	QMap<QRgb, division*>::const_iterator it;
	it = p_colorMap.find(c);
	if (it == p_colorMap.end()) return "nothing";
	else 
	{
		if (all) return it.data() -> getName();
		else if (it.data() -> canAsk()) return it.data() -> getName();
		else return "";
	}
}
