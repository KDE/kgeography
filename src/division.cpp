/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "division.h"

division::division()
{
	p_canAsk = true;
}

bool division::canAsk() const
{
	return p_canAsk;
}

QString division::getName() const
{
	return p_name;
}

QRgb division::getRGB() const
{
	return p_color;
}

void division::setIgnore(bool b)
{
	p_canAsk = !b;
}

void division::setName(QString name)
{
	p_name = name;
}

void division::setRGB(int r, int g, int b)
{
	p_color = qRgb(r, g, b);
}
