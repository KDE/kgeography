/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef RESULTSWIDGET_H
#define RESULTSWIDGET_H

#include <kdialogbase.h>

#include <qvaluevector.h>

class QScrollView;
class QWidget;

class userAnswer;

class answersDialog : public KDialogBase
{
	public:
		answersDialog(QWidget *parent, const QValueVector<userAnswer> &userAnswers, QString question, int correctAnswers);
	
	protected:
		void showEvent(QShowEvent *e);
		void resizeEvent(QResizeEvent *e);
	
	private:
		void positionContainer();
		
		QWidget *p_container;
		QScrollView *p_sv;
};

#endif
