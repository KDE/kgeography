/***************************************************************************
 *   Copyright (C) 2004-2008 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef _INTEGER_INPUT_DIALOG_H
#define _INTEGER_INPUT_DIALOG_H

#include <kdialog.h>

class KIntSpinBox;
class QSlider;

class IntegerInputDialog : public KDialog
{
Q_OBJECT
	public:
		IntegerInputDialog(QWidget *parent, const QString &title, const QString &question,
				   int from, int upto, int byDefault = -1);
		static int GetInteger(QWidget *parent, const QString &title, const QString &question,
				      int from, int upto, int byDefault, bool *rOK);

		int value() const;

	public slots:
		void setValue(int newValue);

	private slots:
		void sliderValueChanged(int newValue);
		void spinboxValueChanged(int newValue);

	private:
		KIntSpinBox *_spinBox;
		QSlider  *_slider;
};

#endif // def _INTEGER_INPUT_DIALOG_H

