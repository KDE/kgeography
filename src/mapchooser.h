/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef MAPCHOOSER_H
#define MAPCHOOSER_H

#include <QDialog>
#include <QHash>

#include "map.h"
#include "mapparser.h"

class QListWidget;
class QLabel;
class mapChooser : public QDialog
{
Q_OBJECT
	public:
		explicit mapChooser(QWidget *parent);
		~mapChooser();

		KGmap *getMap();
	
	private Q_SLOTS:
		void putImage(const QString &mapName);

	private:
		QListWidget *p_listBox;
		QLabel *p_imageContainer;
		mapReader p_reader;
		QHash<QString, KGmap*> p_maps;
};

#endif
