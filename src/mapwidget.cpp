/***************************************************************************
 *   Copyright (C) 2004-2007 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "mapwidget.h"

#include <QCursor>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QScrollBar>
#include <QTimer>

#include <kdebug.h>
#include <klocale.h>

mapWidget::mapWidget(QWidget *parent) : QGraphicsView(parent)
{
	p_mode = None;
	p_zoomRect = 0;
	p_automaticZoom = false;
	
	setCacheMode( CacheBackground );
	p_scene = new QGraphicsScene( this );
	setScene(p_scene);
}

void mapWidget::init(const QString &path)
{
	p_originalImage.load(path);
	p_originalPixmap.load(path);
	p_scene->setSceneRect( p_originalImage.rect() );
	resetCachedContent();
	
	// work around bug in QGraphicsView?
	QMetaObject::invokeMethod(this, "setAutomaticZoom", Qt::QueuedConnection, Q_ARG(bool, p_automaticZoom));
}

void mapWidget::setMapMove(bool b)
{
	if (b)
		p_mode = WantMove;
	else if ( p_mode == WantMove || p_mode == Moving )
		p_mode = None;
	updateActions();
}

void mapWidget::setMapZoom(bool b)
{
	if (b)
		p_mode = WantZoom;
	else if ( p_mode == WantZoom || p_mode == Zooming )
		p_mode = None;
	updateActions();
}

void mapWidget::drawBackground(QPainter *painter, const QRectF &_rect)
{
	QRect rect = _rect.toRect().adjusted( -2, -2, 2, 2 ) & p_originalImage.rect();
	
	if ( painter->matrix().m11() == 1 && painter->matrix().m22() == 1 )
	{
		// Image won't need to be scaled; use pixmap for faster drawing
		painter->drawPixmap( rect.topLeft(), p_originalPixmap, rect );
	}
	else
	{
		QImage copied = p_originalImage.copy( rect );
		painter->setRenderHint( QPainter::SmoothPixmapTransform );
		painter->drawImage( rect.topLeft(), copied );
	}
}

void mapWidget::mousePressEvent(QMouseEvent *e)
{
	p_initial = mapToScene( e->pos() );
	
	if (e -> button() == Qt::LeftButton)
	{
		if ( p_mode == WantZoom )
		{
			p_zoomRect = p_scene->addRect( QRectF( p_initial, QSize( 0, 0 ) ) );
			p_mode = Zooming;
			updateActions();
		}
		else if ( p_mode == WantMove )
		{
			p_prev = e->pos();
			setCursor(Qt::SizeAllCursor);
			p_mode = Moving;
			updateActions();
		}
		else
		{
			if ( QRectF(p_originalImage.rect()).contains( p_initial ) )
			{
				QRgb rgb = p_originalImage.pixel( int(p_initial.x()), int(p_initial.y()) );
				emit clicked( rgb, e->pos() );
			}
		}
	}
	else if (e -> button() == Qt::MidButton)
	{
		p_modeBeforeMidClick = p_mode;
		p_mode = WantMove;
		updateActions();
		p_prev = e->pos();
		setCursor(Qt::SizeAllCursor);
		p_mode = Moving;
		updateActions();
	}
	else if ( p_mode == WantZoom )
	{
		setOriginalImage();
		updateActions();
	}
	else e->ignore(); // that makes the event go to mapasker and clear the popup
}

void mapWidget::mouseMoveEvent(QMouseEvent *e)
{
	if ( p_mode == Zooming )
	{
		QPointF current = mapToScene( e->pos() );
		
		QRectF r;
		r.setTopLeft( p_initial );
		r.setBottomRight( current );
		p_zoomRect->setRect( r.normalized() );
	}
	else if ( p_mode == Moving )
	{
		QPoint diff = p_prev - e->pos();
		
		horizontalScrollBar()->setValue( horizontalScrollBar()->value() + diff.x() );
		verticalScrollBar()->setValue( verticalScrollBar()->value() + diff.y() );
		
		p_prev = e->pos();
	}
}

void mapWidget::mouseReleaseEvent(QMouseEvent *e)
{
	if ( p_mode == Zooming )
	{
		p_automaticZoom = false;
		fitInView( p_zoomRect, Qt::KeepAspectRatio );
		delete p_zoomRect;
		p_zoomRect = 0;
		
		p_mode = WantZoom;
	}
	else if ( p_mode == Moving )
	{
		unsetCursor();
		if ( e -> button() == Qt::MidButton )
		{
			p_mode = p_modeBeforeMidClick;
			updateActions();
		}
		else p_mode = WantMove;
	}
}

void mapWidget::resizeEvent(QResizeEvent *)
{
	resetCachedContent();
	updateZoom();
	updateActions();
	
	// Another hack to work around buginess in QGraphicsView
	if ( matrix().isIdentity() )
	{
		QMetaObject::invokeMethod(this, "setAutomaticZoom", Qt::QueuedConnection, Q_ARG(bool, p_automaticZoom));
	}
}

void mapWidget::setAutomaticZoom(bool automaticZoom)
{
	if (!automaticZoom) setOriginalImage();
	else
	{
		p_automaticZoom = automaticZoom;
		updateZoom();
		updateActions();
	}
}

void mapWidget::setOriginalImage()
{
	p_automaticZoom = false;
	
	// Possibly bug in QGraphicsView? The view isn't updated properly
	// if the matrix isn't set to something non-identity first
	setMatrix( QMatrix( 2, 0, 0, 2, 0, 0 ) );
	resetMatrix();
	
	resetCachedContent();
	updateActions();
}

void mapWidget::updateZoom()
{
	if ( !p_automaticZoom )
		return;
	fitInView( p_originalImage.rect(), Qt::KeepAspectRatio );
}

QSize mapWidget::mapSize() const
{
	return p_originalImage.size();
}

void mapWidget::updateActions()
{
	// Whether the image is bigger than that viewable
	bool biggerThanView = (p_originalImage.width() * matrix().m11() >= width()) || (p_originalImage.height() * matrix().m22() >= height());
	
	emit setMoveActionEnabled( !p_automaticZoom && biggerThanView );
	emit setMoveActionChecked( !p_automaticZoom && (p_mode == Moving || p_mode == WantMove) && biggerThanView );
	emit setZoomActionChecked( p_mode == Zooming || p_mode == WantZoom );
}

#include "mapwidget.moc"
