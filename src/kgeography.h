/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KGEOGRAPHY_H
#define KGEOGRAPHY_H

#include <kmainwindow.h>

#include "popupmanager.h"

class KToggleAction;

class infoWidget;
class map;
class mapWidget;
class myPopup;

class kgeography : public KMainWindow
{
Q_OBJECT
	public:
		kgeography();
		~kgeography();
	
	private slots:
		void openMap();
	
		void consult();
		void question();
		
		void handleMapClick(QRgb c, const QPoint &p);
	
	protected:
		void mousePressEvent(QMouseEvent *e);
	
	private:
		void nextDivision();
		void setMap(map *m);
	
		infoWidget *p_infoWidget;
		mapWidget *p_mapWidget;
		KToggleAction *p_consult, *p_question;
		map *p_map;
		popupManager p_popupManager;
		QStringList p_asked;
		bool p_shouldClearPopup;
};

#endif
