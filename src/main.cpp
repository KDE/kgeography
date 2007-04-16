/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kapplication.h>
#include "kgeography.h"

int main(int argc, char *argv[])
{
	KAboutData about("kgeography", I18N_NOOP("KGeography"), "0.5", I18N_NOOP("A geography learning program"), KAboutData::License_GPL, "© 2004-2005 Albert Astals Cid", 0, "http://kgeography.berlios.de");
	about.addAuthor("Albert Astals Cid", I18N_NOOP("Programmer and designer"), "tsdgeos@terra.es");
	about.addCredit(I18N_NOOP("Sodipodi flags collection"), I18N_NOOP("Got some flags from it"), 0, "http://www.sodipodi.com/index.php3?section=clipart/flags");
	KCmdLineArgs::init(argc, argv, &about);
	KApplication app;

	app.setTopWidget(new kgeography());

	return app.exec();
}
