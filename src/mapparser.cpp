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

#include <QDomDocument>
#include <QFileInfo>
#include <QTextStream>

#include <klocalizedstring.h>

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
			if (root.tagName() == QLatin1String("map"))
			{
				const QByteArray ctxt = kgmap -> getFileName().toUtf8();
				
				// Map name
				kgmap -> setName(i18nc(ctxt, getElementString(QStringLiteral("name"), root, Mandatory).toUtf8()));
				
				// Map image file
				if (!kgmap -> setMapFile( baseDir + getElementString(QStringLiteral("mapFile"), root, Mandatory) ))
				{
					p_error = i18n("The map image file for %1 does not exist", kgmap -> getName());
				}
				
				// Divisions string
				QString divisionKindName = getElementString(QStringLiteral("divisionsName"), root, Mandatory);
				kgmap -> setDivisionsString(i18nc(ctxt, divisionKindName.toUtf8()));

				QString pat = getElementString(QStringLiteral("capitalToDivisionPattern"), root, Optional);
				if ( pat.contains('%') )
				{
					kgmap->setCapitalToDivisionQuestionPattern(pat);
				}
				else if ( ! pat.isEmpty() )
				{
					QTextStream stream(stderr);
					stream << "capitalToDivisionPattern element should contain one '%%' in map " << kgmap->getName() << kgmap -> getFile();
				}

				pat = getElementString(QStringLiteral("divisionToCapitalPattern"), root, Optional);
				if ( pat.contains('%') )
				{
					kgmap->setDivisionToCapitalQuestionPattern(pat);
				}
				else if ( ! pat.isEmpty() )
				{
					QTextStream stream(stderr);
					stream << "divisionToCapitalPattern element should contain one '%%' in map " << kgmap->getName() << kgmap -> getFile();
				}

				QString title = getElementString(QStringLiteral("capitalToDivisionTitle"), root, Optional);
				if ( ! title.isEmpty() )
				{
					kgmap->setCapitalToDivisionTitle(i18nc("@title", title.toUtf8()));
				}
				else
				{
					kgmap->setCapitalToDivisionTitle(i18n("%1 by Capital", kgmap -> getDivisionsString()));
				}

				title = getElementString(QStringLiteral("divisionToCapitalTitle"), root, Optional);
				if ( ! title.isEmpty() )
				{
					kgmap->setDivisionToCapitalTitle(i18nc("@title", title.toUtf8()));
				}
				else
				{
					kgmap->setDivisionToCapitalTitle(i18n("Capitals of %1", kgmap -> getDivisionsString()));
				}

				
				// Author string
				kgmap -> setAuthor( getElementString(QStringLiteral("author"), root, Mandatory) );
				
				QDomElement divisionTag = root.firstChildElement(QStringLiteral("division"));
				while (!divisionTag.isNull())
				{
					division *kgdiv = new division();
					
					// division name
					kgdiv -> setUntranslatedName( getElementString(QStringLiteral("name"), divisionTag, Mandatory) );
					kgdiv -> setName(i18nc(ctxt, kgdiv -> getUntranslatedName().toUtf8()));
					
					// division capital
					QString capital = getElementString(QStringLiteral("capital"), divisionTag, Optional);
					if (!capital.isEmpty()) kgdiv -> setCapital(i18nc(ctxt, capital.toUtf8()));
					
					// division flag
					QString flagFile = getElementString(QStringLiteral("flag"), divisionTag, Optional);
					if (!flagFile.isNull())
					{
						if (!kgdiv -> setFlagFile( baseDir + "/flags/" + flagFile ))
						{
							p_error = i18n("The flag image file for %1 does not exist", kgdiv -> getName());
						}
					}
					
					// division ignoreness
					const QString &ignore = getElementString(QStringLiteral("ignore"), divisionTag, Optional).toLower();
					if (!ignore.isNull())
					{
						if (ignore == QLatin1String("yes")) kgdiv -> setCanAsk(division::eNone);
						else if (ignore == QLatin1String("no")) kgdiv -> setCanAsk(division::eClick | division::eCapital | division::eFlag);
						else if (ignore == QLatin1String("allowclickmode")) kgdiv -> setCanAsk(division::eClick);
						else if (ignore == QLatin1String("allowclickflagmode")) kgdiv -> setCanAsk(division::eClick | division::eFlag);
						else
						{
							p_error = i18n("Invalid value in tag %1", QStringLiteral("<ignore>"));
						}
					}
					
					// division color
					const QDomElement &colorTag = getElement(QStringLiteral("color"), divisionTag);
					kgdiv -> setRGB(getElementString(QStringLiteral("red"), colorTag, Mandatory).toInt(),
					                getElementString(QStringLiteral("green"), colorTag, Mandatory).toInt(),
					                getElementString(QStringLiteral("blue"), colorTag, Mandatory).toInt());
					
					// division false capitals
					QStringList falseCapitals;
					const QDomNodeList &falseCapitalTags = divisionTag.elementsByTagName(QStringLiteral("falseCapital"));
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
					
					divisionTag = divisionTag.nextSiblingElement(QStringLiteral("division"));
				}
			}
			else
			{
				p_error = i18n("The map description file should begin with the %1 tag", QStringLiteral("map"));
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
