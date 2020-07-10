/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <KAboutData>
#include <KLocalizedString>
#include <QApplication>
#include <QCommandLineParser>
#include <Kdelibs4ConfigMigrator>
#include <KCrash>
#include "kgeography.h"
#include "kgeography_version.h"

int main(int argc, char *argv[])
{
        QApplication app(argc, argv);

	Kdelibs4ConfigMigrator migrate(QStringLiteral("kgeography"));
	migrate.setConfigFiles(QStringList() << QStringLiteral("kgeographyrc"));
	migrate.setUiFiles(QStringList() << QStringLiteral("kgeographyui.rc"));
	migrate.migrate();
	KLocalizedString::setApplicationDomain("kgeography");

	KAboutData about(QStringLiteral("kgeography"), i18n("KGeography"), KGEOGRAPHY_VERSION_STRING, i18n("A Geography learning program"), KAboutLicense::GPL, i18n("© 2004-2005 Albert Astals Cid"), QString(), QStringLiteral("https://userbase.kde.org/KGeography"));
	about.addAuthor(i18n("Albert Astals Cid"), i18n("Programmer and designer"), QStringLiteral("aacid@kde.org"));

	KAboutData::setApplicationData(about);

        app.setWindowIcon(QIcon::fromTheme(QStringLiteral("kgeography")));
	app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

	KCrash::initialize();

	QCommandLineParser parser;
	about.setupCommandLine(&parser);
	parser.process(app);
	about.processCommandLine(&parser);

	new kgeography();

	return app.exec();
}
