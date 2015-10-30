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
#include <klocalizedstring.h>
#include <krandom.h>

#include <qbuttongroup.h>
#include <qgroupbox.h>
#include <qevent.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qradiobutton.h>
#include <qpushbutton.h>

#include "map.h"
#include "settings.h"

static const int NB_CHOICES = 4;

boxAsker::boxAsker(QWidget *parent, KGmap *m, QWidget *w, uint count) : askWidget(parent, m, w, count)
{
	p_headWidget = NULL;
	p_lay = new QVBoxLayout(this);

	p_groupBox = new QGroupBox(this);
	p_groupLayout = new QGridLayout(p_groupBox);
	p_label = new QLabel(this);

	p_radioButtons.resize(NB_CHOICES);
	p_answerLabels.resize(NB_CHOICES);
	for(int i = 0; i < NB_CHOICES; i++)
	{
		p_answerLabels[i] = new QLabel(QString::number(i +1));
		p_radioButtons[i] = new QRadioButton(p_groupBox);

		p_radioButtons[i] -> installEventFilter(this);
		connect(p_radioButtons[i], &QAbstractButton::toggled, this, &boxAsker::atLeastOneSelected);
	}
	p_accept = new QPushButton();

	layoutGroupBox();
	layoutAligned();

	KAcceleratorManager::setNoAccel(this);

	p_groupBox -> setFocus();
}

boxAsker::~boxAsker()
{
	if ( p_accept->parent() == NULL )
		delete p_accept;
}

void boxAsker::updateLayout()
{
	layoutGroupBox();
	layoutAligned();
}

void boxAsker::layoutGroupBox()
{
	while ( p_groupLayout->takeAt(0) != NULL ) { }

	int horizAlignCode = kgeographySettings::self() -> questionPlacingScheme() % 3;
	Qt::Alignment horizAlignment = horizAlignCode == 0 ? Qt::AlignLeft : horizAlignCode == 1 ? Qt::AlignHCenter : Qt::AlignRight;
	p_groupLayout->setColumnStretch(0, horizAlignment == Qt::AlignLeft ? 0 : 1);
	p_groupLayout->setColumnStretch(3, horizAlignment == Qt::AlignRight ? 0 : 1);

	p_groupLayout -> setHorizontalSpacing(6);
	for(int i = 0; i < NB_CHOICES; i++)
	{
		p_answerLabels[i]->setAlignment(Qt::AlignRight);
		p_groupLayout -> addWidget(p_answerLabels[i], i, 0);
		p_groupLayout -> addWidget(p_radioButtons[i], i, 1);
	}
}

void boxAsker::layoutAligned()
{
	while ( p_lay->takeAt(0) != NULL ) { }

	if ( p_headWidget != NULL )
		p_lay->addWidget(p_headWidget);

	int horizAlignCode = kgeographySettings::self() -> questionPlacingScheme() % 3;
	Qt::Alignment horizAlignment = horizAlignCode == 0 ? Qt::AlignLeft : horizAlignCode == 1 ? Qt::AlignHCenter : Qt::AlignRight;
	int vertAlignCode = kgeographySettings::self() -> questionPlacingScheme() / 3 % 3;
	Qt::Alignment vertAlignment = vertAlignCode == 0 ? Qt::AlignTop : vertAlignCode == 1 ? Qt::AlignVCenter : Qt::AlignBottom;

	p_label -> setAlignment(horizAlignment);
	p_groupBox -> setAlignment(horizAlignment);

	p_lay -> addWidget(p_label);

	if ( vertAlignment != Qt::AlignTop ) {
		p_lay -> addStretch(1);
	}

	p_lay -> addWidget(p_groupBox, 0);

	if ( vertAlignment != Qt::AlignBottom ) {
		p_lay -> addStretch(1);
	}

	if ( kgeographySettings::self() -> waitsForValidation() ) {
		p_lay->addWidget(p_accept);
		p_accept->show();
	}
	else {
		if ( p_accept->isVisible() )
			checkAnswer();
		p_accept -> hide();
	}
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

void boxAsker::setQuestion(const QString &q)
{
	p_label -> setText(q);
}

void boxAsker::keyPressEvent(QKeyEvent *e)
{
	// we do this on press because it is done so for 0->1-2->3 and 3->2->1->0 movements
	// (those keys are subject to repeat, they have to be treated as press)
	if ( e -> key() == Qt::Key_Down && p_radioButtons[NB_CHOICES -1] -> hasFocus() )
	{
		if ( p_radioButtons[NB_CHOICES -1] -> isChecked() ) p_radioButtons[0] -> setChecked(true);
		p_radioButtons[0] -> setFocus();
	}
	else if ( e -> key() == Qt::Key_Up && p_radioButtons[0] -> hasFocus() )
	{
		if ( p_radioButtons[0] -> isChecked() ) p_radioButtons[NB_CHOICES -1] -> setChecked(true);
		p_radioButtons[NB_CHOICES -1] -> setFocus();
	}
}

void boxAsker::keyReleaseEvent(QKeyEvent *e)
{
	if (e -> key() == Qt::Key_Return || e -> key() == Qt::Key_Enter) checkAnswer();
	else if ( e -> key() >= Qt::Key_1 && e -> key() <= (Qt::Key_1 + NB_CHOICES -1) )
	{
		p_radioButtons[e -> key() - Qt::Key_1] -> setFocus();
		// we check the box after the focus because the check can trigger immediate destruction of the asker at last question
		p_radioButtons[e -> key() - Qt::Key_1] -> setChecked(true);
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

	for(int i = 0; i < NB_CHOICES; i++) p_radioButtons[i] -> setAutoExclusive(false);
	for(int i = 0; i < NB_CHOICES; i++) p_radioButtons[i] -> setChecked(false);
	for(int i = 0; i < NB_CHOICES; i++) p_radioButtons[i] -> setText(QString());
	for(int i = 0; i < NB_CHOICES; i++) p_radioButtons[i] -> setIcon(QIcon());
	for(int i = 0; i < NB_CHOICES; i++) p_radioButtons[i] -> setAutoExclusive(true);

	p_accept -> setEnabled(false);

	auxList << division;

	// we put the division in a random place
	p_position = (int)((float)NB_CHOICES * KRandom::random() / (RAND_MAX + 1.0));
	nextBoxAskerQuestionHook(division, p_position, true);

	// fill the other names
	j = 0;
	while (j < NB_CHOICES)
	{
		if (p_radioButtons[j] -> text().isNull() && p_radioButtons[j] -> icon().isNull())
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
		if (p_radioButtons[i] -> isChecked())
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
	connect(p_accept, &QPushButton::clicked, this, &boxAsker::checkAnswer);
}

void boxAsker::setHeadWidget(QWidget *headWidget)
{
	p_headWidget = headWidget;
	p_lay -> insertWidget(0, headWidget);
}


