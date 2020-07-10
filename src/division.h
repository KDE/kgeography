/***************************************************************************
 *   Copyright (C) 2004-2007 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef DIVISION_H
#define DIVISION_H

#include <QColor>
#include <QString>

class division
{
	public:
		division();
		
		enum askMode { eNone = 0,
		               eClick = 1,
		               eCapital = 2,
		               eFlag = 4};
		
		bool canAsk(askMode am) const;
		QString getUntranslatedName() const;
		QString getName() const;
		QRgb getRGB() const;
		QString getFlagFile() const;
		QString getCapital() const;
		QStringList getFalseCapitals() const;
		
		void setCapital(const QString &name);
		void setFalseCapitals(const QStringList &falseCapitals);
		void setCanAsk(int askMode);
		void setUntranslatedName(const QString &name);
		void setName(const QString &name);
		void setRGB(int r, int g, int b);
		bool setFlagFile(const QString &path);
		
	private:
		QString p_untranslatedName, p_name, p_flagFile, p_capital;
		QStringList p_falseCapitals;
		QRgb p_color;
		int p_askMode;
};

#endif
