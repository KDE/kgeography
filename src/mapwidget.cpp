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

#include <qpainter.h>
#include <qtooltip.h>

#include "mapwidget.h"

mapWidget::mapWidget(QWidget *parent) : QWidget(parent)
{
	p_wantZoom = false;
	p_zooming = false;
}

mapWidget::~mapWidget()
{
}

void mapWidget::setMapImage(const QString &path)
{
	p_originalImage.load(path);
	setPaletteBackgroundPixmap(p_originalImage);
	setFixedSize(p_originalImage.size());
}

void mapWidget::setMapZoom(bool b)
{
	p_wantZoom = b;
}

void mapWidget::mousePressEvent(QMouseEvent *e)
{
	if (e -> button() == Qt::LeftButton)
	{
		if (p_wantZoom)
		{
			p_zooming = true;
			p_initial = e -> pos();
			p_prev = e -> pos();
		}
		else
		{
			QRgb rgb;
			QImage *currentImage;
			
			if (p_zoomedImage.isNull()) currentImage = &p_originalImage;
			else currentImage = &p_zoomedImage;
			
			rgb = currentImage -> pixel(e -> x(), e -> y());
			emit clicked(rgb, e -> pos());
		}
	}
	else
	{
		if (p_wantZoom)
		{
			setPaletteBackgroundPixmap(p_originalImage);
			p_zoomedImage = QImage();
		}
		else e -> ignore();
	}
}

void mapWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (p_zooming)
	{
		QPainter p(this);
		
		p.setRasterOp(Qt::XorROP);
		p.setPen (QPen(Qt::white, 1, Qt::DotLine));

		// remove previous rectangle
		p.drawRect(QRect(p_initial, p_prev));
		// put new rectangle
		p.drawRect(QRect(p_initial, e -> pos()));
		
		p_prev = e -> pos();
	}
}

void mapWidget::mouseReleaseEvent(QMouseEvent *)
{
	if (p_zooming)
	{
		QImage *sourceImage;
		QPainter p(this);
		int minX, minY, maxX, maxY, w, h;
		
		minX = p_initial.x();
		maxX = p_prev.x();
		if (minX > maxX) qSwap(minX, maxX);
		minY = p_initial.y();
		maxY = p_prev.y();
		if (minY > maxY) qSwap(minY, maxY);
		
		w = maxX - minX;
		h = maxY - minY;
		
		p_zooming = false;
		
		p.setRasterOp(Qt::XorROP);
		p.setPen (QPen(Qt::white, 1, Qt::DotLine));
		// remove previous rectangle
		p.drawRect(minX, minY, w, h);
		
		if (p_zoomedImage.isNull()) sourceImage = &p_originalImage;
		else sourceImage = &p_zoomedImage;
	
		if (w > 1 && h > 1)
		{
			p_zoomedImage = sourceImage -> copy(minX, minY, w, h
			);
			p_zoomedImage = p_zoomedImage.scale(p_originalImage.size());
			setPaletteBackgroundPixmap(p_zoomedImage);
		}
	}
}

#include "mapwidget.moc"
