/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef MAPCHOOSER_H
#define MAPCHOOSER_H

#include <kdialogbase.h>

#include <qdict.h>

#include "map.h"
#include "mapparser.h"

class mapChooser : public KDialogBase
{
Q_OBJECT
	public:
		mapChooser(QWidget *parent);
		~mapChooser();

		KGmap *getMap();
	
	private slots:
		void putImage(const QString &mapName);

	private:
		QListBox *p_listBox;
		QWidget *p_image;
		mapReader p_reader;
		QDict<KGmap> p_maps;
};

#endif
