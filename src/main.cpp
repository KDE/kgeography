/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <kaboutdata.h>
#include <klocalizedstring.h>
#include <qapplication.h>
#include <qcommandlineparser.h>
#include <Kdelibs4ConfigMigrator>
#include "kgeography.h"

int main(int argc, char *argv[])
{
    Kdelibs4ConfigMigrator migrate(QLatin1String("kgeography"));
    migrate.setConfigFiles(QStringList() << QLatin1String("kgeographyrc"));
    migrate.setUiFiles(QStringList() << QLatin1String("kgeographyui.rc"));
    migrate.migrate();
	KLocalizedString::setApplicationDomain("kgeography");

	QApplication app(argc, argv);
	KAboutData about("kgeography", i18n("KGeography"), "0.8.1", i18n("A Geography learning program"), KAboutLicense::GPL, i18n("© 2004-2005 Albert Astals Cid"), QString(), "http://userbase.kde.org/KGeography");
	about.addAuthor(i18n("Albert Astals Cid"), i18n("Programmer and designer"), "aacid@kde.org");
	about.addCredit(i18n("Sodipodi flags collection"), i18n("Got some flags from it"), 0, "http://www.sodipodi.com/index.php3?section=clipart/flags");

	KAboutData::setApplicationData(about);

	QCommandLineParser parser;
	parser.addVersionOption();
	about.setupCommandLine(&parser);
	parser.process(app);
	about.processCommandLine(&parser);

	new kgeography();

	return app.exec();
}
