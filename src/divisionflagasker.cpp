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
		QString sw = i18n("If your language is able of using a translation of The flag of %1 is... where %1 is the name of the current division, please translate this message to 1, if not translate it to 0. If you translate this message to 0 you will need to translate all the The flag of DivisionNameHere is... messages, if you translate this message to 1 this messages will not be used so you can translate them to - if you want, that way statistics will not hurt you", "0");
		if (sw == "1")
		{
			QString divisionName = i18n(p_map -> getFileName().toUtf8(), division.toUtf8());
			QString text = i18n("The flag of %1 is...");
			setQuestion(text.arg(divisionName));
		}
		else
		{
			QString s = QString("The flag of %1 is...").arg(division);
			setQuestion(i18n(p_map -> getFileName().toUtf8(), s.toUtf8()));
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
	return i18n("Flag of Division");
}
