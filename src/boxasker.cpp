/***************************************************************************
 *   Copyright (C) 2004-2007 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "boxasker.h"

#include <stdlib.h> // for RAND_MAX

#include <kacceleratormanager.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <krandom.h>

#include <qbuttongroup.h>
#include <qgroupbox.h>
#include <qevent.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qradiobutton.h>

#include "map.h"
#include "settings.h"

static const int NB_CHOICES = 4;

boxAsker::boxAsker(QWidget *parent, KGmap *m, QWidget *w, uint count) : askWidget(parent, m, w, count)
{
	QVBoxLayout *vbl = static_cast<QVBoxLayout*>(w -> layout());
	vbl -> addStretch(1);
	p_lay = new QVBoxLayout(this);

	QGroupBox *bg = new QGroupBox(this);
	p_label = new QLabel(this);
	p_rb = new QRadioButton*[NB_CHOICES];
	QLabel ** labels = new QLabel*[NB_CHOICES];
	for(int i = 0; i < NB_CHOICES; i++)
	{
		labels[i] = new QLabel(QString::number(i +1), bg);
		p_rb[i] = new QRadioButton(bg);

		p_rb[i] -> installEventFilter(this);
		connect(p_rb[i], SIGNAL(toggled(bool)), this, SLOT(atLeastOneSelected()));
	}
	p_accept = new KPushButton();

	if ( kgeographySettings::self() -> questionPlacingScheme() < 1 )
		layoutTop(bg, labels);
	else
		layoutCentered(bg, labels);

	KAcceleratorManager::setNoAccel(this);

	bg -> setFocus();
	delete labels;
}


void boxAsker::layoutCentered(QGroupBox *bg, QLabel ** labels)
{
	QHBoxLayout *centeringLayout = new QHBoxLayout(bg);
	QGridLayout *gridLayout = new QGridLayout();
	gridLayout -> setHorizontalSpacing(6);
	p_label -> setAlignment(Qt::AlignHCenter);
	for(int i = 0; i < NB_CHOICES; i++)
	{
		gridLayout -> addWidget(labels[i], i, 0);
		gridLayout -> addWidget(p_rb[i], i, 1);
	}

	centeringLayout -> addStretch(1);
	centeringLayout -> addItem(gridLayout);
	centeringLayout -> addStretch(1);

	while ( p_lay->takeAt(0) != NULL ) { }
	p_lay -> addStretch(1);
	p_lay -> addWidget(p_label);
	p_lay -> addWidget(bg, 1);
	p_lay -> addStretch(1);

	if ( kgeographySettings::self() -> waitsForValidation() )
		p_lay -> addWidget(p_accept);
}

void boxAsker::layoutTop(QGroupBox *bg, QLabel ** labels)
{
	//QVBoxLayout *gbLayout = new QVBoxLayout(bg);
	QHBoxLayout *centeringLayout = new QHBoxLayout(bg);
	QGridLayout *gridLayout = new QGridLayout();
	for(int i = 0; i < NB_CHOICES; i++)
	{
		gridLayout -> addWidget(labels[i], i, 0);
		gridLayout -> addWidget(p_rb[i], i, 1);
		gridLayout -> addWidget(p_rb[i], i, 1);
	}
	//gbLayout->addStretch(1);
	centeringLayout -> addItem(gridLayout);
	centeringLayout -> addStretch(1);

	while ( p_lay->takeAt(0) != NULL ) { }
	p_lay -> addWidget(p_label);
	p_lay -> addWidget(bg);
	p_lay -> addStretch(1);

	if ( kgeographySettings::self() -> waitsForValidation() )
		p_lay -> addWidget(p_accept);
}

bool boxAsker::eventFilter(QObject *obj, QEvent *event)
{
	if ( kgeographySettings::self() -> focusFollowsMouse() && event -> type() == QEvent::Enter) {
		if (obj == p_accept)
			p_accept -> setFocus();
		else
			((QRadioButton*)obj) -> setFocus();
		return true;
	} else {
		// pass the event on to the parent class
		return QWidget::eventFilter(obj, event);
	}
}

boxAsker::~boxAsker()
{
	if ( p_accept->parent() == NULL )
		delete p_accept;
	delete[] p_rb;
}

void boxAsker::setQuestion(const QString &q)
{
	p_label -> setText(q);
}

void boxAsker::keyPressEvent(QKeyEvent *e)
{
	// we do this on press because it is done so for 0->1-2->3 and 3->2->1->0 movements
	// (those keys are subject to repeat, they have to be treated as press)
	if ( e -> key() == Qt::Key_Down && p_rb[NB_CHOICES -1] -> hasFocus() )
	{
		if ( p_rb[NB_CHOICES -1] -> isChecked() ) p_rb[0] -> setChecked(true);
		p_rb[0] -> setFocus();
	}
	else if ( e -> key() == Qt::Key_Up && p_rb[0] -> hasFocus() )
	{
		if ( p_rb[0] -> isChecked() ) p_rb[NB_CHOICES -1] -> setChecked(true);
		p_rb[NB_CHOICES -1] -> setFocus();
	}
}

void boxAsker::keyReleaseEvent(QKeyEvent *e)
{
	if (e -> key() == Qt::Key_Return || e -> key() == Qt::Key_Enter) checkAnswer();
	else if ( e -> key() >= Qt::Key_1 && e -> key() <= (Qt::Key_1 + NB_CHOICES -1) )
	{
		p_rb[e -> key() - Qt::Key_1] -> setFocus();
		// we check the box after the focus because the check can trigger immediate destruction of the asker at last question
		p_rb[e -> key() - Qt::Key_1] -> setChecked(true);
		// next line triggered by previous, no need to go this way, crashes at end.
		//if ( ! kgeographySettings::self() -> waitsForValidation() )			checkAnswer();
	}
	else askWidget::keyReleaseEvent(e);
}

void boxAsker::nextQuestionHook(const QString &division)
{
	QString otherDivision;
	QStringList auxList;
	int j;

	for(int i = 0; i < NB_CHOICES; i++) p_rb[i] -> setAutoExclusive(false);
	for(int i = 0; i < NB_CHOICES; i++) p_rb[i] -> setChecked(false);
	for(int i = 0; i < NB_CHOICES; i++) p_rb[i] -> setText(QString());
	for(int i = 0; i < NB_CHOICES; i++) p_rb[i] -> setIcon(QIcon());
	for(int i = 0; i < NB_CHOICES; i++) p_rb[i] -> setAutoExclusive(true);

	p_accept -> setEnabled(false);

	auxList << division;

	// we put the division in a random place
	p_position = (int)((float)NB_CHOICES * KRandom::random() / (RAND_MAX + 1.0));
	nextBoxAskerQuestionHook(division, p_position, true);

	// fill the other names
	j = 0;
	while (j < NB_CHOICES)
	{
		if (p_rb[j] -> text().isNull() && p_rb[j] -> icon().isNull())
		{
			otherDivision = p_map -> getRandomDivision(askMode());
			while (auxList.contains(otherDivision))
				otherDivision = p_map -> getRandomDivision(askMode());
			if (nextBoxAskerQuestionHook(otherDivision, j, false))
				++j;
			auxList << otherDivision;
		}
		else ++j;
	}
}

void boxAsker::atLeastOneSelected()
{
	if ( ! kgeographySettings::self() -> waitsForValidation() )
		checkAnswer();
	else
		p_accept -> setEnabled(true);
}

void boxAsker::checkAnswer()
{
	bool any, correct;
	int i;

	correct = false;
	any = false;
	i = 0;
	while(!any && i < NB_CHOICES)
	{
		if (p_rb[i] -> isChecked())
		{
			any = true;
			correct = (i == p_position);
		}
		else i++;
	}

	if (any)
	{
		setAnswerHook(i);
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

void boxAsker::setHeadWidget(QWidget *headWidget)
{
	p_lay -> insertWidget(0, headWidget);
}

#include "boxasker.moc"
