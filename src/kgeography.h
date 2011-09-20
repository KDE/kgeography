/***************************************************************************
 *   Copyright (C) 2004-2007 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KGEOGRAPHY_H
#define KGEOGRAPHY_H

#include <kxmlguiwindow.h>

class KPushButton;
class KToggleAction;

class QLabel;

class KGmap;
class askWidget;

class kgeography : public KXmlGuiWindow
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
		void askPlaceMap();
		void askFlagDivisions();
		void askDivisionFlags();
		void disclaimer();
		void showMapAuthor();
		void resizeMainWindow();
		void questionsEnded();
		
		void setAutomaticZoom(bool b);
		void setMoveActionEnabled(bool b);

		void showPreferencesDialog();

	private:
		QSize getPreferredSize();
		void showResultsDialog();
		
		void putAskWidget();
		void removeOldAskWidget();
		void setMap(KGmap *m);

		int askQuestionNumber(int upto, bool *rOK);


		QWidget *p_bigWidget;
		QWidget *p_underLeftWidget;

		KPushButton *p_consult;
		KPushButton *p_askCapitalDivisions;
		KPushButton *p_askDivisionCapitals;
		KPushButton *p_askMap;
		KPushButton *p_askPlaceMap;
		KPushButton *p_askFlagDivisions;
		KPushButton *p_askDivisionFlags;
		KToggleAction *p_zoom;
		KToggleAction *p_move;
		KToggleAction *p_zoomAutomatic;
		QAction *p_zoomOriginal;
		QAction *p_showAuthor;
		QLabel *p_currentMap;

		KGmap *p_map;
		askWidget *p_askWidget;
		bool p_firstShow, p_mustShowResultsDialog;
};

#endif
