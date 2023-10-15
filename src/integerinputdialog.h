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

#include <QDialog>

class QSpinBox;
class QSlider;

class IntegerInputDialog : public QDialog
{
Q_OBJECT
	public:
		IntegerInputDialog(QWidget *parent, const QString &title, const QString &question,
				   int from, int upto, int byDefault = -1);
		static int GetInteger(QWidget *parent, const QString &title, const QString &question,
				      int from, int upto, int byDefault, bool *rOK);

		int value() const;

	public Q_SLOTS:
		void setValue(int newValue);

	private Q_SLOTS:
		void sliderValueChanged(int newValue);
		void spinboxValueChanged(int newValue);

	private:
		QSpinBox *_spinBox;
		QSlider  *_slider;
};

#endif // def _INTEGER_INPUT_DIALOG_H

