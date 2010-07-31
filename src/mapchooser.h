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

#include <tqdict.h>

#include "map.h"
#include "mapparser.h"

class mapChooser : public KDialogBase
{
Q_OBJECT
	public:
		mapChooser(TQWidget *parent);
		~mapChooser();

		KGmap *getMap();
	
	private slots:
		void putImage(const TQString &mapName);

	private:
		TQListBox *p_listBox;
		TQWidget *p_image;
		mapReader p_reader;
		TQDict<KGmap> p_maps;
};

#endif
