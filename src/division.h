/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef DIVISION_H
#define DIVISION_H

#include <qcolor.h>
#include <qstring.h>

class division
{
	public:
		division();
		
		bool canAsk() const;
		QString getName() const;
		QRgb getRGB() const;
		
		void setIgnore(bool b);
		void setName(QString name);
		void setRGB(int r, int g, int b);
		
	private:
		QString p_name;
		QRgb p_color;
		bool p_canAsk;
};

#endif
