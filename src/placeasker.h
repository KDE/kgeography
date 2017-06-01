/***************************************************************************
 *   Copyright (C) 2004-2007 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *   Copyright (C) 2006 by Isaac Clerencia                                 *
 *   isaac@warp.es                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef PLACEASKER_H
#define PLACEASKER_H

#include "askwidget.h"
#include "popupmanager.h"

class QLabel;
class QSize;

class infoWidget;
class placeMapWidget;

class placeAsker : public askWidget
{
Q_OBJECT
	public:
		placeAsker(QWidget *parent, KGmap *m, QWidget *w, uint count = 0);
		~placeAsker();

		QSize mapSize() const;
		bool isAsker() const;

	protected:
		void nextQuestionHook(const QString &division) Q_DECL_OVERRIDE;
		void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
		QString getQuestionHook() const Q_DECL_OVERRIDE;
		division::askMode askMode() const Q_DECL_OVERRIDE { return division::eClick; }

	public slots:
		void setMovement(bool b) Q_DECL_OVERRIDE;
		void setZoom(bool b) Q_DECL_OVERRIDE;
		void setOriginalZoom() Q_DECL_OVERRIDE;
		void setAutomaticZoom(bool b) Q_DECL_OVERRIDE;
	
	private slots:
		void handleMapClick(QRgb c, const QPoint &, const QPointF &mapPoint);
	
	private:
		void setCurrentDivision(const QString& division);

		QLabel *p_next;
		QWidget *p_fill;

		infoWidget *p_infoWidget;
		placeMapWidget *p_mapWidget;

		QImage *p_currentDivisionImage;
		QImage *p_mapImage;
		QRect p_currentDivisionRect;
		QRgb p_currentRgb;
		QVector<uchar> p_placedPixelIndices;
};

#endif
