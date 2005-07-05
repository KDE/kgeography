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

capitalDivisionAsker::capitalDivisionAsker(QWidget *parent, KGmap *m, QWidget *w, uint count) : boxAsker(parent, m, w, count)
{
	init();
}

bool capitalDivisionAsker::nextBoxAskerQuestionHook(const QString &division, int i, bool isAnswer)
{
	bool b;
	if (isAnswer)
	{
		p_capital = p_map -> getDivisionCapital(division);
		p_currentAnswer.setQuestion(i18n(p_map -> getFileName().utf8(), p_capital.utf8()));
		p_currentAnswer.setCorrectAnswer(i18n(p_map -> getFileName().utf8(), division.utf8()));
		QString s = QString("%1 is the capital of...").arg(p_capital);
		setQuestion(i18n(p_map -> getFileName().utf8(), s.utf8()));
		p_rb[i] -> setText(i18n(p_map -> getFileName().utf8(), division.utf8()));
		b = true;
	}
	else
	{
		// There are crazy countries like Norway where two different divisions
		// have the same capital, avoid asking
		// Oslo is the capital of ...
		// Oslo (true)
		// Akershus (true)
		// Buskerud
		// Hedmark
		if (p_map -> getDivisionCapital(division) != p_capital)
		{
			p_rb[i] -> setText(i18n(p_map -> getFileName().utf8(), division.utf8()));
			b = true;
		}
		else b = false;
	}
	return b;
}

void capitalDivisionAsker::setAnswerHook(int userSays)
{
	p_currentAnswer.setAnswer(p_rb[userSays] -> text());
}

QString capitalDivisionAsker::getQuestionHook() const
{
	return i18n("Division From Its Capital");
}
