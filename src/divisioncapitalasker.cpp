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
 
#include "divisioncapitalasker.h"
#include "map.h"

divisionCapitalAsker::divisionCapitalAsker(QWidget *parent, KGmap *m, QWidget *w, uint count) : boxAsker(parent, m, w, count)
{
	init();
}

void divisionCapitalAsker::nextBoxAskerQuestionHook(const QString &division, int i, bool isAnswer)
{
	p_rb[i] -> setText(p_map -> getDivisionCapital(division));
	if (isAnswer)
	{
		setQuestion(i18n("The capital of %1 is...").arg(division));
		p_currentAnswer.setCorrectAnswer(p_rb[i] -> text());
		p_currentAnswer.setQuestion(division);
	}
}

void divisionCapitalAsker::setAnswerHook(int userSays)
{
	p_currentAnswer.setAnswer(p_rb[userSays] -> text());
}

QString divisionCapitalAsker::getQuestionHook() const
{
	return i18n("The capital of ... is ...");
}
