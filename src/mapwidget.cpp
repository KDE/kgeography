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
#include <math.h>

#include <qcursor.h>
#include <qpainter.h>

#include "mapwidget.h"

mapWidget::mapWidget(QWidget *parent, const QString &path) : QWidget(parent)
{
	p_wantZoom = false;
	p_zooming = false;
	p_wantMove = false;
	p_moving = false;
	p_zoomW = 0;
	p_zoomH = 0;
	
	p_originalImage.load(path);
	setOriginalImage();
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

QSize mapWidget::sizeHint() const
{
	return p_originalImage.size();
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
		int oW, oH;
		
		// some shortcuts :-D 
		oW = p_originalImage.width();
		oH = p_originalImage.height();
		
		// where next x and y will be 
		p_zoomX += (p_initial.x() - e -> pos().x());
		p_zoomY += (p_initial.y() - e -> pos().y());
		
		// make sure we don't go out of bounds
		if (p_zoomX < 0) p_zoomX = 0;
		if (p_zoomY < 0) p_zoomY = 0;
		if (p_zoomX > oW - width() * p_lastFactorX) p_zoomX = (int)rint(oW - width() * p_lastFactorX);
		if (p_zoomY > oH - height() * p_lastFactorY) p_zoomY = (int)rint(oH - height() * p_lastFactorY);
		
		p_zoomedImageShown = p_originalImage.copy(p_zoomX, p_zoomY, p_zoomW, p_zoomH);
		p_zoomedImageShown = p_zoomedImageShown.scale(size());
		setPaletteBackgroundPixmap(p_zoomedImageShown);
		p_initial = e -> pos();
		kapp -> processEvents();
	}
}

void mapWidget::mouseReleaseEvent(QMouseEvent *e)
{
	if (p_zooming)
	{
		QPainter p(this);
		QRect r(p_initial, e -> pos());
		
		p.setRasterOp(Qt::XorROP);
		p.setPen(QPen(Qt::white, 1, Qt::DotLine));
		// remove previous rectangle
		p.drawRect(r);
		
		r = r.normalize();
		
		p_zoomX += (int)rint(r.left() * p_lastFactorX);
		p_zoomY += (int)rint(r.top() * p_lastFactorY);
		
		p_zoomW = (int)rint(r.width() * p_lastFactorX);
		p_zoomH = (int)rint(r.height() * p_lastFactorY);
		
		if (r.right() > width()) p_zoomW = width() - p_zoomX;
		if (r.height() > height()) p_zoomH = height() - p_zoomY;
		if (p_zoomX < 0)
		{
			p_zoomX = 0;
			p_zoomW = (int)rint(r.right() * p_lastFactorX);
		}
		if (p_zoomY < 0)
		{
			p_zoomY = 0;
			p_zoomH = (int)rint(r.bottom() * p_lastFactorY);
		}
		
		p_zooming = false;
		
		if (p_zoomW > 1 && p_zoomH > 1)
		{
			double factorX, factorY;
			
			p_zoomedImageShown = p_originalImage.copy(p_zoomX, p_zoomY, p_zoomW, p_zoomH);
			p_zoomedImageShown = p_zoomedImageShown.scale(size());
			setPaletteBackgroundPixmap(p_zoomedImageShown);
			
			factorX = (double)p_zoomW / width();
			factorY = (double)p_zoomH / height();
			
			setMaximumSize((int)rint(p_originalImage.width() / factorX), (int)rint(p_originalImage.height() / factorY));
			
			p_lastFactorX = factorX;
			p_lastFactorY = factorY;
			
			emit setMoveActionEnabled(true);
		}
	}
	else if (p_moving)
	{
		unsetCursor();
		p_moving = false;
	}
}

void mapWidget::resizeEvent(QResizeEvent *e)
{
	p_zoomW = (int)rint(e -> size().width() * p_lastFactorX);
	p_zoomH = (int)rint(e -> size().height() * p_lastFactorY);
	
	p_zoomedImageShown = p_originalImage.copy(p_zoomX, p_zoomY, p_zoomW, p_zoomH);
	p_zoomedImageShown = p_zoomedImageShown.scale(size());
	setPaletteBackgroundPixmap(p_zoomedImageShown);
	kapp -> processEvents();
	
	emitMoveActionEnabled();
}

void mapWidget::emitMoveActionEnabled()
{
	if (p_zoomW < maximumWidth() * p_lastFactorX || p_zoomH < maximumHeight() * p_lastFactorY)
	{
		emit setMoveActionEnabled(true);
	}
	else
	{
		emit setMoveActionEnabled(false);
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
	p_zoomedImageShown = QImage();
	p_lastFactorX = 1;
	p_lastFactorY = 1;
	p_zoomX = 0;
	p_zoomY = 0;
	
	if (p_zoomH != 0 && p_zoomW != 0)
	{
		/* setting the original image not when opening the app */
		p_zoomW = width();
		p_zoomH = height();
	}
	
	setMaximumSize(p_originalImage.size());
	emitMoveActionEnabled();
}

#include "mapwidget.moc"
