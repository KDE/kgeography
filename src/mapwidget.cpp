/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid                               *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <kapplication.h>
#include <klocale.h>

#include <qcursor.h>
#include <qpainter.h>
#include <qtooltip.h>

#include "mapwidget.h"

mapWidget::mapWidget(QWidget *parent) : QWidget(parent)
{
	p_wantZoom = false;
	p_zooming = false;
	p_wantMove = false;
	p_moving = false;
}

void mapWidget::setMapImage(const QString &path)
{
	p_originalImage.load(path);
	setOriginalImage();
	setFixedSize(p_originalImage.size());
}

void mapWidget::setMapMove(bool b)
{
	p_wantMove = b;
	if (b)
	{
		emit setZoomActionChecked(false);
		p_wantZoom = false;
	}
}

void mapWidget::setMapZoom(bool b)
{
	p_wantZoom = b;
	if (b)
	{
		emit setMoveActionChecked(false);
		p_wantMove = false;
	}
}

void mapWidget::mousePressEvent(QMouseEvent *e)
{
	if (e -> button() == Qt::LeftButton)
	{
		if (p_wantZoom)
		{
			p_initial = e -> pos();
			p_prev = e -> pos();
			p_zooming = true;
		}
		else if (p_wantMove)
		{
			p_initial = e -> pos();
			setCursor(Qt::SizeAllCursor);
			p_moving = true;
		}
		else
		{
			QRgb rgb;
			QImage *currentImage;
			
			currentImage = getCurrentImage();
			
			rgb = currentImage -> pixel(e -> x(), e -> y());
			emit clicked(rgb, e -> pos());
		}
	}
	else
	{
		if (p_wantZoom)
		{
			setOriginalImage();
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
		p.setPen(QPen(Qt::white, 1, Qt::DotLine));

		// remove previous rectangle
		p.drawRect(QRect(p_initial, p_prev));
		// put new rectangle
		p.drawRect(QRect(p_initial, e -> pos()));
		
		p_prev = e -> pos();
	}
	else if (p_moving)
	{
		int nextX, nextY, zW, zH, oW, oH;
		
		/* some shortcuts :-D */
		oW = p_originalImage.width();
		oH = p_originalImage.height();
		zW = p_zoomedImageBig.width();
		zH = p_zoomedImageBig.height();
		
		/* where next x and y will be */
		nextX = p_zoomX + (p_initial.x() - e -> pos().x());
		nextY = p_zoomY + (p_initial.y() - e -> pos().y());
		
		/* make sure we don't go out of bounds */
		if (nextX < 0) nextX = 0;
		if (nextY < 0) nextY = 0;
		if (nextX > zW - oW) nextX = zW - oW;
		if (nextY > zH - oH) nextY = zH - oH;
		
		/* if nothing has change there's no need to repaint */
		if (nextX != p_zoomX || nextY != p_zoomY)
		{
			p_zoomedImageShown = p_zoomedImageBig.copy(nextX, nextY, oW, oH);
			setPaletteBackgroundPixmap(p_zoomedImageShown);
			p_initial = e -> pos();
			p_zoomX = nextX;
			p_zoomY = nextY;
			kapp -> processEvents();
		}
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
		p.setPen(QPen(Qt::white, 1, Qt::DotLine));
		// remove previous rectangle
		p.drawRect(minX, minY, w, h);
		
		if (minX + w > width()) w = width() - minX;
		if (minY + h > height()) h = height() - minY;
		if (minX < 0)
		{
			minX = 0;
			w = maxX;
		}
		if (minY < 0)
		{
			minY = 0;
			h = maxY;
		}
		
		sourceImage = getCurrentImage();
	
		if (w > 1 && h > 1)
		{
			int zW, zH;
			double factorX, factorY;
			
			/* set the new image quick */
			p_zoomedImageShown = sourceImage -> copy(minX, minY, w, h);
			p_zoomedImageShown = p_zoomedImageShown.scale(p_originalImage.size());
			setPaletteBackgroundPixmap(p_zoomedImageShown);
			kapp -> processEvents(); 
			
			/* calculate the zoomed map so we can scroll*/
			zW = p_zoomedImageBig.width();
			zH = p_zoomedImageBig.height();
			
			factorX = (double)zW / w / p_lastFactorX;
			factorY = (double)zH / h / p_lastFactorY;
			
			p_zoomX = (int)((p_zoomX + minX) * factorX);
			p_zoomY = (int)((p_zoomY + minY) * factorY);
			
			p_lastFactorX = factorX;
			p_lastFactorY = factorY;
			
			p_zoomedImageBig = p_zoomedImageBig.scale((int)(zW * factorX), (int)(zH * factorY));
			emit setMoveActionEnabled(true);
		}
	}
	else if (p_moving)
	{
		unsetCursor();
		p_moving = false;
	}
}

QImage *mapWidget::getCurrentImage()
{
	if (p_zoomedImageShown.isNull()) return &p_originalImage;
	else return &p_zoomedImageShown;
}

void mapWidget::setOriginalImage()
{
	setPaletteBackgroundPixmap(p_originalImage);
	p_zoomedImageBig = p_originalImage;
	p_zoomedImageShown = QImage();
	p_lastFactorX = 1;
	p_lastFactorY = 1;
	p_zoomX = 0;
	p_zoomY = 0;
	emit setMoveActionEnabled(false);
}

#include "mapwidget.moc"
