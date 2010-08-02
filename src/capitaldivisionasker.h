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

#include "boxasker.h"

class TQLabel;

class capitalDivisionAsker : public boxAsker
{
	public:
		capitalDivisionAsker(TQWidget *parent, KGmap *m, TQWidget *w, uint count);

	protected:
		bool nextBoxAskerQuestionHook(const TQString &division, int i, bool isAnswer);
		void setAnswerHook(int userSays);
		TQString getQuestionHook() const;
	
	private:
		TQString p_capital;
};

#endif
