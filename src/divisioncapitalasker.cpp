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
		QString sw = i18n("There are two ways of dealing with the translation of \"The capital of %1 is...\". The first option simply replaces %1 with the translated name of the relevant region. If the grammar of your language allows this, choose this option by setting the translation of this message to 1, and leave untranslated the translations of \"The capital of %1 is...\" that have the placename embedded (or translate them as - if you wish to show the file as fully translated. The second option is to translate all messages in full - this is likely to be required in the case of highly-inflected languages like Russian. To choose this option, set the translation of this message to 0, and translate all the messages.", "0");
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
