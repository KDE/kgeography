/***************************************************************************
 *   Copyright (C) 2004-2006 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
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
		
		bool canAsk(bool clickDivisionMode) const;
		QString getName() const;
		QRgb getRGB() const;
		QString getFlagFile() const;
		QString getCapital() const;
		QStringList getFalseCapitals() const;
		
		void setCapital(const QString &name);
		void setFalseCapitals(const QStringList &falseCapitals);
		void setIgnore(bool ignore, bool canAskClickDivision);
		void setName(const QString &name);
		void setRGB(int r, int g, int b);
		bool setFlagFile(const QString &path);
		
	private:
		QString p_name, p_flagFile, p_capital;
		QStringList p_falseCapitals;
		QRgb p_color;
		bool p_canAskAlways, p_canAskClickDivision;
};

#endif
