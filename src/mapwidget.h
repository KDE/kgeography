/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
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
		
		void init(const QString &path, int scrollBarWidth, int scrollBarHeight);

		void setMapMove(bool b);
		void setMapZoom(bool b);
		
		QSize sizeHint() const;
		QSize mapSize() const;
	
	public slots:
		void updateHPosition(int value);
		void updateVPosition(int value);
		void setOriginalImage();
	
	signals:
		void setMoveActionChecked(bool b);
		void setZoomActionChecked(bool b);
		void setMoveActionEnabled(bool b);
		void clicked(QRgb, const QPoint&);
		void updatePosition(int X, int Y);
		void updateVisibleSize(int w, int h);
		void updateMaximumSize(int w, int h);
	
	protected:
		void mousePressEvent(QMouseEvent *e);
		void mouseMoveEvent(QMouseEvent *e);
		void mouseReleaseEvent(QMouseEvent *e);
		void resizeEvent(QResizeEvent *e);
	
	private:
		void emitMoveActionEnabled();
		QImage *getCurrentImage();
		void updateShownImage();
		
		QImage p_originalImage, p_zoomedImageShown;
		bool p_zooming, p_wantZoom, p_moving, p_wantMove;
		QPoint p_initial, p_prev; // rubberbanding and moving
		int p_zoomX, p_zoomY, p_zoomW, p_zoomH;
		double p_lastFactorX, p_lastFactorY;
		// useful to not do that many updateShownImage
		QSize p_oldSize;
		int p_oldZoomX, p_oldZoomY, p_oldZoomW, p_oldZoomH;
		int p_scrollBarWidth, p_scrollBarHeight;
		bool p_scrollBarsVisible;
};

#endif
