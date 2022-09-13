/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "divisionflagasker.h"

#include <KLocalizedString>

#include <QImage>
#include <QRadioButton>
 
#include "map.h"

divisionFlagAsker::divisionFlagAsker(QWidget *parent, KGmap *m, QWidget *w, uint count) : boxAsker(parent, m, w, count)
{
	init();
}

bool divisionFlagAsker::nextBoxAskerQuestionHook(const division *div, int i, bool isAnswer)
{
	QSize pixmapSize(60, 40);
	QIcon icon(div -> getFlagForQuestion());

	if (isAnswer)
	{
		const QString divisionName = div -> getName();
		setQuestion(i18nc("@title:group", "The flag of %1 is...", divisionName));
		p_currentAnswer.setCorrectAnswer(icon.pixmap(QSize(60, 40)));
		p_currentAnswer.setQuestion(i18nc("@item:intable column Question, %1 is region name", "%1", divisionName));
	}
	else
	{
		// There are some maps like the Pacific one where two divisions have the same flag
		if (icon.pixmap(pixmapSize).toImage() == p_radioButtons[p_position]->icon().pixmap(pixmapSize).toImage()) return false;
	}
	p_radioButtons[i] -> setIcon(icon);
	p_radioButtons[i] -> show();
	p_radioButtons[i] -> setIconSize(pixmapSize);
	return true;
}

void divisionFlagAsker::setAnswerHook(int userSays)
{
	QIcon i = p_radioButtons[userSays] -> icon();
	p_currentAnswer.setAnswer(i.pixmap(QSize(60, 40)));
}

QString divisionFlagAsker::getQuestionHook() const
{
	QString divisionType = p_map->getDivisionsString();
	return i18nc("@title", "Flag of %1", divisionType);
}
