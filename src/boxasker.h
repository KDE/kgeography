/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef BOXASKER_H
#define BOXASKER_H

#include "askwidget.h"

class KPushButton;

class QRadioButton;
class QVBoxLayout;

class boxAsker : public askWidget
{
Q_OBJECT
	public:
		boxAsker(QWidget *parent, map *m, uint count);
	
	protected:
		virtual void nextQuestionHook(QString division, int i, bool isAnswer) = 0;
		
		QVBoxLayout *p_lay;
		QRadioButton **p_rb;

	public slots:
		void goToMenu();
	
	protected slots:
		void init();
		
	private slots:
		void checkAnswer();
	
	private:
		void nextQuestion();
		void showAnswersMessageBox();
	
		KPushButton *p_accept;
		
		bool p_mustShowCorrectIncorrect;
		
		// the position the correct answer is in
		int p_position;
		
		int p_correctAnswers, p_incorrectAnswers;
};

#endif
