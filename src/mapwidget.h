/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <qwidget.h>

class mapWidget : public QWidget
{
Q_OBJECT
	public:
		mapWidget(QWidget *parent);
		~mapWidget();

		void setMapImage(const QString &path);
	
	signals:
		void clicked(QRgb);
	
	protected:
		void mousePressEvent(QMouseEvent *e);
	
	private:
		QImage *p_image;
};

#endif
