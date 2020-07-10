/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef POPUPMANAGER_H
#define POPUPMANAGER_H

#include <QObject>

class QWidget;

class myPopup;
class QPoint;

class popupManager : public QObject
{
Q_OBJECT
	public:
		popupManager();

		// sets the parent widget of the popups
		void setWidget(QWidget *w);

		// shows text at p and the flag
		void show(const QString &text, const QString &wikiLink, const QString &text2, const QPoint &p, const QString &flagFile);
	
		// shows text at p
		void show(const QString &text, const QString &wikiLink, const QString &text2, const QPoint &p);
		
		// shows text at p
		void show(const QString &text, const QString &wikiLink, const QPoint &p);
		
	public slots:
		void clear();

	private:
		void init(const QPoint &p);
	
		myPopup *p_mp;
		QWidget *p_parent;
};

#endif
