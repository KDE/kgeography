/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef POPUPMANAGER_H
#define POPUPMANAGER_H

#include <qobject.h>

class QWidget;

class myPopup;

class popupManager : public QObject
{
Q_OBJECT
	public:
		popupManager(QWidget *parent);
		
		// shows text at p
		void show(QString text, const QPoint &p);
		
		// shots text centered
		void show(QString text);
	
	public slots:
		void clear();

	private:
		myPopup *p_mp;
		QWidget *p_parent;
};

#endif
