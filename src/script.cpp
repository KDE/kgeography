/***************************************************************************
 *   Copyright (C) 2004-2007 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <stdio.h>

#include <qdir.h>
#include <qdom.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qtextstream.h>

static const QString copyright =
"/***************************************************************************\n"
" *   Copyright (C) 2004-2007 by Albert Astals Cid                          *\n"
" *   aacid@kde.org                                                         *\n"
" *                                                                         *\n"
" *   This program is free software; you can redistribute it and/or modify  *\n"
" *   it under the terms of the GNU General Public License as published by  *\n"
" *   the Free Software Foundation; either version 2 of the License, or     *\n"
" *   (at your option) any later version.                                   *\n"
" ***************************************************************************/\n";

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		qDebug("Usage: ./script dirWithkgmFiles fileToOutputTranslations");
		return 1;
	}
	
	QDir d(argv[1], "*.kgm");
	if (!d.exists()) return 2;
	
	QFile *output = new QFile(argv[2]);
	output->open(QIODevice::WriteOnly | QIODevice::Truncate);
	QTextStream os(output);
	os.setCodec("UTF-8");
	
	QStringList files = d.entryList();
	QStringList::const_iterator it;
	QDomDocument dd;

	os << copyright;

	for (it = files.constBegin(); it != files.constEnd(); ++it)
	{
		QFile *f = new QFile(d.absolutePath() + QDir::separator() + *it);
		f -> open(QIODevice::ReadOnly);
		dd.setContent(f);
		
		QDomNode n, n2, n3, n4, n5;
		QDomNodeList nodes = dd.firstChild().childNodes();
		for (int i = 0; i < nodes.count(); i++)
		{
			n = nodes.item(i);
			if (n.nodeName() == "name")
			{
				os << "i18nc(\"" << *it << "\", \"" << n.firstChild().nodeValue() << "\");\n";
			}
			else if (n.nodeName() == "division")
			{
				n2 = n.namedItem("name");
				os << "i18nc(\"" << *it << "\", \"" << n2.firstChild().nodeValue() << "\");\n";
				
				n3 = n.namedItem("ignore");
				if (n3.isNull() || n3.firstChild().nodeValue() == "no")
				{
					n5 = n.namedItem("capital");
					if (!n5.firstChild().nodeValue().isEmpty())
					{
						os << "i18nc(\"" << *it << "\", \"" << n5.firstChild().nodeValue() << "\");\n";
					}
				}
			}
			else if (n.nodeName() == "divisionsName")
			{
				os << "i18nc(\"" << *it << "\", \"" << n.firstChild().nodeValue() << "\");\n";
			}
		}
		
		f -> close();
		delete f;
	}
	output->close();
        delete output;
}
