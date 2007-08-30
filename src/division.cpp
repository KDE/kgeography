/***************************************************************************
 *   Copyright (C) 2004-2007 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "division.h"

#include <qfile.h>

division::division() : p_askMode(eClick | eCapital | eFlag)
{
}

bool division::canAsk(askMode am) const
{
	bool can = p_askMode & am;
	
	switch(am)
	{
		case eClick:
			can = can && !p_name.isEmpty();
		break;
		
		case eCapital:
			can = can && !p_capital.isEmpty();
		break;
		
		case eFlag:
			can = can && !p_flagFile.isEmpty();
		break;
		
		default:
			can = false;
		break;
	}
	return can;
}

QString division::getName() const
{
	return p_name;
}

QRgb division::getRGB() const
{
	return p_color;
}

QString division::getFlagFile() const
{
	return p_flagFile;
}

QString division::getCapital() const
{
	return p_capital;
}

QStringList division::getFalseCapitals() const
{
	return p_falseCapitals;
}

void division::setCapital(const QString &name)
{
	p_capital = name;
}

void division::setFalseCapitals(const QStringList &falseCapitals)
{
	p_falseCapitals = falseCapitals;
}

void division::setCanAsk(int askMode)
{
	p_askMode = askMode;
}

void division::setName(const QString &name)
{
	p_name = name;
}

void division::setRGB(int r, int g, int b)
{
	p_color = qRgb(r, g, b);
}

bool division::setFlagFile(const QString &path)
{
	p_flagFile = path;
	return QFile::exists(path);
}
