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
#include <qlayout.h>
#include <qradiobutton.h>
 
#include "flagdivisionasker.h"
#include "map.h"

flagDivisionAsker::flagDivisionAsker(QWidget *parent, map *m, uint count) : boxAsker(parent, m, count)
{
	p_flag = new QWidget(this);
	p_lay -> insertWidget(0, p_flag);
	setQuestion(i18n("This flag belongs to:"));
	init();
}

void flagDivisionAsker::nextQuestionHook(QString division, int i, bool isAnswer)
{
	if (isAnswer)
	{
		// we put the flag image
		QImage image(p_map -> getDivisionFlagFile(division));
		p_flag -> setPaletteBackgroundPixmap(image);
		p_flag -> setFixedSize(image.size());
	}
	p_rb[i] -> setText(division);
}
