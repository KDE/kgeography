/***************************************************************************
 *   Copyright (C) 2004-2007 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef FLAGDIVISIONASKER_H
#define FLAGDIVISIONASKER_H

#include "boxasker.h"

class flagWidget;

class flagDivisionAsker : public boxAsker
{
	public:
		flagDivisionAsker(QWidget *parent, KGmap *m, QWidget *w, uint count);
	
	protected:
		bool nextBoxAskerQuestionHook(const division *div, int i, bool isAnswer) override;
		void setAnswerHook(int userSays) override;
		QString getQuestionHook() const override;
		division::askMode askMode() const override { return division::eFlag; }

	private:
		flagWidget *p_flag;
};

#endif
