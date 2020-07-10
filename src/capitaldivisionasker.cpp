/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "capitaldivisionasker.h"

#include <KLocalizedString>

#include <QRadioButton>
 
#include "map.h"

capitalDivisionAsker::capitalDivisionAsker(QWidget *parent, KGmap *m, QWidget *w, uint count) : boxAsker(parent, m, w, count)
{
	init();
}

bool capitalDivisionAsker::nextBoxAskerQuestionHook(const division *div, int i, bool isAnswer)
{
	bool b;
	if (isAnswer)
	{
		const QString divisionName = div -> getName();
		p_capital = div -> getCapital();
		p_currentAnswer.setQuestion(i18nc("@item:intable column Question, %1 is capital name", "%1", p_capital));
		p_currentAnswer.setCorrectAnswer(i18nc("@option:radio <City> is the capital of...", "%1", divisionName));

		setQuestion(p_map->getCapitalToDivisionQuestionPattern().subs(p_capital).toString());
		p_radioButtons[i] -> setText(i18nc("@option:radio <City> is the capital of...", "%1", divisionName));
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
		if (div -> getCapital() != p_capital)
		{
			const QString divisionName = div -> getName();
			p_radioButtons[i] -> setText(i18nc("@option:radio <City> is the capital of...", "%1", divisionName));
			b = true;
		}
		else b = false;
	}
	return b;
}

void capitalDivisionAsker::setAnswerHook(int userSays)
{
	p_currentAnswer.setAnswer(p_radioButtons[userSays] -> text());
}

QString capitalDivisionAsker::getQuestionHook() const
{
	return p_map->getCapitalToDivisionTitle();
}
