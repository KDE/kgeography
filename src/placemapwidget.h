/***************************************************************************
 *   Copyright (C) 2004-2007 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef PLACEMAPWIDGET_H
#define PLACEMAPWIDGET_H

#include <QGraphicsView>

#include "map.h"

class QGraphicsRectItem;
class QGraphicsScene;

class placeMapWidget : public QGraphicsView
{
Q_OBJECT
	public:
		explicit placeMapWidget(QWidget *parent);
		~placeMapWidget();
		
		void init(KGmap *map, QImage* mapImage);

		void setMapMove(bool b);
		void setMapZoom(bool b);

		void setCurrentDivisionImage(QImage *divisionImage);
		void placeDivision(QRect& position);
		QSize mapSize() const;

		size_t nbPixels(int pixelIndex) const;
		size_t nbBorderPixels(int pixelIndex1, int pixelIndex2) const;
		const QVector<uchar> outerPixelIndices() const { return p_outerPixelIndices;}

	public slots:
		void setGameImage();
		void setAutomaticZoom(bool b);
	
	signals:
		void setMoveActionChecked(bool b);
		void setZoomActionChecked(bool b);
		void setMoveActionEnabled(bool b);
		// color, widget position and map position
		void clicked(QRgb, const QPoint&, const QPointF &);
	
	protected:
		void mousePressEvent(QMouseEvent *e);
		void mouseMoveEvent(QMouseEvent *e);
		void mouseReleaseEvent(QMouseEvent *e);
		void enterEvent(QEvent *e);
		void leaveEvent(QEvent *e);
		void wheelEvent(QWheelEvent *e);
		void resizeEvent(QResizeEvent *e);
		void createGameMapImage();
		void updateCursor();

	private:

		/**
		 * Updates the move and zoom toggle states from the current mode.
		 */
		void updateActions();
		void updateZoom();
		
		enum Mode { Zooming, WantZoom, Moving, WantMove, None };
		Mode p_mode, p_modeBeforeMidClick;
		KGmap *p_map;
		QImage *p_mapImage;
		QImage *p_gameImage;
		QImage *p_currentDivisionImage;
		QCursor *p_currentCursor;
		QGraphicsRectItem *p_zoomRect;
		QGraphicsScene *p_scene;
		QPointF p_initial; // for rubberbanding, in scene coords
		QPoint p_prev; // for moving, in view coords
		bool p_automaticZoom;
		QGraphicsPixmapItem *p_currentDivisionItem;
		QPoint lastMouseEvent; // to first show the division under the mouse

		QVector<uchar> p_outerPixelIndices;
		QVector<size_t> p_pixelsStats; // how many pixels have given indexed val
		QVector<size_t> p_bordersStats;// how many border-like pixels have indexed vals pair
};

#endif
