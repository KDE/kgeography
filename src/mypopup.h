/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef MYPOPUP_H
#define MYPOPUP_H

#include <qhbox.h>

class myPopup : public QHBox
{
Q_OBJECT
	public:
		myPopup(QString text, const QPoint &p, QWidget *parent);
	
	signals:
		void deleteMe();

	protected:
		void mousePressEvent(QMouseEvent *);
};

#endif
