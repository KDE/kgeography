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

#include <qimage.h>
#include <qradiobutton.h>
 
#include "divisionflagasker.h"
#include "map.h"

divisionFlagAsker::divisionFlagAsker(QWidget *parent, map *m, QWidget *w, uint count) : boxAsker(parent, m, w, count)
{
	init();
}

void divisionFlagAsker::nextBoxAskerQuestionHook(QString division, int i, bool isAnswer)
{
	if (isAnswer)
	{
		setQuestion(i18n("The flag of %1 is...").arg(division));
	}
	QImage im(p_map -> getDivisionFlagFile(division));
	im = im.smoothScale(im.width()/5, im.height()/5);
	p_rb[i] -> setPixmap(im);
}
