/***************************************************************************
 *   Copyright (C) 2004-2006 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef MAP_H
#define MAP_H

#include <qcolor.h>
#include <qmap.h>

class division;

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
		
		// return the number of askable divisions
		uint count(bool clickDivisionMode) const;
		bool hasAllFlags() const;
		QString getDivisionFlagFile(const QString &divisionName) const;
		QString getDivisionCapital(const QString &divisionName) const;
		QStringList getDivisionFalseCapitals(const QString &divisionName) const;
		QString getFile() const;
		QString getFileName() const;
		QString getMapFile() const;
		QString getName() const;
		QString getDivisionsString() const;
		QString getRandomDivision(bool clickDivisionMode) const;
		QString getWhatIs(QRgb c, bool all) const;
		QColor getColor(const QString &s) const;
		const QList<division*> getIgnoredDivisions(bool clickDivisionMode) const;
		
	private:
		division *getDivision(const QString &divisionName) const;
		division *getDivision(QRgb c) const;

		QList<division*> p_divisions;
		
		QString p_name, p_file, p_mapFile;
		QString p_divisionsString;
		bool p_hasAllFlags;
};

#endif
