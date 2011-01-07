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

#include <tqlabel.h>
#include <tqlayout.h>
#include <tqscrollview.h>

#include "answer.h"
#include "answersdialog.h"

answersDialog::answersDialog(TQWidget *parent, const TQValueVector<userAnswer> &userAnswers, TQString question, int correctAnswers) : KDialogBase(parent, 0, true, i18n("Your Answers Were"), Ok)
{
	TQLabel *l1, *l2, *l3;
	TQFont boldFont, bigFont;
	uint totalAnswers;
	totalAnswers = userAnswers.count();
	
	p_sv = new TQScrollView(this);
	setMainWidget(p_sv);
	
	p_container = new TQWidget(p_sv -> viewport());
	p_sv -> viewport() -> setPaletteBackgroundColor(p_container -> paletteBackgroundColor());
	
	TQGridLayout *lay = new TQGridLayout(p_container);
	lay -> setColStretch(0, 1);
	lay -> setColStretch(4, 1);
	lay -> setRowStretch(totalAnswers + 4, 1);
	
	// Title
	bigFont = p_container -> font();
	bigFont.setPointSize(24);
	l1 = new TQLabel(question, p_container);
	l1 -> setFont(bigFont);
	l1 -> setAlignment(Qt::AlignCenter);
	lay->addMultiCellWidget(l1, 0, 0, 0, 4);
	
	// Headers
	boldFont = p_container -> font();
	boldFont.setBold(true);
	
	l1 = new TQLabel(i18n("Question"), p_container);
	l1 -> setFont(boldFont);
	l2 = new TQLabel(i18n("Your Answer"), p_container);
	l2 -> setFont(boldFont);
	l3 = new TQLabel(i18n("Correct Answer"), p_container);
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

	lay -> addItem(new TQSpacerItem(20, 20, TQSizePolicy::Fixed, TQSizePolicy::Fixed), totalAnswers + 3, 2);
	
	l1 = new TQLabel(i18n("You answered correctly %1 out of %2 questions.").arg(correctAnswers).arg(totalAnswers), p_container);
	l1 -> setAlignment(Qt::AlignCenter);
	lay->addMultiCellWidget(l1, totalAnswers + 4, totalAnswers + 4, 0, 4);
	
	p_sv -> addChild(p_container);
	resize(500, 500);
}

void answersDialog::showEvent(TQShowEvent *)
{
	positionContainer();
}

void answersDialog::resizeEvent(TQResizeEvent *)
{
	positionContainer();
}

void answersDialog::positionContainer()
{
	int x = p_sv -> viewport() -> width() - p_container -> width();
	x = x / 2;
	if (x < 0) x = 0;
	p_sv -> moveChild(p_container, x, 0);
}

