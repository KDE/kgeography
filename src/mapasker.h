/***************************************************************************
 *   Copyright (C) 2004-2007 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef MAPASKER_H
#define MAPASKER_H

#include "askwidget.h"
#include "popupmanager.h"

class QLabel;
class QSize;

class infoWidget;
class mapWidget;

class mapAsker : public askWidget
{
Q_OBJECT
	public:
		mapAsker(QWidget *parent, KGmap *m, QWidget *w, bool asker, uint count = 0);
		~mapAsker();

		QSize mapSize() const;
		bool isAsker() const;
		
	protected:
		void nextQuestionHook(const QString &division) Q_DECL_OVERRIDE;
		void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
		QString getQuestionHook() const Q_DECL_OVERRIDE;
		void showEvent(QShowEvent *e) Q_DECL_OVERRIDE;
		division::askMode askMode() const Q_DECL_OVERRIDE { return division::eClick; }

	public slots:
		void setMovement(bool b) Q_DECL_OVERRIDE;
		void setZoom(bool b) Q_DECL_OVERRIDE;
		void setOriginalZoom() Q_DECL_OVERRIDE;
		void setAutomaticZoom(bool b) Q_DECL_OVERRIDE;
	
	private slots:
		void handleMapClick(QRgb c, const QPoint &p);
	
	private:
		QLabel *p_next;
		QWidget *p_fill;
		
		infoWidget *p_infoWidget;
		mapWidget *p_mapWidget;
		
		popupManager p_popupManager;
		bool p_shouldClearPopup, p_asker, p_firstShow;

		QVector<QRgb> p_shuffledColormap;
		QVector<QRgb> p_originalColormap;
};

#endif
