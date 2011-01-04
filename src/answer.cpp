/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <tqimage.h>
#include <tqlabel.h>
#include <tqlayout.h>

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

void userAnswer::setQuestion(TQVariant question)
{
	p_question = question;
}

void userAnswer::setAnswer(TQVariant answer)
{
	p_answer = answer;
}

void userAnswer::setAnswerCorrect(bool correct)
{
	p_correct = correct;
}

void userAnswer::setCorrectAnswer(TQVariant correctAnswer)
{
	p_correctAnswer = correctAnswer;
}

void userAnswer::putWidgets(TQWidget *w, TQGridLayout *lay, int row) const
{
	TQWidget *widgets[3];
	const TQVariant *v;
	
	for (int i = 0; i < 3; i++)
	{
		if (i == 0) v = &p_question;
		else if (i == 1) v = &p_answer;
		else v = &p_correctAnswer;
		
		if (v -> type() == TQVariant::String)
		{
			TQLabel *l;
			l = new TQLabel(w);
			l -> setText(v -> toString());
			l -> setMargin(KDialog::marginHint() / 2);
			widgets[i] = l;
		}
		else if (v -> type() == TQVariant::Color)
		{
			TQWidget *aux = new TQWidget(w);
			TQHBoxLayout *lay = new TQHBoxLayout(aux);
			
			TQFrame *inner = new TQFrame(aux);
			lay -> addWidget(inner);
			inner -> setBackgroundColor(v -> toColor());
			inner -> setLineWidth(1);
			lay -> setMargin(KDialog::marginHint() / 2);
			widgets[i] = aux;
		}
		else if (v -> type() == TQVariant::Image)
		{
			TQLabel *l;
			l = new TQLabel(w);
			l -> setPixmap(v -> toImage());
			l -> tqsetAlignment(Qt::AlignHCenter);
			l -> setMargin(KDialog::marginHint() / 2);
			widgets[i] = l;
		}
		
		lay -> addWidget(widgets[i], row, i + 1);
	}
	
	if (!p_correct)
	{
		TQColor back, fore;
		back = widgets[0] -> tqcolorGroup().highlight();
		fore = widgets[0] -> tqcolorGroup().highlightedText();
		for (int i = 0; i < 3; i++)
		{
			widgets[i] -> setPaletteBackgroundColor(back);
			widgets[i] -> setPaletteForegroundColor(fore);
		}
	}
}

