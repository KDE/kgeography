/***************************************************************************
 *   Copyright (C) 2004-2007 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "mapparser.h"

#include <QDir>
#include <QDomDocument>
#include <QFileInfo>

#include <klocale.h>

#include "division.h"
#include "map.h"

mapReader::mapReader()
{
}

KGmap *mapReader::parseMap(const QString &path)
{
	QString baseDir;
	p_error.clear();
	KGmap *kgmap = new KGmap();
	kgmap -> setFile(path);
	baseDir = QFileInfo(path).absolutePath() + '/'; // baseDir = path but without the file name
	QFile xmlFile(path);
	if (xmlFile.exists())
	{
		if (xmlFile.open(QIODevice::ReadOnly))
		{
			QDomDocument doc;
			doc.setContent(xmlFile.readAll());
			const QDomElement &root = doc.documentElement();
			if (root.tagName() == "map")
			{
				// Map name
				kgmap -> setName( getElementString("name", root, Mandatory) );
				
				// Map image file
				if (!kgmap -> setMapFile( baseDir + getElementString("mapFile", root, Mandatory) ))
				{
					p_error = i18n("The map image file for %1 does not exist", kgmap -> getName());
				}
				
				// Divisions string
				kgmap -> setDivisionsString( getElementString("divisionsName", root, Mandatory) );
				
				// Author string
				kgmap -> setAuthor( getElementString("author", root, Mandatory) );
				
				QDomElement divisionTag = root.firstChildElement("division");
				while (!divisionTag.isNull())
				{
					division *kgdiv = new division();
					
					// division name
					kgdiv -> setName( getElementString("name", divisionTag, Mandatory) );
					
					// division capital
					QString capital = getElementString("capital", divisionTag, Optional);
					if (!capital.isNull()) kgdiv -> setCapital( capital );
					
					// division flag
					QString flagFile = getElementString("flag", divisionTag, Optional);
					if (!flagFile.isNull())
					{
						if (!kgdiv -> setFlagFile( baseDir + "/flags/" + flagFile ))
						{
							p_error = i18n("The flag image file for %1 does not exist", kgdiv -> getName());
						}
					}
					
					// division ignoreness
					const QString &ignore = getElementString("ignore", divisionTag, Optional).toLower();
					if (!ignore.isNull())
					{
						if (ignore == "yes") kgdiv -> setCanAsk(division::eNone);
						else if (ignore == "no") kgdiv -> setCanAsk(division::eClick | division::eCapital | division::eFlag);
						else if (ignore == "allowclickmode") kgdiv -> setCanAsk(division::eClick);
						else if (ignore == "allowclickflagmode") kgdiv -> setCanAsk(division::eClick | division::eFlag);
						else
						{
							p_error = i18n("Invalid value in tag %1", QString("<ignore>"));
						}
					}
					
					// division color
					const QDomElement &colorTag = getElement("color", divisionTag);
					kgdiv -> setRGB(getElementString("red", colorTag, Mandatory).toInt(),
					                getElementString("green", colorTag, Mandatory).toInt(),
					                getElementString("blue", colorTag, Mandatory).toInt());
					
					// division false capitals
					QStringList falseCapitals;
					const QDomNodeList &falseCapitalTags = divisionTag.elementsByTagName("falseCapital");
					QDomElement falseCapital;
					for (int i = 0; i < falseCapitalTags.count(); ++i)
					{
						falseCapital = falseCapitalTags.item(i).toElement();
						if (!falseCapital.isNull()) falseCapitals << falseCapital.text();
					}
					if (!falseCapitals.isEmpty()) kgdiv->setFalseCapitals(falseCapitals);
					
					if (!kgmap -> addDivision(kgdiv))
					{
						p_error = i18n("There is already either a division called %1 or a division with the same colors as %2", kgdiv -> getName(), kgdiv -> getName());
						delete kgdiv;
					}
					
					divisionTag = divisionTag.nextSiblingElement("division");
				}
			}
			else
			{
				p_error = i18n("The map description file should begin with the %1 tag", QString("map"));
			}
			xmlFile.close();
		}
		else
		{
			p_error = i18n("Could not open %1 for reading.", path);
		}
	}
	else
	{
		p_error = i18n("%1 does not exist.", path);
	}
	
	if (p_error.isNull()) return kgmap;
	else
	{
		delete kgmap;
		return NULL;
	}
}

QString mapReader::getError() const
{
	return p_error;
}

QString mapReader::getElementString(const QString &tagName, const QDomElement &parentTag, eMandatoryness mandatoryness)
{
	QString result;
	const QDomElement &tag = parentTag.firstChildElement(tagName);
	if (tag.isNull())
	{
		if (mandatoryness == Mandatory)
		{
			p_error = i18n("The map description file should have a %1 tag inside %2", tagName, parentTag.tagName());
		}
	}
	else
	{
		if (tag == parentTag.lastChildElement(tagName))
		{
			result = tag.text();
		}
		else
		{
			p_error = i18n("The map description file should have exactly one %1 tag inside %2", tagName, parentTag.tagName());
		}
	}
	return result;
}

QDomElement mapReader::getElement(const QString &tagName, const QDomElement &parentTag)
{
	const QDomElement &tag = parentTag.firstChildElement(tagName);
	if (tag.isNull())
	{
		p_error = i18n("The map description file should have a %1 tag inside %2", tagName, parentTag.tagName());
	}
	else
	{
		if (tag != parentTag.lastChildElement(tagName))
		{
			p_error = i18n("The map description file should have exactly one %1 tag inside %2", tagName, parentTag.tagName());
		}
	}
	return tag;
}
