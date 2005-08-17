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
		QString sw = i18n("If your language is able of using a translation of The capital of %1 is... where %1 is the name of the current division, please translate this message to 1, if not translate it to 0. If you translate this message to 0 you will need to translate all the The capital of DivisionNameHere is... messages, if you translate this message to 1 this messages will not be used so you can translate them to - if you want, that way statistics will not hurt you", "0");
		if (sw == "1")
		{
			QString divisionName = i18n(p_map -> getFileName().utf8(), division.utf8());
			QString text = i18n("The capital of %1 is...");
			setQuestion(text.arg(divisionName));
		}
		else
		{
			QString s = QString("The capital of %1 is...").arg(division);
			setQuestion(i18n(p_map -> getFileName().utf8(), s.utf8()));
		}
		p_currentAnswer.setQuestion(i18n(p_map -> getFileName().utf8(), division.utf8()));
		p_capital = p_map -> getDivisionCapital(division);
		p_currentAnswer.setCorrectAnswer(i18n(p_map -> getFileName().utf8(), p_capital.utf8()));
		p_rb[i] -> setText(i18n(p_map -> getFileName().utf8(), p_capital.utf8()));
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
			p_rb[i] -> setText(i18n(p_map -> getFileName().utf8(), capital.utf8()));
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
	return i18n("Capital of Division");
}
