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

bool divisionCapitalAsker::nextBoxAskerQuestionHook(const QString &division, int i, bool isAnswer)
{
	bool b;
	if (isAnswer)
	{
		setQuestion(i18n("The capital of %1 is...").arg(division));
		p_currentAnswer.setCorrectAnswer(p_rb[i] -> text());
		p_currentAnswer.setQuestion(division);
		p_capital = p_map -> getDivisionCapital(division);
		p_rb[i] -> setText(p_capital);
		b = true;
	}
	else
	{
		QString capital;
		capital = p_map -> getDivisionCapital(division);
		// There are crazy countries like Norway where two different divisions
		// have the same capital, avoid asking
		// The capital of Akershus is ...
		// Oslo (true)
		// Oslo (true)
		// Lillehammer
		// Moss
		if (capital != p_capital)
		{
			p_rb[i] -> setText(capital);
			b = true;
		}
		else b = false;
	}
	return b;
}

void divisionCapitalAsker::setAnswerHook(int userSays)
{
	p_currentAnswer.setAnswer(p_rb[userSays] -> text());
}

QString divisionCapitalAsker::getQuestionHook() const
{
	return i18n("The capital of ... is ...");
}
