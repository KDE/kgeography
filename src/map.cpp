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

#include <kapplication.h>
#include <klocale.h>

#include <qfile.h>
#include <qfileinfo.h>

#include "division.h"
#include "map.h"

KGmap::KGmap()
{
	p_count = 0;
	p_hasAllFlags = true;
}

KGmap::~KGmap()
{
	qDeleteAll(p_colorMap.values());
}

bool KGmap::addDivision(division *c)
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

uint KGmap::count() const
{
	return p_count;
}

bool KGmap::hasAllFlags() const
{
	return p_hasAllFlags;
}

QString KGmap::getDivisionFlagFile(const QString &s) const
{
	return p_nameMap[s] -> getFlagFile();
}

QString KGmap::getDivisionCapital(const QString &s) const
{
	return p_nameMap[s] -> getCapital();
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

QString KGmap::getRandomDivision() const
{
	QList<division*> aux;
	int i = (int)((float)p_nameMap.size() * kapp -> random() / (RAND_MAX + 1.0));
	aux = p_nameMap.values();
	if (!aux[i] -> canAsk()) return getRandomDivision();
	else return aux[i] -> getName();
}

QString KGmap::getWhatIs(QRgb c, bool all) const
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

QColor KGmap::getColor(const QString &s) const
{
	QList<division*> divisions;
	QList<QRgb> colors;
	division *d;
	int i;
	
	d = p_nameMap[s];
	divisions = p_colorMap.values();
	colors = p_colorMap.keys();
	
	i = 0;
	while(divisions[i] != d) i++;
	return QColor(colors[i]);
}
