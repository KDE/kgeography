/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "divisioncapitalasker.h"

#include <klocale.h>
#include <krandom.h>

#include <qradiobutton.h>
 
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
		QString divisionName = i18nc(p_map -> getFileName().toUtf8(), division.toUtf8());
		setQuestion(i18nc("@title:group", p_map->getDivisionToCapitalQuestionPattern().toUtf8(), divisionName));
		p_currentAnswer.setQuestion(i18nc("@item:intable column Question, %1 is region name", "%1", i18nc(p_map -> getFileName().toUtf8(), division.toUtf8())));
		p_capital = p_map -> getDivisionCapital(division);
		p_currentAnswer.setCorrectAnswer(i18nc("@option:radio The capital of <Region> is...", "%1", i18nc(p_map -> getFileName().toUtf8(), p_capital.toUtf8())));
		p_radioButtons[i] -> setText(i18nc("@option:radio The capital of <Region> is...", "%1", i18nc(p_map -> getFileName().toUtf8(), p_capital.toUtf8())));
		b = true;
		
		QStringList falseCapitals = p_map -> getDivisionFalseCapitals(division);
		int index = 0;
		while (!falseCapitals.isEmpty() && index < 4)
		{
			if (index != i)
			{
				int random = (int)((float)falseCapitals.size() * KRandom::random() / (RAND_MAX + 1.0));
				QString falseCapital = falseCapitals.at(random);
				falseCapitals.removeAt(random);
				p_radioButtons[index] -> setText(i18nc("@option:radio The capital of <Region> is...", "%1", i18nc(p_map -> getFileName().toUtf8(), falseCapital.toUtf8())));
			}
			++index;
		}
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
			p_radioButtons[i] -> setText(i18nc("@option:radio The capital of <Region> is...", "%1", i18nc(p_map -> getFileName().toUtf8(), capital.toUtf8())));
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
	QString divisionType = i18nc(p_map -> getFileName().toUtf8(), p_map->getDivisionsString().toUtf8());
	return i18nc("@title", p_map->getDivisionToCapitalTitle().toUtf8());
}
