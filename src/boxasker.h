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

class TQLabel;
class TQRadioButton;
class TQVBoxLayout;

class boxAsker : public askWidget
{
Q_OBJECT
	public:
		boxAsker(TQWidget *parent, KGmap *m, TQWidget *w, uint count);
		~boxAsker();
	
	protected:
		virtual bool nextBoxAskerQuestionHook(const TQString &division, int i, bool isAnswer) = 0;
		void nextQuestionHook(const TQString &division);
		void setQuestion(const TQString &q);
		virtual void setAnswerHook(int userSays) = 0;
		
		void keyReleaseEvent(TQKeyEvent *e);
		
		TQVBoxLayout *p_lay;
		TQRadioButton **p_rb;

	protected slots:
		void init();
		
	private slots:
		void checkAnswer();
	
	private:
		/*void nextQuestion();*/
	
		KPushButton *p_accept;
		
		// the position the correct answer is in
		int p_position;
		
		TQLabel *p_label;
};

#endif
