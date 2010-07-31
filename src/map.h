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

#include <tqcolor.h>
#include <tqmap.h>

class division;

class KGmap
{
	public:
		KGmap();
		~KGmap();
		
		bool addDivision(division *d);
		
		void setFile(const TQString &s);
		bool setMapFile(const TQString &s);
		void setName(const TQString &s);
		
		// return the number of askable divisions
		uint count(bool clickDivisionMode) const;
		bool hasAllFlags() const;
		TQString getDivisionFlagFile(const TQString &s) const;
		TQString getDivisionCapital(const TQString &s) const;
		TQString getFile() const;
		TQString getFileName() const;
		TQString getMapFile() const;
		TQString getName() const;
		TQString getRandomDivision(bool clickDivisionMode) const;
		TQString getWhatIs(QRgb c, bool all) const;
		TQColor getColor(const TQString &s) const;
		
	private:
		TQMap<QRgb, division*> p_colorMap;
		TQMap<TQString, division*> p_nameMap;
		
		TQString p_name, p_file, p_mapFile;
		bool p_hasAllFlags;
};

#endif
