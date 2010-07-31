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

#include <tqdir.h>
#include <tqdom.h>
#include <tqstring.h>
#include <tqstringlist.h>

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		qDebug("Usage: ./script dirWithkgmFiles fileToOutputTranslations");
		return 1;
	}
	
	TQDir d(argv[1], "*.kgm");
	if (!d.exists()) return 2;
	
	TQFile *output = new TQFile(argv[2]);
	output->open(IO_WriteOnly | IO_Truncate);
	TQTextStream os(output);
	os.setEncoding(TQTextStream::UnicodeUTF8);
	
	TQStringList files = d.entryList();
	TQStringList::const_iterator it;
	TQDomDocument dd;

	for (it = files.begin(); it != files.end(); ++it)
	{
		TQFile *f = new TQFile(d.absPath() + "/" + *it);
		f -> open(IO_ReadOnly);
		dd.setContent(f);
		
		TQDomNode n, n2, n3, n4, n5;
		TQDomNodeList nodes = dd.firstChild().childNodes();
		for (uint i = 0; i < nodes.count(); i++)
		{
			n = nodes.item(i);
			if (n.nodeName() == "name")
			{
				os << "i18n(\"" << *it << "\", \"" << n.firstChild().nodeValue() << "\");\n";
				os << "i18n(\"" << *it << "\", \"<qt>Current map:<br><b>" << n.firstChild().nodeValue() << "</b></qt>\");\n";
			}
			else if (n.nodeName() == "division")
			{
				n2 = n.namedItem("name");
				os << "i18n(\"" << *it << "\", \"" << n2.firstChild().nodeValue() << "\");\n";
				
				n3 = n.namedItem("ignore");
				if (n3.isNull() || (n3.firstChild().nodeValue() != "yes" && n3.firstChild().nodeValue() != "allowClickMode"))
				{
					os << "i18n(\"" << *it << "\", \"The capital of " << n2.firstChild().nodeValue() << " is...\");\n";
					os << "i18n(\"" << *it << "\", \"Please click on:\\n" << n2.firstChild().nodeValue() << "\");\n";
					
					n4 = n.namedItem("flag");
					if (!n4.isNull())
					{
						os << "i18n(\"" << *it << "\", \"The flag of " << n2.firstChild().nodeValue() << " is...\");\n";
					}
					
					n5 = n.namedItem("capital");
					os << "i18n(\"" << *it << "\", \"" << n5.firstChild().nodeValue() << "\");\n";
					os << "i18n(\"" << *it << "\", \"" << n5.firstChild().nodeValue() << " is the capital of...\");\n";
				}
				else if (n3.firstChild().nodeValue() == "allowClickMode")
				{
					os << "i18n(\"" << *it << "\", \"Please click on:\\n" << n2.firstChild().nodeValue() << "\");\n";
				}
			}
		}
		
		f -> close();
		delete f;
	}
	output->close();
}
