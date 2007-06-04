/***************************************************************************
 *   Copyright (C) 2004-2006 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef MAPPARSER_H
#define MAPPARSER_H

class QDomElement;

class KGmap;
#include <QString>

class mapReader
{
	public:
		mapReader();
		
		KGmap *parseMap(const QString &path);
		QString getError();
	
	private:
		enum eMandatoryness { Mandatory, Optional };
		QString getElementString(const QString &tagName, const QDomElement &parentTag, eMandatoryness mandatoryness);
		QDomElement getElement(const QString &tagName, const QDomElement &parentTag);
	
		QString p_error;
};

#endif
