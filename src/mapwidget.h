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

#include <tqimage.h>
#include <tqpoint.h>
#include <tqwidget.h>

class mapWidget : public QWidget
{
Q_OBJECT
	public:
		mapWidget(TQWidget *parent);
		
		void init(const TQString &path, int scrollBarWidth, int scrollBarHeight);

		void setMapMove(bool b);
		void setMapZoom(bool b);
		
		TQSize sizeHint() const;
		TQSize mapSize() const;
	
	public slots:
		void updateHPosition(int value);
		void updateVPosition(int value);
		void setOriginalImage();
	
	signals:
		void setMoveActionChecked(bool b);
		void setZoomActionChecked(bool b);
		void setMoveActionEnabled(bool b);
		void clicked(QRgb, const TQPoint&);
		void updatePosition(int X, int Y);
		void updateVisibleSize(int w, int h);
		void updateMaximumSize(int w, int h);
	
	protected:
		void mousePressEvent(TQMouseEvent *e);
		void mouseMoveEvent(TQMouseEvent *e);
		void mouseReleaseEvent(TQMouseEvent *e);
		void resizeEvent(TQResizeEvent *e);
	
	private:
		void emitMoveActionEnabled();
		TQImage *getCurrentImage();
		void updateShownImage();
		
		TQImage p_originalImage, p_zoomedImageShown;
		bool p_zooming, p_wantZoom, p_moving, p_wantMove;
		TQPoint p_initial, p_prev; // rubberbanding and moving
		int p_zoomX, p_zoomY, p_zoomW, p_zoomH;
		double p_lastFactorX, p_lastFactorY;
		// useful to not do that many updateShownImage
		TQSize p_oldSize;
		int p_oldZoomX, p_oldZoomY, p_oldZoomW, p_oldZoomH;
		int p_scrollBarWidth, p_scrollBarHeight;
		bool p_scrollBarsVisible;
};

#endif
