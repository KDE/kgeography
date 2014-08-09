/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef ANSWER_H
#define ANSWER_H

#include <QtCore/qvariant.h>

class QGridLayout;

class userAnswer
{
	public:
		userAnswer();
		userAnswer(const userAnswer &qa);
		
		userAnswer &operator=(const userAnswer &qa);
		
		void setQuestion(const QVariant &question);
		void setAnswer(const QVariant &answer);
		void setAnswerCorrect(bool correct);
		void setCorrectAnswer(const QVariant &correctAnswer);
		
		void putWidgets(QWidget *w, QGridLayout *lay, int row, int margin) const;
		
	private:
		QVariant p_question, p_answer, p_correctAnswer;
		bool p_correct;
};

#endif
