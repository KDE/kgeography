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

#include <tqvaluevector.h>

class TQScrollView;
class TQWidget;

class userAnswer;

class answersDialog : public KDialogBase
{
	public:
		answersDialog(TQWidget *parent, const TQValueVector<userAnswer> &userAnswers, TQString question, int correctAnswers);
	
	protected:
		void showEvent(TQShowEvent *e);
		void resizeEvent(TQResizeEvent *e);
	
	private:
		void positionContainer();
		
		TQWidget *p_container;
		TQScrollView *p_sv;
};

#endif
