/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef RESULTSWIDGET_H
#define RESULTSWIDGET_H

#include <kdialog.h>

#include <QtCore/qvector.h>

class QScrollArea;
class QWidget;

class userAnswer;

class answersDialog : public KDialog
{
	public:
		answersDialog(QWidget *parent, const QVector<userAnswer> &userAnswers, const QString &question, int correctAnswers);
	
	private:
		QWidget *p_container;
		QScrollArea *p_sa;
};

#endif
