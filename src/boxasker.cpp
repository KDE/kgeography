/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <stdlib.h> // for RAND_MAX

#include <kapplication.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpushbutton.h>

#include <qlabel.h>
#include <qlayout.h>
#include <qradiobutton.h>
#include <qvbuttongroup.h>

#include "boxasker.h"
#include "map.h"

boxAsker::boxAsker(QWidget *parent, map *m, uint count) : askWidget(parent, m, count)
{
	p_lay = new QVBoxLayout(this);
	
	QVButtonGroup *bg = new QVButtonGroup(this);
	p_label = new QLabel(this);
	p_rb = new QRadioButton*[4];
	for(int i = 0; i < 4; i++)
	{
		p_rb[i] = new QRadioButton(bg);
	}
	p_accept = new KPushButton(this);

	
	p_lay -> addWidget( p_label);
	p_lay -> addWidget(bg, 1);
	p_lay -> addWidget(p_accept);
}

void boxAsker::setQuestion(QString q)
{
	p_label -> setText(q);
}

void boxAsker::nextQuestion()
{
	QStringList auxList;
	QString aux;
	int i;
	
	for (i = 0; i < 4; i++) p_rb[i] -> setChecked(false);
	
	if (p_asked.count() < p_count)
	{
		// aux is the division we ask for
		aux = p_map -> getRandomDivision();
		while (p_asked.find(aux) != p_asked.end()) aux = p_map -> getRandomDivision();
		p_asked << aux;
		auxList << aux;
		
		// we put the division in a random place
		p_position = (int)((float)4 * kapp -> random() / (RAND_MAX + 1.0));
		nextQuestionHook(aux, p_position, true);
		
		// we put other 3 names
		for (i = 0; i < 4; i++)
		{
			aux = p_map -> getRandomDivision();
			while (auxList.find(aux) != auxList.end()) aux = p_map -> getRandomDivision();
			if (i == p_position) i++;
			nextQuestionHook(aux, i, false);
			auxList << aux;
			if (p_position ==3 && i == 2) i++;
		}
	}
	else
	{
		for (i = 0; i < 4; i++) p_rb[i] -> setText("");
		
		p_accept -> setText("&Restart");
		p_accept -> disconnect();
		connect(p_accept, SIGNAL(clicked()), this, SLOT(init()));
		
		showAnswersMessageBox();
	}
}

void boxAsker::goToMenu()
{
	showAnswersMessageBox();
	emit finished();
}

void boxAsker::checkAnswer()
{
	bool any, correct;

	correct = false;	
	any = false;
	for (int i = 0; i < 4; i++)
	{
		if (p_rb[i] -> isChecked())
		{
			any = true;
			correct = (i == p_position);
		}
	}
		
	if (any)
	{
		if (correct) p_correctAnswers++;
		else p_incorrectAnswers++;
		nextQuestion();
	}
}

void boxAsker::init()
{
	p_accept -> setText(i18n("&Accept"));

	p_correctAnswers = 0;
	p_incorrectAnswers = 0;
	p_mustShowCorrectIncorrect = true;
	p_asked.clear();
	nextQuestion();
	
	p_accept -> disconnect();
	connect(p_accept, SIGNAL(clicked()), this, SLOT(checkAnswer()));
}

void boxAsker::showAnswersMessageBox()
{
	if (p_mustShowCorrectIncorrect)
	{
		KMessageBox::information(this, i18n("You have answered correctly %1 of %2 questions").arg(p_correctAnswers).arg(p_correctAnswers + p_incorrectAnswers));
		p_mustShowCorrectIncorrect = false;
		setQuestion("");
	}
}

#include "boxasker.moc"
