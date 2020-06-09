/***************************************************************************
 *   Copyright (C) 2004-2007 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef MAP_H
#define MAP_H

#include <klocalizedstring.h>

#include <qcolor.h>
#include <qmap.h>

#include "division.h"

class KGmap
{
	public:
		KGmap();
		~KGmap();
		
		bool addDivision(division *d);
		
		void setFile(const QString &s);
		bool setMapFile(const QString &s);
		void setName(const QString &s);
		void setDivisionsString(const QString &s);
		void setAuthor(const QString &s);
		void setCapitalToDivisionQuestionPattern(const QString &str);
		void setDivisionToCapitalQuestionPattern(const QString &str);
		void setCapitalToDivisionTitle(const QString &str);
		void setDivisionToCapitalTitle(const QString &str);
		
		// return the number of askable divisions
		uint count(division::askMode am) const;
		QString getDivisionFlagFile(const QString &divisionName) const;
		QString getDivisionCapital(const QString &divisionName) const;
		QStringList getDivisionFalseCapitals(const QString &divisionName) const;
		bool getDivisionCanAsk(const QString &div, division::askMode am) const;
		QString getFile() const;
		QString getFileName() const;
		QString getMapFile() const;
		QString getName() const;
		QString getDivisionsString() const;
		division *getRandomDivision(division::askMode am) const;
		QString getWhatIs(QRgb c, bool all) const;
		QString getAuthor() const;
		QColor getColor(const QString &s) const;
		QList<const division*> getIgnoredDivisions(division::askMode am) const;
		QList<const division*> getAllDivisionsOrdered() const;
		KLocalizedString getCapitalToDivisionQuestionPattern() const;
		KLocalizedString getDivisionToCapitalQuestionPattern() const;
		QString getCapitalToDivisionTitle() const;
		QString getDivisionToCapitalTitle() const;
		division *getDivision(QRgb c) const;
		division *getDivision(const QString &divisionName) const;
		
	private:
		QList<division*> p_divisions;
		
		QString p_name, p_file, p_mapFile;
		QString p_divisionsString;
		QString p_capitalToDivisionPattern;
		QString p_divisionToCapitalPattern;
		QString p_capitalToDivisionTitle;
		QString p_divisionToCapitalTitle;
		QString p_author;
};

#endif
