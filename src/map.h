/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
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
		
		// return the number of askable divisions
		uint count(bool clickDivisionMode) const;
		bool hasAllFlags() const;
		QString getDivisionFlagFile(const QString &s) const;
		QString getDivisionCapital(const QString &s) const;
		QString getFile() const;
		QString getFileName() const;
		QString getMapFile() const;
		QString getName() const;
		QString getRandomDivision(bool clickDivisionMode) const;
		QString getWhatIs(QRgb c, bool all) const;
		QColor getColor(const QString &s) const;
		
	private:
		QMap<QRgb, division*> p_colorMap;
		QMap<QString, division*> p_nameMap;
		
		QString p_name, p_file, p_mapFile;
		bool p_hasAllFlags;
};

#endif
