/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef CAPITALDIVISIONASKER_H
#define CAPITALDIVISIONASKER_H

#include "askwidget.h"

class KPushButton;

class QLabel;
class QRadioButton;
class QStringList;

class map;

class capitalDivisionAsker : public askWidget
{
Q_OBJECT
	public:
		capitalDivisionAsker(QWidget *parent, map *m, uint count);
	
	public slots:
		void goToMenu();
		
	private slots:
		void checkAnswer();
		void init();
	
	private:
		void nextCapital();
		void showAnswersMessageBox();
	
		QLabel *p_label;
		QRadioButton **p_rb;
		KPushButton *p_accept;
		
		// the position the correct answer is in
		int p_position;
		
		int p_correctAnswers, p_incorrectAnswers;
};

#endif
