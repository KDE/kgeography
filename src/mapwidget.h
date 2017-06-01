/***************************************************************************
 *   Copyright (C) 2004-2007 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QGraphicsView>

#include "map.h"

class QGraphicsRectItem;
class QGraphicsScene;

class mapWidget : public QGraphicsView
{
Q_OBJECT
	public:
		explicit mapWidget(QWidget *parent);
		
		void init(const QImage &mapImage);

		void setMapMove(bool b);
		void setMapZoom(bool b);
		
		QSize mapSize() const;
	
	public slots:
		void setOriginalImage();
        void setAutomaticZoom(bool b);
	
	signals:
		void setMoveActionChecked(bool b);
		void setZoomActionChecked(bool b);
		void setMoveActionEnabled(bool b);
		void clicked(QRgb, const QPoint&);
	
	protected:
		void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
		void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
		void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
		void wheelEvent(QWheelEvent *e) Q_DECL_OVERRIDE;
		void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
	
	private:
		/**
		 * Updates the move and zoom toggle states from the current mode.
		 */
		void updateActions();
		void updateZoom();
		
		enum Mode { Zooming, WantZoom, Moving, WantMove, None };
		Mode p_mode, p_modeBeforeMidClick;
		QImage p_originalImage;
		QGraphicsRectItem *p_zoomRect;
		QGraphicsScene *p_scene;
		QPointF p_initial; // for rubberbanding, in scene coords
		QPoint p_prev; // for moving, in view coords
		bool p_automaticZoom;
};

#endif
