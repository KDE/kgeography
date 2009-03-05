/***************************************************************************
 *   Copyright (C) 2004-2008 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "integerinputdialog.h"

#include <qboxlayout.h>
#include <qlabel.h>
#include <qslider.h>
#include <qspinbox.h>
#include <knuminput.h>

IntegerInputDialog::IntegerInputDialog(QWidget *parent, const QString &title, const QString &question,
				       int from, int upto, int byDefault)
	: KDialog(parent)
{
	setCaption(title);
	setButtons(KDialog::Ok | KDialog::Cancel);

	int value = ( from <= byDefault && byDefault <= upto ) ? byDefault : from;

	QWidget *container = new QWidget(this);
	this->setMainWidget(container);
	QVBoxLayout *vbox = new QVBoxLayout(container);

	QLabel *questionLbl = new QLabel(question);
	vbox->addWidget(questionLbl);

	if ( upto % 2 )
	{
		_nojet = new KIntNumInput();
		_nojet->setRange(from, upto, byDefault, true);
		vbox->addWidget(_nojet);
		_slider = NULL;
	}
	else
	{
		_nojet = NULL;
		QHBoxLayout *hbox = new QHBoxLayout();
		vbox->addLayout(hbox);

		_slider = new QSlider(Qt::Horizontal);
		_slider->setRange(from, upto);
		_slider->setTickPosition(QSlider::TicksBelow);
		_slider->setValue(value);
		connect(_slider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
		hbox->addWidget(_slider);

		_spinBox = new QSpinBox();
		_spinBox->setRange(from, upto);
		_spinBox->setValue(value);
		connect(_spinBox, SIGNAL(valueChanged(int)), this, SLOT(spinboxValueChanged(int)));
		hbox->addWidget(_spinBox);
	}
    
	_spinBox->setFocus();
}

int IntegerInputDialog::value() const
{
	return ( _nojet != NULL ) ? _nojet->value() : _slider->value();
}

void IntegerInputDialog::setValue(int newValue)
{
	if ( _nojet != NULL )
	{
		_nojet->setValue(newValue);
	}
	else
	{
		_slider->setValue(newValue);
	}
	/*
	if ( _slider->value() != newValue )
		_slider->setValue(newValue);
	if ( _slider->value() != newValue )
		_slider->setValue(newValue);
	*/
}

void IntegerInputDialog::sliderValueChanged(int newValue)
{
	if ( _spinBox->value() != newValue )
		_spinBox->setValue(newValue);
}

void IntegerInputDialog::spinboxValueChanged(int newValue)
{
	if ( _slider->value() != newValue )
		_slider->setValue(newValue);
}

int IntegerInputDialog::GetInteger(QWidget *parent, const QString &title, const QString &question,
				   int from, int upto, int byDefault, bool *rOK)
{
	int ret = -1;
	bool ok = false;
	IntegerInputDialog dialog(parent, title, question, from, upto, byDefault);
	if ( dialog.exec() == QDialog::Accepted )
	{
		ok = true;
		ret = dialog.value();
	}
	if ( rOK != NULL ) *rOK = ok;
	return ret;

}

#include "integerinputdialog.moc"

