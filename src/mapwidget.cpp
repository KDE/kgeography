/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <klocale.h>
#include <kmessagebox.h>

#include <qimage.h>
#include <qpixmap.h>
#include <qtooltip.h>

#include "mapwidget.h"

mapWidget::mapWidget(QWidget *parent) : QWidget(parent)
{
	p_image = 0;
}

mapWidget::~mapWidget()
{
	delete p_image;
}

void mapWidget::setMapImage(const QString &path)
{
	delete p_image;
	p_image = new QImage(path);
	setPaletteBackgroundPixmap(QPixmap(*p_image));
	setFixedSize(p_image -> size());
}

void mapWidget::mousePressEvent(QMouseEvent *e)
{
	if (p_image && e -> button() == Qt::LeftButton)
	{
		QRgb rgb = p_image -> pixel(e -> x(), e -> y());
		emit clicked(rgb, e -> pos());
	}
	else e -> ignore();
}

#include "mapwidget.moc"
