/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef DIVISIONFLAGASKER_H
#define DIVISIONFLAGASKER_H

#include "boxasker.h"

class divisionFlagAsker : public boxAsker
{
	public:
		divisionFlagAsker(QWidget *parent, map *m, uint count);
	
	protected:
		void nextQuestionHook(QString division, int i, bool isAnswer);
};

#endif
