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
#include <kpushbutton.h>

#include <qlabel.h>
#include <qlayout.h>
#include <qradiobutton.h>
#include <qvbuttongroup.h>

#include "boxasker.h"
#include "map.h"

boxAsker::boxAsker(QWidget *parent, map *m, QWidget *w, uint count) : askWidget(parent, m, w, count)
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

boxAsker::~boxAsker()
{
	delete[] p_rb;
}

void boxAsker::setQuestion(QString q)
{
	p_label -> setText(q);
}

void boxAsker::clean()
{
	for (int i = 0; i < 4; i++) p_rb[i] -> setText("");
	cleanHook();
		
	p_accept -> setText("&Restart");
	p_accept -> disconnect();
	connect(p_accept, SIGNAL(clicked()), this, SLOT(init()));
		
	setQuestion("");
}

void boxAsker::cleanHook()
{
}

void boxAsker::nextQuestionHook(QString division)
{
	QStringList auxList;
	int i;
	
	for (i = 0; i < 4; i++) p_rb[i] -> setChecked(false);
	
	auxList << division;
		
	// we put the division in a random place
	p_position = (int)((float)4 * kapp -> random() / (RAND_MAX + 1.0));
	nextBoxAskerQuestionHook(division, p_position, true);
		
	// we put other 3 names
	for (i = 0; i < 4; i++)
	{
		division = p_map -> getRandomDivision();
		while (auxList.find(division) != auxList.end()) division = p_map -> getRandomDivision();
		if (i == p_position) i++;
		nextBoxAskerQuestionHook(division, i, false);
		auxList << division;
		if (p_position == 3 && i == 2) i++;
	}
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
		questionAnswered(correct);
		nextQuestion();
	}
}

void boxAsker::init()
{
	p_accept -> setText(i18n("&Accept"));

	resetAnswers();
	clearAsked();
	nextQuestion();
	
	p_accept -> disconnect();
	connect(p_accept, SIGNAL(clicked()), this, SLOT(checkAnswer()));
}

#include "boxasker.moc"
