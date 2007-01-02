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

#include <qimage.h>
#include <qradiobutton.h>
 
#include "divisionflagasker.h"
#include "map.h"

divisionFlagAsker::divisionFlagAsker(QWidget *parent, KGmap *m, QWidget *w, uint count) : boxAsker(parent, m, w, count)
{
	init();
}

bool divisionFlagAsker::nextBoxAskerQuestionHook(const QString &division, int i, bool isAnswer)
{
	QIcon icon(p_map -> getDivisionFlagFile(division));
	p_rb[i] -> setIcon(icon);
	p_rb[i] -> show();
	p_rb[i] -> setIconSize(QSize(60, 40));
	if (isAnswer)
	{
		QString sw = i18nc("There are two ways of dealing with the translation of \"The flag of %1 is...\". The first option simply replaces %1 with the translated name of the relevant region. If the grammar of your language allows this, choose this option by setting the translation of this message to 1, and leave untranslated the translations of \"The flag of %1 is...\" that have the placename embedded (or translate them as - if you wish to show the file as fully translated. The second option is to translate all messages in full - this is likely to be required in the case of highly-inflected languages like Russian. To choose this option, set the translation of this message to 0, and translate all the messages.", "0");
		if (sw == "1")
		{
			QString divisionName = i18nc(p_map -> getFileName().toUtf8(), division.toUtf8());
			setQuestion(i18n("The flag of %1 is...", divisionName));
		}
		else
		{
			QString s = QString("The flag of %1 is...").arg(division);
			setQuestion(i18nc(p_map -> getFileName().toUtf8(), s.toUtf8()));
		}
		p_currentAnswer.setCorrectAnswer(icon.pixmap(QSize(60, 40)));
		p_currentAnswer.setQuestion(division);
	}
	return true;
}

void divisionFlagAsker::setAnswerHook(int userSays)
{
	QIcon i = p_rb[userSays] -> icon();
	p_currentAnswer.setAnswer(i.pixmap(QSize(60, 40)));
}

QString divisionFlagAsker::getQuestionHook() const
{
	return i18n("Flag of %1", p_map->getDivisionsString());
}
