/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <qimage.h>
#include <qlabel.h>
#include <qlayout.h>

#include <kdialog.h>

#include "answer.h"

userAnswer::userAnswer()
{
}

userAnswer::userAnswer(const userAnswer &r)
{
	p_question = r.p_question;
	p_answer = r.p_answer;
	p_correctAnswer = r.p_correctAnswer;
	p_correct = r.p_correct;
}

userAnswer &userAnswer::operator=(const userAnswer &r)
{
	p_question = r.p_question;
	p_answer = r.p_answer;
	p_correctAnswer = r.p_correctAnswer;
	p_correct = r.p_correct;
	return *this;
}

void userAnswer::setQuestion(QVariant question)
{
	p_question = question;
}

void userAnswer::setAnswer(QVariant answer)
{
	p_answer = answer;
}

void userAnswer::setAnswerCorrect(bool correct)
{
	p_correct = correct;
}

void userAnswer::setCorrectAnswer(QVariant correctAnswer)
{
	p_correctAnswer = correctAnswer;
}

void userAnswer::putWidgets(QWidget *w, QGridLayout *lay, int row) const
{
	QWidget *widgets[3];
	const QVariant *v;
	
	for (int i = 0; i < 3; i++)
	{
		if (i == 0) v = &p_question;
		else if (i == 1) v = &p_answer;
		else v = &p_correctAnswer;
		
		if (v -> type() == QVariant::String)
		{
			QLabel *l;
			l = new QLabel(w);
			l -> setText(v -> toString());
			l -> setMargin(KDialog::marginHint() / 2);
			widgets[i] = l;
		}
		else if (v -> type() == QVariant::Color)
		{
			QWidget *aux = new QWidget(w);
			QHBoxLayout *lay = new QHBoxLayout(aux);
			
			QFrame *inner = new QFrame(aux);
			lay -> addWidget(inner);
			inner -> setBackgroundColor(v -> toColor());
			inner -> setLineWidth(1);
			lay -> setMargin(KDialog::marginHint() / 2);
			widgets[i] = aux;
		}
		else if (v -> type() == QVariant::Image)
		{
			QLabel *l;
			l = new QLabel(w);
			l -> setPixmap(v -> toImage());
			l -> setAlignment(Qt::AlignHCenter);
			l -> setMargin(KDialog::marginHint() / 2);
			widgets[i] = l;
		}
		
		lay -> addWidget(widgets[i], row, i + 1);
	}
	
	if (!p_correct)
	{
		QColor back, fore;
		back = widgets[0] -> colorGroup().highlight();
		fore = widgets[0] -> colorGroup().highlightedText();
		for (int i = 0; i < 3; i++)
		{
			widgets[i] -> setPaletteBackgroundColor(back);
			widgets[i] -> setPaletteForegroundColor(fore);
		}
	}
}

