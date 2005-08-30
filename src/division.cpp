/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <qfile.h>

#include "division.h"

division::division()
{
	p_canAskAlways = true;
	p_canAskClickDivision = true;
	p_flagFile = QString::null;
}

bool division::canAsk(bool clickDivisionMode) const
{
	if (clickDivisionMode) return p_canAskClickDivision;
	else return p_canAskAlways;
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
	if (p_capital.isEmpty()) return "";
	return p_capital;
}

void division::setCapital(const QString &name)
{
	p_capital = name;
}

void division::setIgnore(bool ignore, bool canAskClickDivision)
{
	if (ignore)
	{
		p_canAskAlways = false;
		p_canAskClickDivision = canAskClickDivision;
	}
	else
	{
		p_canAskAlways = true;
		p_canAskClickDivision = true;
	}
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
