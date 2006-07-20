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

#include <qlabel.h>
#include <qlayout.h>
#include <qscrollarea.h>

#include "answer.h"
#include "answersdialog.h"

answersDialog::answersDialog(QWidget *parent, const QVector<userAnswer> &userAnswers, QString question, int correctAnswers) : KDialog(parent)
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
	lay->addWidget(l1, 1, 1);
	lay->addWidget(l2, 1, 2);
	lay->addWidget(l3, 1, 3);
	
	for(uint i = 0; i < totalAnswers; i++)
	{
		userAnswers[i].putWidgets(p_container, lay, i + 2);
	}

	lay -> addItem(new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Fixed), totalAnswers + 3, 2);
	
	l1 = new QLabel(i18n("You answered correctly %1 out of %2 questions.", correctAnswers, totalAnswers), p_container);
	l1 -> setAlignment(Qt::AlignCenter);
	lay -> addWidget(l1, totalAnswers + 4, 0, 1, 5);
	
	resize(500, 500);
}

