/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef RESULT_H
#define RESULT_H

#include <tqvariant.h>

class QGridLayout;

class userAnswer
{
	public:
		userAnswer();
		userAnswer(const userAnswer &qa);
		
		userAnswer &operator=(const userAnswer &qa);
		
		void setQuestion(TQVariant question);
		void setAnswer(TQVariant answer);
		void setAnswerCorrect(bool correct);
		void setCorrectAnswer(TQVariant correctAnswer);
		
		void putWidgets(TQWidget *w, TQGridLayout *lay, int row) const;
		
	private:
		TQVariant p_question, p_answer, p_correctAnswer;
		bool p_correct;
};

#endif
