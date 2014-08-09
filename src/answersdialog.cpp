/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "answersdialog.h"

#include <klocalizedstring.h>

#include <qlabel.h>
#include <qlayout.h>
#include <qscrollarea.h>
#include <qdialogbuttonbox.h>

#include "answer.h"

answersDialog::answersDialog(QWidget *parent, const QVector<userAnswer> &userAnswers, const QString &question, int correctAnswers)
	: QDialog(parent)
{
	setWindowTitle(i18n("Your Answers Were"));

	QVBoxLayout *mainLayout = new QVBoxLayout;

	p_scrollArea = new QScrollArea(this);
	mainLayout->addWidget(p_scrollArea);

	p_container = new QWidget(this);
	p_scrollArea -> setWidget(p_container);
	p_scrollArea -> setWidgetResizable(true);

	uint totalAnswers = userAnswers.count();
	
	QGridLayout *gridLayout = new QGridLayout(p_container);
	int spacing = gridLayout -> spacing();
	gridLayout -> setSpacing(0);
	gridLayout -> setColumnStretch(0, 1);
	gridLayout -> setColumnStretch(4, 1);
	gridLayout -> setRowStretch(totalAnswers + 4, 1);
	
	p_container->setLayout(gridLayout);

	QFont titleFont = p_container -> font();
	titleFont.setPointSize(24);

	QLabel *titleLabel = new QLabel(question);
	titleLabel -> setFont(titleFont);
	gridLayout -> addWidget(titleLabel, 0, 0, 1, 5, Qt::AlignHCenter);

	QLabel *correctAnswersInfoLabel = new QLabel(i18n("You answered correctly %1 out of %2 questions.", correctAnswers, totalAnswers));
	correctAnswersInfoLabel -> setAlignment(Qt::AlignCenter);
	gridLayout -> addWidget(correctAnswersInfoLabel, 1, 0, 1, 5);

	QFont headerFont = p_container -> font();
	headerFont.setBold(true);
	
	QLabel *questionHeaderLabel = new QLabel(i18n("Question"));
	questionHeaderLabel -> setFont(headerFont);
	questionHeaderLabel -> setMargin(spacing);
	gridLayout->addWidget(questionHeaderLabel, 2, 1);

	QLabel *userAnswerHeaderLabel = new QLabel(i18n("Your Answer"));
	userAnswerHeaderLabel -> setFont(headerFont);
	userAnswerHeaderLabel -> setMargin(spacing);
	gridLayout->addWidget(userAnswerHeaderLabel, 2, 2);

	QLabel *correctAnswerHeaderLabel = new QLabel(i18n("Correct Answer"));
	correctAnswerHeaderLabel -> setFont(headerFont);
	correctAnswerHeaderLabel	 -> setMargin(spacing);
	gridLayout->addWidget(correctAnswerHeaderLabel, 2, 3);

	for(uint i = 0; i < totalAnswers; i++)
	{
		userAnswers[i].putWidgets(p_container, gridLayout, i + 3, spacing);
	}

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	mainLayout->addWidget(buttonBox);

	setLayout(mainLayout);

	resize(500, 500);
}

