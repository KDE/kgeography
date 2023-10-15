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
		void nextQuestionHook(const division *div) override;
		void mousePressEvent(QMouseEvent *e) override;
		QString getQuestionHook() const override;
		division::askMode askMode() const override { return division::eClick; }

	public Q_SLOTS:
		void setMovement(bool b) override;
		void setZoom(bool b) override;
		void setOriginalZoom() override;
		void setAutomaticZoom(bool b) override;
	
	private Q_SLOTS:
		void handleMapClick(QRgb c, const QPoint &, const QPointF &mapPoint);
	
	private:
		void setCurrentDivision(const division *div);

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
