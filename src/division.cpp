/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <klocale.h>

#include <qfile.h>

#include "division.h"

division::division()
{
	p_canAsk = true;
	p_flagFile = QString::null;
}

bool division::canAsk() const
{
	return p_canAsk;
}

QString division::getName() const
{
	//utf8 is needed to make names like Côte d'Ivoire work
	return i18n(p_name.utf8());
}

QRgb division::getRGB() const
{
	return p_color;
}

QString division::getFlagFile() const
{
	return p_flagFile;
}

void division::setIgnore(bool b)
{
	p_canAsk = !b;
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
