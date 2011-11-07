/***************************************************************************
 *   Copyright (C) 2004-2007 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef ASKWIDGET_H
#define ASKWIDGET_H

#include <qtimer.h>
#include <qvector.h>
#include <qwidget.h>

#include "answer.h"
#include "division.h"

class QLabel;

class KGmap;

class askWidget : public QWidget
{
Q_OBJECT
	public:
		askWidget(QWidget *parent, KGmap *m, QWidget *w, uint count, bool showLabel = true);
		virtual ~askWidget();
		
		virtual QString getQuestionHook() const = 0;
		int correctAnswers() const;
		QVector<userAnswer> userAnswers() const;
	
	public slots:
		virtual void setMovement(bool b);
		virtual void setZoom(bool b);
		virtual void setOriginalZoom();
		virtual void setAutomaticZoom(bool b);
	
	signals:
		void setMoveActionChecked(bool b);
		void setAutomaticZoomActionChecked(bool b);
		void setZoomActionChecked(bool b);
		void setMoveActionEnabled(bool b);
		void questionsEnded();
	
	protected:
		void clearAsked();
		QString lastDivisionAsked();
		void nextQuestion();
		virtual void nextQuestionHook(const QString &division) = 0;
		virtual division::askMode askMode() const = 0;
		void questionAnswered(bool wasCorrect);
		void resetAnswers();
	
		KGmap *p_map;
		userAnswer p_currentAnswer;
		QLabel *p_answers;
	
	private slots:
		void resetLabelColor();
	private:
		void updateLabel();
		
		int p_correctAnswers, p_incorrectAnswers;
		
		QVector<userAnswer> p_userAnswers;
		
		// the list of asked divisions
		QStringList p_asked;
		
		// the number of questions to do
		int p_count;
		
		QTimer m_timer;
};

#endif
