/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
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

flagDivisionAsker::flagDivisionAsker(QWidget *parent, KGmap *m, QWidget *w, uint count) : boxAsker(parent, m, w, count)
{
	p_flag = new QWidget(this);
	p_lay -> insertWidget(0, p_flag);
	setQuestion(i18n("This flag belongs to:"));
	init();
}

bool flagDivisionAsker::nextBoxAskerQuestionHook(const QString &division, int i, bool isAnswer)
{
	if (isAnswer)
	{
		// we put the flag image
		QImage image(p_map -> getDivisionFlagFile(division));
		p_flag -> setPaletteBackgroundPixmap(image);
		p_flag -> setFixedSize(image.size());
		
		p_currentAnswer.setQuestion(image.smoothScale(image.width()/5, image.height()/5));
		p_currentAnswer.setCorrectAnswer(i18n(p_map -> getFileName().utf8(), division.utf8()));
	}
	p_rb[i] -> setText(i18n(p_map -> getFileName().utf8(), division.utf8()));
	return true;
}

void flagDivisionAsker::setAnswerHook(int userSays)
{
	p_currentAnswer.setAnswer(p_rb[userSays] -> text());
}

QString flagDivisionAsker::getQuestionHook() const
{
	return i18n("Division From Its Flag");
}
