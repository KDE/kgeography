/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <klocale.h>

#include "division.h"
#include "map.h"
#include "mapparser.h"

/* unitReader */

mapReader::mapReader() : QXmlSimpleReader()
{
}

bool mapReader::parseMap(const QString &path)
{
	QString aux;
	p_map = new KGmap();
	p_map -> setFile(path);
	aux = path.left(path.findRev('/') + 1); // aux = path but without the file name
	mapParser handler(p_map, aux);
	QFile xmlFile(path);
	if (xmlFile.exists())
	{
		QXmlInputSource source(&xmlFile);
		setContentHandler(&handler);
		if (parse(source)) return true;
		p_error = handler.errorString();
	}
	else
	{
		p_error = i18n("%1 does not exist.").arg(path);
	}
	return false;
}

QString mapReader::getError()
{
	return p_error;
}

KGmap *mapReader::getMap()
{
	return p_map;
}

/* mapParser */

mapParser::mapParser(KGmap *m, const QString &path) : QXmlDefaultHandler(), p_map(m), p_path(path)
{
}

bool mapParser::startDocument()
{
	p_mapNameSet = false;
	p_mapFileSet = false;
	return true;
}

bool mapParser::startElement(const QString&, const QString &name, const QString&, const QXmlAttributes&)
{
	QString prev;
	bool b = true;
	prev = getPreviousTag();
	p_previousTags += ":" + name;
	if (prev.isEmpty())
	{
		b = name == "map";
		if (!b) p_error = i18n("The map description file should begin with the %1 tag").arg("map");
	}
	else if (prev == "map")
	{
		if (name != "mapFile" && name != "name" && name != "division")
		{
			b = false;
			p_error = i18n("%1 is not a valid tag inside tag %2. Valid tags are %3, %4 and %5").arg(name).arg(prev).arg("mapFile").arg("name").arg("division");
		}
		else if ((name == "mapFile" && p_mapFileSet) || (name == "name" && p_mapNameSet))
		{
			b = false;
			p_error = i18n("%1 tag has already been set").arg(name);
		}
		p_colorSet = false;
		if (name == "division")
		{
			p_divisionNameSet = false;
			p_divisionIgnoreSet = false;
			p_flagFileSet = false;
			p_capitalSet = false;
			p_division = new division();
		}
	}
	else if (prev == "mapFile" || prev == "name" || prev == "red" || prev == "green" || prev == "blue" ||
			prev == "ignore")
	{
		b = false;
		p_error = i18n("There can not be a tag inside %1 tag").arg(prev);
	}
	else if (prev == "division")
	{
		if (name != "color" && name != "name" && name != "ignore" && name != "flag" && name != "capital")
		{
			b = false;
			p_error = i18n("%1 is not a valid tag inside tag %2. Valid tags are %3, %4, %5, %6 and %7").arg(name).arg(prev).arg("color").arg("name").arg("ignore").arg("capital").arg("flag");
		}
		else if ((name == "name" && p_divisionNameSet) || (name == "color" && p_colorSet) ||
		(name == "ignore" && p_divisionIgnoreSet) || (name == "flag" && p_flagFileSet) ||
		(name == "capital" && p_capitalSet))
		{
			b = false;
			p_error = i18n("%1 tag has already been set").arg(name);
		}
		p_red = -1;
		p_green = -1;
		p_blue = -1;
	}
	else if (prev == "color")
	{
		b = (name == "red" && p_red == -1) || (name == "green" && p_green == -1) ||
			(name == "blue" && p_blue == -1);
	}
	else b = false;
	return b;
}

bool mapParser::endElement(const QString &, const QString &, const QString &)
{
	QString aux;
	bool b;
	aux = getPreviousTag();
	b = true;
	if (p_previousTags == ":map:name")
	{
		p_map -> setName(p_contents);
		p_mapNameSet = true;
	}
	else if (p_previousTags == ":map:mapFile")
	{
		b = p_map -> setMapFile(p_path + p_contents);
		p_mapFileSet = true;
		if (!b) p_error = i18n("File %1 does not exist").arg(p_path + p_contents);
	}
	else if (aux == "division")
	{
		p_division -> setRGB(p_red, p_green , p_blue);
		b = p_divisionNameSet;
		if (!b) p_error = i18n("There is a division without name");
		b = b && p_map -> addDivision(p_division);
		if (!b) p_error = i18n("There is already either a division called %1 or a division with the same colors as %2").arg(p_division -> getName()).arg(p_division -> getName());
		b = b && (p_capitalSet || !p_division -> canAsk(false));
		if (!b) p_error = i18n("Division %1 has no capital").arg(p_division -> getName());
	}
	else if (p_previousTags == ":map:division:name")
	{
		p_divisionNameSet = true;
		p_division -> setName(p_contents);
	}
	else if (p_previousTags == ":map:division:capital")
	{
		p_capitalSet = true;
		p_division -> setCapital(p_contents);
	}
	else if (aux == "color")
	{
		if (p_red == -1)
		{
			b = false;
			p_error = i18n("Tag %1 has not the %2 tag.").arg("<color>").arg("<red>");
		}
		else if (p_green == -1)
		{
			b = false;
			p_error = i18n("Tag %1 has not the %2 tag.").arg("<color>").arg("<green>");
		}
		else if (p_blue == -1)
		{
			b = false;
			p_error = i18n("Tag %1 has not the %2 tag.").arg("<color>").arg("<blue>");
		}
		else p_colorSet = true;
	}
	else if (aux == "red")
	{
		p_red = p_contents.toInt();
	}
	else if (aux == "green")
	{
		p_green = p_contents.toInt();
	}
	else if (aux == "blue")
	{
		p_blue = p_contents.toInt();
	}
	else if (aux == "ignore")
	{
		p_divisionIgnoreSet = true;
		if (p_contents.lower() == "yes")
		{
			p_division -> setIgnore(true, false);
		}
		else if (p_contents.lower() == "no")
		{
			p_division -> setIgnore(false, false);
		}
		else if (p_contents.lower() == "allowclickmode")
		{
			p_division -> setIgnore(true, true);
		}
		else
		{
			b = false;
			p_error = i18n("Invalid value in tag %1").arg("<ignore>");
		}
	}
	else if (aux == "flag")
	{
		b = p_division -> setFlagFile(p_path + "flags/" + p_contents);
		p_flagFileSet = true;
		if (!b) p_error = i18n("Could not find flag file %1").arg(p_path + "flags/" + p_contents);
	}
	else if (aux == "map")
	{
	}
	else b = false;
	removeLastTag();
	p_contents = QString::null;
	return b;
}

bool mapParser::characters(const QString &ch)
{
	QString aux;
	if (ch.simplifyWhiteSpace().length() == 0) return true;
	aux = getPreviousTag();
	if (aux == "mapFile" || aux == "name" || aux == "red" || aux == "green" || aux == "blue" || aux == "ignore" || aux == "flag" || aux == "capital")
	{
		p_contents += ch;
		return true;
	}
	p_error = i18n("There are characters outside tags.");
	return false;
}

bool mapParser::endDocument()
{
	QString aux;
	if (p_mapNameSet && p_mapFileSet)
	{
		return true;
	}
	else if (!p_mapNameSet) aux = "name";
	else if (!p_mapFileSet) aux = "mapFile";
	p_error = i18n("Tag %1 is missing.").arg(aux);
	return false;
}

QString mapParser::errorString()
{
	if (!p_error.isEmpty()) return p_error;
	return i18n("The XML document is malformed.");
}

QString mapParser::getPreviousTag() const
{
	return p_previousTags.right(p_previousTags.length() - p_previousTags.findRev(':') - 1);
}

void mapParser::removeLastTag()
{
	p_previousTags = p_previousTags.left(p_previousTags.findRev(':'));
}
