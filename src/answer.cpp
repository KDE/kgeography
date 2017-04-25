/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "answer.h"

#include <qimage.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpixmap.h>

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

void userAnswer::setQuestion(const QVariant &question)
{
	p_question = question;
}

void userAnswer::setAnswer(const QVariant &answer)
{
	p_answer = answer;
}

void userAnswer::setAnswerCorrect(bool correct)
{
	p_correct = correct;
}

void userAnswer::setCorrectAnswer(const QVariant &correctAnswer)
{
	p_correctAnswer = correctAnswer;
}

void userAnswer::putWidgets(QWidget *w, QGridLayout *lay, int row, int margin) const
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
			l -> setMargin(margin);
			widgets[i] = l;
		}
		else if (v -> type() == QVariant::Color)
		{
			QWidget *aux = new QWidget(w);
			QHBoxLayout *lay = new QHBoxLayout(aux);
			
			QFrame *inner = new QFrame(aux);
			lay -> addWidget(inner);
			inner -> setPalette(QPalette(v -> value<QColor>()));
			inner -> setAutoFillBackground(true);
			lay -> setMargin(2);
			lay -> setSpacing(2);
			widgets[i] = aux;
		}
		else if (v -> type() == QVariant::List)
		{
			QVariantList vl = v->value<QVariantList>();
			QVariant vi;
			QWidget *aux = new QWidget(w);
			QHBoxLayout *lay = new QHBoxLayout(aux);

			QLabel *coloredLabel = new QLabel(aux);
			lay -> addWidget(coloredLabel);
			coloredLabel-> setAutoFillBackground(true);
			coloredLabel-> setMargin(margin);
			lay -> setMargin(2);
			lay -> setSpacing(2);

			for (int i=0; i<vl.count(); i++)
			{
				vi = vl[i];
				if(vi.type() == QVariant::Color)
				{
					coloredLabel -> setPalette(QPalette(vi.value<QColor>()));
				}
				if(vi.type() == QVariant::String)
				{
					coloredLabel -> setText(vi.value<QString>());
				}
			}
			widgets[i] = aux;
        }
		else if (v -> type() == QVariant::Pixmap)
		{
			QLabel *l;
			l = new QLabel(w);
			l -> setPixmap(v -> value<QPixmap>());
			l -> setAlignment(Qt::AlignHCenter);
			l -> setMargin(margin);
			widgets[i] = l;
		}
		
		lay -> addWidget(widgets[i], row, i + 1);
	}

	if (!p_correct)
	{
		// Calc the background color
		// using widgets[i] -> setBackgroundRole(QPalette::Highlight);
		// does not work because "overwrites" the color when v -> type() == QVariant::Color
		widgets[0]->show();
		widgets[0]->setBackgroundRole(QPalette::Highlight);
		QColor c = widgets[0]->palette().highlight().color();
		widgets[0]->setBackgroundRole(QPalette::NoRole);
	
		for (int i = 0; i < 3; i++)
		{
			widgets[i] -> setForegroundRole(QPalette::HighlightedText);
			widgets[i] -> setPalette( c );
			widgets[i] -> setAutoFillBackground(true);
		}
	}
}

