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

#include <qradiobutton.h>
 
#include "capitaldivisionasker.h"
#include "map.h"

capitalDivisionAsker::capitalDivisionAsker(QWidget *parent, map *m, uint count) : boxAsker(parent, m, count)
{
	init();
}

void capitalDivisionAsker::nextQuestionHook(QString division, int i, bool isAnswer)
{
	p_rb[i] -> setText(division);
	if (isAnswer) setQuestion(i18n("%1 is the capital of...").arg(p_map -> getDivisionCapital(division)));
}
