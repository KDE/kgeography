/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "capitaldivisionasker.h"

#include <klocale.h>

#include <qradiobutton.h>
 
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
		p_currentAnswer.setQuestion(i18nc("@item:intable column Question", "%1", i18nc(p_map -> getFileName().toUtf8(), p_capital.toUtf8())));
		p_currentAnswer.setCorrectAnswer(i18nc("@option:radio <City> is the capital of...", "%1", i18nc(p_map -> getFileName().toUtf8(), division.toUtf8())));

		QString capitalName = i18nc(p_map -> getFileName().toUtf8(), p_capital.toUtf8());
		setQuestion(i18nc("@title:group", "%1 is the capital of...", capitalName));
		p_rb[i] -> setText(i18nc("@option:radio <City> is the capital of...", "%1", i18nc(p_map -> getFileName().toUtf8(), division.toUtf8())));
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
			p_rb[i] -> setText(i18nc("@option:radio <City> is the capital of...", "%1", i18nc(p_map -> getFileName().toUtf8(), division.toUtf8())));
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
	QString divisionType = i18nc(p_map -> getFileName().toUtf8(), p_map->getDivisionsString().toUtf8());
	return i18nc("@title", "%1 From Its Capital", divisionType);
}
