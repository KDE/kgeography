/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef MAPPARSER_H
#define MAPPARSER_H

#include <qxml.h>

#include "map.h"

class division;

class mapReader : public QXmlSimpleReader
{
	public:
		mapReader();
		
		bool parseMap(QString path);
		QString getError();
		map *getMap();
	
	private:
		QString p_error;
		map *p_map;
};

class mapParser : public QXmlDefaultHandler
{
	public:
		mapParser(map *m, QString &path);
		bool startDocument();
		bool startElement(const QString&, const QString&, const QString&, const QXmlAttributes&);
		bool endElement(const QString &, const QString &name, const QString &);
		bool characters(const QString &ch);
		bool endDocument();
		QString errorString();
	
	private:
		QString getPreviousTag() const;
		void removeLastTag();
		
		division *p_division;
		int p_red, p_green, p_blue;
		map *p_map;
		bool p_allowChars, p_mapNameSet, p_mapFileSet;
		bool p_divisionNameSet, p_colorSet, p_divisionIgnoreSet, p_flagFileSet;
		QString &p_path, p_error, p_contents, p_previousTags;
};

#endif
