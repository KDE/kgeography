/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
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

#include "answer.h"
#include "answersdialog.h"

answersDialog::answersDialog(QWidget *parent, const QValueVector<userAnswer> &userAnswers, QString question, int correctAnswers) : KDialogBase(parent, 0, true, i18n("Your answers were"), Ok)
{
	QLabel *l1, *l2, *l3;
	QWidget *container;
	QFont boldFont, bigFont;
	uint totalAnswers;
	totalAnswers = userAnswers.count();
	
	container = new QWidget(this/*viewport()*/);
	setMainWidget(container);
	
	QGridLayout *lay = new QGridLayout(container, totalAnswers + 4, 3);
	lay -> setSpacing(KDialog::spacingHint());
	lay -> setColStretch(0, 1);
	lay -> setColStretch(4, 1);
	lay -> setRowStretch(totalAnswers + 4, 1);
	
	// Title
	bigFont = container -> font();
	bigFont.setPointSize(24);
	l1 = new QLabel(question, container);
	l1 -> setFont(bigFont);
	l1 -> setAlignment(Qt::AlignCenter);
	lay->addMultiCellWidget(l1, 0, 0, 0, 4);
	
	// Headers
	boldFont = container -> font();
	boldFont.setBold(true);
	
	l1 = new QLabel(i18n("Question"), container);
	l1 -> setFont(boldFont);
	l2 = new QLabel(i18n("Your answer"), container);
	l2 -> setFont(boldFont);
	l3 = new QLabel(i18n("Correct answer"), container);
	l3 -> setFont(boldFont);
	lay->addWidget(l1, 1, 1);
	lay->addWidget(l2, 1, 2);
	lay->addWidget(l3, 1, 3);
	
	for(uint i = 0; i < totalAnswers; i++)
	{
		userAnswers[i].putWidgets(container, lay, i + 2);
	}
	
	l1 = new QLabel(i18n("You answered correctly %1 out of %2 questions").arg(correctAnswers).arg(totalAnswers), container);
	l1 -> setAlignment(Qt::AlignCenter);
	lay->addMultiCellWidget(l1, totalAnswers + 3, totalAnswers + 3, 0, 4);
}
