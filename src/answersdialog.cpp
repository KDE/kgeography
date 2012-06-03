/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "answersdialog.h"

#include <klocale.h>

#include <qlabel.h>
#include <qlayout.h>
#include <qscrollarea.h>

#include "answer.h"

answersDialog::answersDialog(QWidget *parent, const QVector<userAnswer> &userAnswers, const QString &question, int correctAnswers)
	: KDialog(parent)
{
	setCaption(i18n("Your Answers Were"));
	setButtons(KDialog::Ok);

	QLabel *l1, *l2, *l3;
	QFont boldFont, bigFont;
	uint totalAnswers;
	totalAnswers = userAnswers.count();
	
	p_sa = new QScrollArea(this);
	setMainWidget(p_sa);
	
	p_container = new QWidget(this);
	p_sa -> setWidget(p_container);
	p_sa -> setWidgetResizable(true);
	
	QGridLayout *lay = new QGridLayout(p_container);
	lay -> setSpacing(0);
	lay -> setColumnStretch(0, 1);
	lay -> setColumnStretch(4, 1);
	lay -> setRowStretch(totalAnswers + 4, 1);
	
	// Title
	bigFont = p_container -> font();
	bigFont.setPointSize(24);
	l1 = new QLabel(question, p_container);
	l1 -> setFont(bigFont);
	lay -> addWidget(l1, 0, 0, 1, 5, Qt::AlignHCenter);
	
	l1 = new QLabel(i18n("You answered correctly %1 out of %2 questions.", correctAnswers, totalAnswers), p_container);
	l1 -> setAlignment(Qt::AlignCenter);
	lay -> addWidget(l1, 1, 0, 1, 5);
	//lay -> addWidget(l1, totalAnswers + 4, 0, 1, 5);

	// Headers
	boldFont = p_container -> font();
	boldFont.setBold(true);
	
	l1 = new QLabel(i18n("Question"), p_container);
	l1 -> setFont(boldFont);
	l2 = new QLabel(i18n("Your Answer"), p_container);
	l2 -> setFont(boldFont);
	l3 = new QLabel(i18n("Correct Answer"), p_container);
	l3 -> setFont(boldFont);
	l1 -> setMargin(KDialog::marginHint() / 2);
	l2 -> setMargin(KDialog::marginHint() / 2);
	l3 -> setMargin(KDialog::marginHint() / 2);

	lay->addWidget(l1, 2, 1);
	lay->addWidget(l3, 2, 2);
	lay->addWidget(l2, 2, 3);
	
	for(uint i = 0; i < totalAnswers; i++)
	{
		userAnswers[i].putWidgets(p_container, lay, i + 3);
	}

	//lay -> addItem(new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Fixed), totalAnswers + 3, 2);

	resize(500, 500);
}

