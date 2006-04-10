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
		p_currentAnswer.setQuestion(i18nc(p_map -> getFileName().toUtf8(), p_capital.toUtf8()));
		p_currentAnswer.setCorrectAnswer(i18nc(p_map -> getFileName().toUtf8(), division.toUtf8()));

		QString sw = i18nc("There are two ways of dealing with the translation of \"%1 is the capital of...\". The first option simply replaces %1 with the translated name of the relevant city. If the grammar of your language allows this, choose this option by setting the translation of this message to 1, and leave untranslated the translations of \"%1 is the capital of...\" that have the placename embedded (or translate them as - if you wish to show the file as fully translated. The second option is to translate all messages in full - this is likely to be required in the case of highly-inflected languages like Russian. To choose this option, set the translation of this message to 0, and translate all the messages.", "0");
		if (sw == "1")
		{
			QString capitalName = i18nc(p_map -> getFileName().toUtf8(), p_capital.toUtf8());
			setQuestion(i18n("%1 is the capital of...", capitalName));
		}
		else
		{
			QString s = QString("%1 is the capital of...").arg(p_capital);
			setQuestion(i18nc(p_map -> getFileName().toUtf8(), s.toUtf8()));
		}
		p_rb[i] -> setText(i18nc(p_map -> getFileName().toUtf8(), division.toUtf8()));
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
			p_rb[i] -> setText(i18nc(p_map -> getFileName().toUtf8(), division.toUtf8()));
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
