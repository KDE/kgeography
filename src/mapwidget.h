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

#include <qimage.h>
#include <qpoint.h>
#include <qwidget.h>

class mapWidget : public QWidget
{
Q_OBJECT
	public:
		mapWidget(QWidget *parent);

		void setMapImage(const QString &path);
		void setMapZoom(bool b);
	
	signals:
		void clicked(QRgb, const QPoint&);
	
	protected:
		void mousePressEvent(QMouseEvent *e);
		void mouseMoveEvent(QMouseEvent *e);
		void mouseReleaseEvent(QMouseEvent *e);
	
	private:
		QImage p_originalImage, p_zoomedImage;
		bool p_zooming, p_wantZoom;
		QPoint p_initial, p_prev;
};

#endif
