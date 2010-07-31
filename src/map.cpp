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

#include <tqfile.h>
#include <tqfileinfo.h>

#include "division.h"
#include "map.h"

KGmap::KGmap()
{
	p_hasAllFlags = true;
}

KGmap::~KGmap()
{
	TQMap<QRgb, division*>::iterator it;
	it = p_colorMap.begin();
	while (it != p_colorMap.end())
	{
		delete it.data();
		p_colorMap.remove(it);
		it = p_colorMap.begin();
	}
}

bool KGmap::addDivision(division *c)
{
	bool b;
	if (p_nameMap.find(c -> getName()) == p_nameMap.end() && 
		p_colorMap.find(c -> getRGB()) == p_colorMap.end())
	{
		p_colorMap.insert(c -> getRGB(), c);
		p_nameMap.insert(c -> getName(), c);
		b = true;
		if (c -> canAsk(false)) p_hasAllFlags = p_hasAllFlags && !c -> getFlagFile().isNull();
	}
	else b = false;
	return b;
}

void KGmap::setFile(const TQString &s)
{
	p_file = s;
}

bool KGmap::setMapFile(const TQString &s)
{
	p_mapFile = s;
	return TQFile::exists(s);
}

void KGmap::setName(const TQString &s)
{
	p_name = s;
}

uint KGmap::count(bool clickDivisionMode) const
{
	TQValueList<division*> aux = p_nameMap.values();
	TQValueList<division*>::const_iterator it = aux.begin();
	TQValueList<division*>::const_iterator end = aux.end();
	uint count = 0;
	for( ; it != end; ++it)
	{
		if ((*it)->canAsk(clickDivisionMode)) count++;
	}
	return count;
}

bool KGmap::hasAllFlags() const
{
	return p_hasAllFlags;
}

TQString KGmap::getDivisionFlagFile(const TQString &s) const
{
	return p_nameMap[s] -> getFlagFile();
}

TQString KGmap::getDivisionCapital(const TQString &s) const
{
	return p_nameMap[s] -> getCapital();
}

TQString KGmap::getFile() const
{
	return p_file;
}

TQString KGmap::getFileName() const
{
	TQFileInfo fi(p_file);
	return fi.fileName();
}

TQString KGmap::getMapFile() const
{
	return p_mapFile;
}

TQString KGmap::getName() const
{
	return p_name;
}

TQString KGmap::getRandomDivision(bool clickDivisionMode) const
{
	TQValueList<division*> aux;
	int i = (int)((float)p_nameMap.size() * kapp -> random() / (RAND_MAX + 1.0));
	aux = p_nameMap.values();
	if (!aux[i] -> canAsk(clickDivisionMode)) return getRandomDivision(clickDivisionMode);
	else return aux[i] -> getName();
}

TQString KGmap::getWhatIs(QRgb c, bool all) const
{
	// this is only asked from mapasker.cpp hence the true in canAsk
	TQMap<QRgb, division*>::const_iterator it;
	it = p_colorMap.find(c);
	if (it == p_colorMap.end()) return "nothing";
	else
	{
		if (all) return it.data() -> getName();
		else if (it.data() -> canAsk(true)) return it.data() -> getName();
		else return "";
	}
}

TQColor KGmap::getColor(const TQString &s) const
{
	TQValueList<division*> divisions;
	TQValueList<QRgb> colors;
	division *d;
	int i;
	
	d = p_nameMap[s];
	divisions = p_colorMap.values();
	colors = p_colorMap.keys();
	
	i = 0;
	while(divisions[i] != d) i++;
	return TQColor(colors[i]);
}
