/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef ASKWIDGET_H
#define ASKWIDGET_H

#include <qvaluevector.h>
#include <qwidget.h>

#include "answer.h"

class QLabel;

class KGmap;

class askWidget : public QWidget
{
Q_OBJECT
	public:
		askWidget(QWidget *parent, KGmap *m, QWidget *w, uint count, bool showLabel = true);
		virtual ~askWidget();
		
		void showResultsDialog();
	
	public slots:
		virtual void setMovement(bool b);
		virtual void setZoom(bool b);
	
	signals:
		void setMoveActionChecked(bool b);
		void setZoomActionChecked(bool b);
		void setMoveActionEnabled(bool b);
	
	protected:
		virtual void clean() = 0;
		void clearAsked();
		QString lastDivisionAsked();
		void nextQuestion();
		virtual void nextQuestionHook(const QString &division) = 0;
		void questionAnswered(bool wasCorrect);
		void resetAnswers();
		virtual QString getQuestionHook() const = 0;
	
		KGmap *p_map;
		userAnswer p_currentAnswer;
	
	private:
		void updateLabel();
		
		QLabel *p_answers;
		int p_correctAnswers, p_incorrectAnswers;
		
		QValueVector<userAnswer> p_userAnswers;
		
		// the list of asked divisions
		QStringList p_asked;
		
		// the number of questions to do
		uint p_count;
		
		bool p_shouldShowResultsDialog;
};

#endif
