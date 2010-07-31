/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef POPUPMANAGER_H
#define POPUPMANAGER_H

#include <tqobject.h>

class QWidget;

class myPopup;

class popupManager : public QObject
{
Q_OBJECT
	public:
		popupManager(TQWidget *parent);

		// shows text at p and the flag
		void show(const TQString &text, const TQString &text2, const TQPoint &p, const TQString &flagFile);
	
		// shows text at p
		void show(const TQString &text, const TQString &text2, const TQPoint &p);
		
		// shows text at p
		void show(const TQString &text, const TQPoint &p);
		
	public slots:
		void clear();

	private:
		void init(const TQPoint &p);
	
		myPopup *p_mp;
		TQWidget *p_parent;
};

#endif
