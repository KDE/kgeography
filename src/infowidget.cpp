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

#include <qlabel.h>

#include "infowidget.h"

infoWidget::infoWidget(QWidget *parent) : QHBox(parent)
{
	p_name = new QLabel(this);
	p_name -> setAlignment(Qt::AlignCenter);
	p_next = 0;
	p_guesses = 0;
}

void infoWidget::setQuestionMode(bool b)
{
	if (!p_next && b)
	{
		p_next = new QLabel(this);
		p_next -> setAlignment(Qt::AlignCenter);
		p_guesses = new QLabel(this);
		p_guesses -> setAlignment(Qt::AlignCenter);
		p_next -> show();
		p_guesses -> show();
		p_total = 0;
		p_correct = 0;
		p_guesses -> setText(i18n("%1/%2").arg(p_correct).arg(p_total));
	}
	else
	{
		delete p_next;
		delete p_guesses;
		p_next = 0;
		p_guesses = 0;
	}
}

void infoWidget::setName(QString text)
{
	p_name -> setText(text);
}

void infoWidget::setNext(QString text)
{
	p_next -> setText(text);
}

void infoWidget::addResult(bool correct)
{
	p_total++;
	if (correct) p_correct++;
	p_guesses -> setText(i18n("%1/%2").arg(p_correct).arg(p_total));
}

int infoWidget::getCorrect() const
{
	return p_correct;
}

int infoWidget::getTotal() const
{
	return p_correct;
}
