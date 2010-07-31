/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   tsdgeos@terra.es                                                      *
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
class QScrollBar;
class QSize;

class flagDivisionAsker;
class infoWidget;
class mapWidget;

class mapAsker : public askWidget
{
Q_OBJECT
	public:
		mapAsker(TQWidget *parent, KGmap *m, TQWidget *w, bool asker, uint count = 0);
		~mapAsker();

		TQSize mapSize() const;
		bool isAsker() const;
		virtual bool isClickOnDivision() const { return true; }
		
	protected:
		void nextQuestionHook(const TQString &division);
		void mousePressEvent(TQMouseEvent *e);
		TQString getQuestionHook() const;
		void showEvent(TQShowEvent *e);

	public slots:
		void setMovement(bool b);
		void setZoom(bool b);
		void setOriginalZoom();
	
	private slots:
		void handleMapClick(QRgb c, const TQPoint &p);
		void setScrollBarsPosition(int x, int y);
		void setScrollBarsVisibleSize(int w, int h);
		void setScrollBarsMaximumSize(int w, int h);
		void showScrollBars(bool b);
	
	private:
		TQLabel *p_next;
		TQWidget *p_fill;
		TQScrollBar *p_hsb, *p_vsb;
		int p_hsbms, p_vsbms; //H and V maximum size for scrollbars
		
		infoWidget *p_infoWidget;
		mapWidget *p_mapWidget;
		
		popupManager p_popupManager;
		bool p_shouldClearPopup, p_asker, p_firstShow;
};

#endif
