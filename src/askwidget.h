/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef ASKWIDGET_H
#define ASKWIDGET_H

#include <qwidget.h>

class map;

class askWidget : public QWidget
{
Q_OBJECT
	public:
		askWidget(QWidget *parent, map *m, uint count);
		virtual ~askWidget();
	
	public slots:
		virtual void setMovement(bool b);
		virtual void setZoom(bool b);
		virtual void goToMenu() = 0;
	
	signals:
		void setMoveActionChecked(bool b);
		void setZoomActionChecked(bool b);
		void setMoveActionEnabled(bool b);
		void finished();
	
	protected:
		map *p_map;
		
		// the list of asked divisions
		QStringList p_asked;
		
		// the number of questions to do
		uint p_count;
};

#endif
