/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <qhbox.h>

class QLabel;

class infoWidget : public QHBox
{
	public:
		infoWidget(QWidget *parent);
		
		void setQuestionMode(bool b);
		
		void setName(QString text);
		void setNext(QString text);
		void addResult(bool correct);
		int getCorrect() const;
		int getTotal() const;
	
	private:
		QLabel *p_name, *p_next, *p_guesses;
		uint p_correct, p_total;
};

#endif
