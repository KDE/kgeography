/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "flagdivisionasker.h"

#include <klocale.h>

#include <qimage.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qradiobutton.h>

#include "map.h"

class flagWidget : public QWidget
{
	public:
		flagWidget(QWidget *parent) : QWidget(parent)
		{
		}

		QImage img;

	protected:
		void paintEvent(QPaintEvent *)
		{
			QPainter p(this);
			p.drawImage((width() - img.width()) / 2, 0, img);
		}
};

flagDivisionAsker::flagDivisionAsker(QWidget *parent, KGmap *m, QWidget *w, uint count) : boxAsker(parent, m, w, count)
{
	p_flag = new flagWidget(this);
	p_lay -> insertWidget(0, p_flag);
	setQuestion(i18n("This flag belongs to:"));
	init();
}

bool flagDivisionAsker::nextBoxAskerQuestionHook(const QString &division, int i, bool isAnswer)
{
	if (isAnswer)
	{
		// we put the flag image
		QImage image(p_map -> getDivisionFlagFile(division));
		p_flag -> img = image;
		p_flag -> setMinimumSize(image.size());
		p_flag -> update();
		
		p_currentAnswer.setQuestion(QPixmap::fromImage(image.scaled(image.width()/5, image.height()/5, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
		p_currentAnswer.setCorrectAnswer(i18nc(p_map -> getFileName().toUtf8(), division.toUtf8()));
	}
	else
	{
		// There are some maps like the Pacific one where two divisions have the same flag
		QImage image(p_map -> getDivisionFlagFile(division));
		if (p_flag -> img == image) return false;
	}
	p_rb[i] -> setText(i18nc(p_map -> getFileName().toUtf8(), division.toUtf8()));
	
	return true;
}

void flagDivisionAsker::setAnswerHook(int userSays)
{
	p_currentAnswer.setAnswer(p_rb[userSays] -> text());
}

QString flagDivisionAsker::getQuestionHook() const
{
	QString divisionType = i18nc(DIVTYPE_CONTEXT, p_map->getDivisionsString().toUtf8());
	return i18n("%1 From Their Flag", divisionType);
}
