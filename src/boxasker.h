/***************************************************************************
 *   Copyright (C) 2004-2007 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef BOXASKER_H
#define BOXASKER_H

#include "askwidget.h"

class QPushButton;
class QGroupBox;
class QLabel;
class QRadioButton;
class QVBoxLayout;

class boxAsker : public askWidget
{
Q_OBJECT
	public:
		boxAsker(QWidget *parent, KGmap *m, QWidget *w, uint count);
		~boxAsker();

		bool eventFilter(QObject *obj, QEvent *event) override;

	public slots:
		void updateLayout();

	protected:
		virtual bool nextBoxAskerQuestionHook(const division *div, int i, bool isAnswer) = 0;
		void nextQuestionHook(const division *div) override;
		void setQuestion(const QString &q);
		virtual void setAnswerHook(int userSays) = 0;

		void keyPressEvent(QKeyEvent *e) override;
		void keyReleaseEvent(QKeyEvent *e) override;
		
		void setHeadWidget(QWidget *headWidget);
		QVector<QRadioButton*> p_radioButtons;

		// the position the correct answer is in
		int p_position;

	protected slots:
		void init();
		void atLeastOneSelected();
		
	private slots:
		void checkAnswer();
	
	private:
		void layoutGroupBox();
		void layoutAligned();
	
		QVBoxLayout *p_lay;
		QGridLayout *p_groupLayout;
		QPushButton *p_accept;

		QWidget *p_headWidget;
		QLabel *p_label;
		QGroupBox *p_groupBox;
		QVector<QLabel*> p_answerLabels;
};

#endif
