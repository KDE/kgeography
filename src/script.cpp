/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
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
	output->open(IO_WriteOnly | IO_Truncate);
	QTextStream os(output);
	
	QStringList files = d.entryList();
	QStringList::const_iterator it;
	QDomDocument dd;

	for (it = files.begin(); it != files.end(); ++it)
	{
		QFile *f = new QFile(d.absPath() + "/" + *it);
		f -> open(IO_ReadOnly);
		dd.setContent(f);
		
		QDomNode n, n2, n3, n4, n5;
		QDomNodeList nodes = dd.firstChild().childNodes();
		for (uint i = 0; i < nodes.count(); i++)
		{
			n = nodes.item(i);
			if (n.nodeName() == "name")
			{
				os << "i18n(\"" << *it << "\", \"" << n.firstChild().nodeValue().utf8() << "\");\n";
				os << "i18n(\"" << *it << "\", \"<qt>Current map:<br><b>" << n.firstChild().nodeValue().utf8() << "</b></qt>\");\n";
			}
			else if (n.nodeName() == "division")
			{
				n2 = n.namedItem("name");
				os << "i18n(\"" << *it << "\", \"" << n2.firstChild().nodeValue().utf8() << "\");\n";
				
				n3 = n.namedItem("ignore");
				if (n3.isNull() || n3.firstChild().nodeValue() != "yes")
				{
					os << "i18n(\"" << *it << "\", \"The capital of " << n2.firstChild().nodeValue().utf8() << " is...\");\n";
					os << "i18n(\"" << *it << "\", \"Please click on:\\n" << n2.firstChild().nodeValue().utf8() << "\");\n";
					
					n4 = n.namedItem("flag");
					if (!n4.isNull())
					{
						os << "i18n(\"" << *it << "\", \"The flag of " << n2.firstChild().nodeValue().utf8() << " is...\");\n";
					}
					
					n5 = n.namedItem("capital");
					os << "i18n(\"" << *it << "\", \"" << n5.firstChild().nodeValue().utf8() << "\");\n";
					os << "i18n(\"" << *it << "\", \"" << n5.firstChild().nodeValue().utf8() << " is the capital of...\");\n";
				}
			}
		}
		
		f -> close();
		delete f;
	}
	output->close();
}
