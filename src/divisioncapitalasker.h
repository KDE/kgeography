/***************************************************************************
 *   Copyright (C) 2004-2007 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef DIVISIONCAPITALASKER_H
#define DIVISIONCAPITALASKER_H

#include "boxasker.h"

class divisionCapitalAsker : public boxAsker
{
	public:
		divisionCapitalAsker(QWidget *parent, KGmap *m, QWidget *w, uint count);

	protected:
		bool nextBoxAskerQuestionHook(const QString &division, int i, bool isAnswer) Q_DECL_OVERRIDE;
		void setAnswerHook(int userSays) Q_DECL_OVERRIDE;
		QString getQuestionHook() const Q_DECL_OVERRIDE;
		division::askMode askMode() const Q_DECL_OVERRIDE { return division::eCapital; }
	
	private:
		QString p_capital;
};

#endif
