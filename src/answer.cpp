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

#include "answer.h"

userAnswer::userAnswer()
{
}

userAnswer::userAnswer(const userAnswer &r)
{
	p_question = r.p_question;
	p_answer = r.p_answer;
	p_correctAnswer = r.p_correctAnswer;
}

userAnswer &userAnswer::operator=(const userAnswer &r)
{
	p_question = r.p_question;
	p_answer = r.p_answer;
	p_correctAnswer = r.p_correctAnswer;
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

void userAnswer::setCorrectAnswer(QVariant correctAnswer)
{
	p_correctAnswer = correctAnswer;
}

void userAnswer::putWidgets(QWidget *w, QGridLayout *lay, int row) const
{
	QLabel *l1, *l2, *l3;
	
	l1 = new QLabel(w);
	if (p_question.type() == QVariant::String) l1 -> setText(p_question.toString());
	else l1 -> setPixmap(p_question.toImage());
		
	l2 = new QLabel(w);
	if (p_answer.type() == QVariant::String) l2 -> setText(p_answer.toString());
	else if (p_answer.type() == QVariant::Color) l2 -> setBackgroundColor(p_answer.toColor());
	else l2 -> setPixmap(p_answer.toImage());
	
	l3 = new QLabel(w);
	if (p_correctAnswer.type() == QVariant::String) l3 -> setText(p_correctAnswer.toString());
	else if (p_correctAnswer.type() == QVariant::Color) l3 -> setBackgroundColor(p_correctAnswer.toColor());
	else l3 -> setPixmap(p_correctAnswer.toImage());
	
	lay->addWidget(l1, row, 1);
	lay->addWidget(l2, row, 2);
	lay->addWidget(l3, row, 3);
}

