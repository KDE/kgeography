/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <klocale.h>
#include <kmessagebox.h>

#include <qlabel.h>

#include "askwidget.h"
#include "map.h"

askWidget::askWidget(QWidget *parent, KGmap *m, QWidget *w, uint count, bool showLabel) : QWidget(parent), p_map(m), p_count(count)
{
	if (showLabel)
	{
		p_answers = new QLabel(w);
		p_answers -> setAlignment(AlignTop | AlignHCenter);
		resetAnswers();
		p_answers -> show();
	}
	else p_answers = 0;
}

askWidget::~askWidget()
{
	delete p_answers;
}

int askWidget::correctAnswers() const
{
	return p_correctAnswers;
}

QValueVector<userAnswer> askWidget::userAnswers() const
{
	return p_userAnswers;
}

void askWidget::setMovement(bool)
{
}

void askWidget::setZoom(bool)
{
}

void askWidget::setOriginalZoom()
{
}

void askWidget::clearAsked()
{
	p_asked.clear();
}

QString askWidget::lastDivisionAsked()
{
	return p_asked.last();
}

void askWidget::nextQuestion()
{
	QString aux;
	
	if (p_asked.count() < p_count)
	{
		aux = p_map -> getRandomDivision(isClickOnDivision());
		while (p_asked.find(aux) != p_asked.end()) aux = p_map -> getRandomDivision(isClickOnDivision());
		p_asked << aux;
		nextQuestionHook(aux);
	}
	else emit questionsEnded();
}

void askWidget::questionAnswered(bool wasCorrect)
{
	p_currentAnswer.setAnswerCorrect(wasCorrect);
	p_userAnswers.append(p_currentAnswer);
	
	if (wasCorrect) p_correctAnswers++;
	else p_incorrectAnswers++;
	updateLabel();
}

void askWidget::resetAnswers()
{
	p_correctAnswers = 0;
	p_incorrectAnswers = 0;
	updateLabel();
}

void askWidget::updateLabel()
{
	p_answers -> setText(i18n("Correct answers: %1/%2").arg(p_correctAnswers).arg(p_correctAnswers + p_incorrectAnswers));
}

#include "askwidget.moc"
