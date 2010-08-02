/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef ASKWIDGET_H
#define ASKWIDGET_H

#include <tqvaluevector.h>
#include <tqwidget.h>

#include "answer.h"

class TQLabel;

class KGmap;

class askWidget : public QWidget
{
Q_OBJECT
	public:
		askWidget(TQWidget *parent, KGmap *m, TQWidget *w, uint count, bool showLabel = true);
		virtual ~askWidget();
		
		virtual TQString getQuestionHook() const = 0;
		virtual bool isClickOnDivision() const { return false; }
		int correctAnswers() const;
		TQValueVector<userAnswer> userAnswers() const;
	
	public slots:
		virtual void setMovement(bool b);
		virtual void setZoom(bool b);
		virtual void setOriginalZoom();
	
	signals:
		void setMoveActionChecked(bool b);
		void setZoomActionChecked(bool b);
		void setMoveActionEnabled(bool b);
		void questionsEnded();
	
	protected:
		void clearAsked();
		TQString lastDivisionAsked();
		void nextQuestion();
		virtual void nextQuestionHook(const TQString &division) = 0;
		void questionAnswered(bool wasCorrect);
		void resetAnswers();
	
		KGmap *p_map;
		userAnswer p_currentAnswer;
		TQLabel *p_answers;
	
	private:
		void updateLabel();
		
		int p_correctAnswers, p_incorrectAnswers;
		
		TQValueVector<userAnswer> p_userAnswers;
		
		// the list of asked divisions
		TQStringList p_asked;
		
		// the number of questions to do
		uint p_count;
};

#endif
