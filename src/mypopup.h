/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef MYPOPUP_H
#define MYPOPUP_H

#include <qframe.h>
#include <qpopupmenu.h>

class myPopup : public QPopupMenu
{
Q_OBJECT
	friend class popupManager;
	
	private:
		myPopup(QWidget *parent, const QString &text, const QString &text2 = QString::null, const QString &flagFile = QString::null);
	
	private slots:
		void popupClick();
		void slotActivated(int);

	signals:
		void deleteMe();

};

class myPopupItem : public QFrame
{
Q_OBJECT
	friend class myPopup;
	
	private:
		myPopupItem(myPopup *parent, const QString &text, const QString &text2 = QString::null, const QString &flagFile = QString::null);
	
	signals:
		void click();

	protected:
		void mousePressEvent(QMouseEvent *);
};

#endif
