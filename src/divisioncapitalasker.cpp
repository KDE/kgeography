/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "divisioncapitalasker.h"

#include <KLocalizedString>

#include <QRadioButton>
#include <QRandomGenerator>
 
#include "map.h"

divisionCapitalAsker::divisionCapitalAsker(QWidget *parent, KGmap *m, QWidget *w, uint count) : boxAsker(parent, m, w, count)
{
	init();
}

bool divisionCapitalAsker::nextBoxAskerQuestionHook(const division *div, int i, bool isAnswer)
{
	bool b;
	if (isAnswer)
	{
		const QString divisionName = div -> getName();
		setQuestion(p_map->getDivisionToCapitalQuestionPattern().subs(divisionName).toString());
		p_currentAnswer.setQuestion(i18nc("@item:intable column Question, %1 is region name", "%1", divisionName));
		p_capital = div -> getCapital();
		p_currentAnswer.setCorrectAnswer(i18nc("@option:radio The capital of <Region> is...", "%1", p_capital));
		p_radioButtons[i] -> setText(i18nc("@option:radio The capital of <Region> is...", "%1", p_capital));
		b = true;
		
		QStringList falseCapitals = div -> getFalseCapitals();
		int index = 0;
		while (!falseCapitals.isEmpty() && index < 4)
		{
			if (index != i)
			{
				const int random = QRandomGenerator::global()->bounded(falseCapitals.size());
				QString falseCapital = falseCapitals.at(random);
				falseCapitals.removeAt(random);
				p_radioButtons[index] -> setText(i18nc("@option:radio The capital of <Region> is...", "%1", falseCapital));
			}
			++index;
		}
	}
	else
	{
		QString capital;
		capital = div -> getCapital();
		// There are crazy countries like Norway where two different divisions
		// have the same capital, avoid asking
		// The capital of Akershus is ...
		// Oslo (true)
		// Oslo (true)
		// Lillehammer
		// Moss
		if (capital != p_capital)
		{
			p_radioButtons[i] -> setText(i18nc("@option:radio The capital of <Region> is...", "%1", capital));
			b = true;
		}
		else b = false;
	}
	return b;
}

void divisionCapitalAsker::setAnswerHook(int userSays)
{
	p_currentAnswer.setAnswer(p_radioButtons[userSays] -> text());
}

QString divisionCapitalAsker::getQuestionHook() const
{
	return p_map->getDivisionToCapitalTitle();
}
