/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef MYPOPUP_H
#define MYPOPUP_H

#include <QFrame>

class QLabel;

class myPopup : public QFrame
{
Q_OBJECT
	friend class popupManager;
	
	private:
		myPopup(QWidget *parent, const QString &text, const QString &wikiLink, const QString &text2 = QString(), const QString &flagFile = QString());
	
	signals:
		void deleteMe();

	protected:
		void mousePressEvent(QMouseEvent *) override;
		bool eventFilter(QObject *obj, QEvent *ev) override;

	private:
		QString wikipedia;
		QLabel *wiki;
};

#endif
