/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KGEOGRAPHY_H
#define KGEOGRAPHY_H

#include <kmainwindow.h>

class KAction;
class KPushButton;
class KToggleAction;

class QHBox;
class QLabel;
class QVBox;

class KGmap;
class askWidget;

class kgeography : public KMainWindow
{
Q_OBJECT
	public:
		kgeography();
		~kgeography();
	
	protected:
		void showEvent(QShowEvent *e);

	private slots:
		void openMap();

		void consult();
		void askCapitalDivisions();
		void askDivisionCapitals();
		void askMap();
		void askFlagDivisions();
		void askDivisionFlags();
		void disclaimer();
		void resizeMainWindow();
		void showResultsDialog();

	private:
		QSize getPreferredSize();
		
		void putAskWidget();
		void removeOldAskWidget();
		void setMap(KGmap *m);

		QHBox *p_bigWidget;
		QVBox *p_underLeftWidget;

		KPushButton *p_consult;
		KPushButton *p_askCapitalDivisions;
		KPushButton *p_askDivisionCapitals;
		KPushButton *p_askMap;
		KPushButton *p_askFlagDivisions;
		KPushButton *p_askDivisionFlags;
		KToggleAction *p_zoom, *p_move;
		KAction *p_zoomOriginal;
		QLabel *p_currentMap;

		KGmap *p_map;
		askWidget *p_askWidget;
		bool p_firstShow, p_mustShowResultsDialog;
};

#endif
