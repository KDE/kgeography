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

class KToggleAction;

class map;
class mapWidget;

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
		
		void handleClick(QRgb c);
	
	private:
		void nextDivision();
	
		mapWidget *p_mapWidget;
		KToggleAction *p_consult, *p_question;
		map *p_map;
		QStringList p_asked;
};

#endif
