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

#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QScrollBar>

#include <KLocalizedString>
#include <math.h>

mapWidget::mapWidget(QWidget *parent) : QGraphicsView(parent)
{
	p_mode = None;
	p_zoomRect = 0;
	p_automaticZoom = false;

	setCacheMode( CacheBackground );
	p_scene = new QGraphicsScene( this );
	setScene(p_scene);
	setViewportUpdateMode(SmartViewportUpdate);
}

void mapWidget::init(const QImage &mapImage)
{
	p_originalImage = mapImage;
	p_scene->clear();
	resetCachedContent();
	p_scene->addPixmap(QPixmap::fromImage(p_originalImage));
	p_scene->setSceneRect( p_originalImage.rect() );

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
	else if (e -> button() == Qt::MiddleButton)
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
		if ( e -> button() == Qt::MiddleButton )
		{
			p_mode = p_modeBeforeMidClick;
			updateActions();
		}
		else p_mode = WantMove;
	}
}

void mapWidget::resizeEvent(QResizeEvent *)
{
	updateZoom();
	updateActions();

	// Another hack to work around buginess in QGraphicsView
	if ( transform().isIdentity() )
	{
		QMetaObject::invokeMethod(this, "setAutomaticZoom", Qt::QueuedConnection, Q_ARG(bool, p_automaticZoom));
	}
}

void mapWidget::wheelEvent(QWheelEvent *e)
{
	if ( e->modifiers() == Qt::NoModifier ) QGraphicsView::wheelEvent(e);
	else if ( e->modifiers()  == Qt::ShiftModifier )
	{
		// Scroll horizontally by swapping x and y for the delta
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		QWheelEvent reorientedEvent(e->position(), e->globalPosition(), e->pixelDelta().transposed(), -e->angleDelta().transposed(), e->buttons(), Qt::NoModifier, e->phase(), e->inverted(), e->source(), e->pointingDevice());
#else
		QWheelEvent reorientedEvent(e->position(), e->globalPosition(), e->pixelDelta().transposed(), e->angleDelta().transposed(), e->buttons(), Qt::NoModifier, e->phase(), e->inverted(), e->source());
#endif
		QGraphicsView::wheelEvent(&reorientedEvent);
	}
	else if ( e->modifiers()  == Qt::ControlModifier )
	{
		int delta = e->angleDelta().y();
		if ( delta != 0 )
		{
			const qreal rescale = pow(2, qreal(delta/120)/2.0);
			scale(rescale, rescale);
		}
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
	// if the transform isn't set to something non-identity first
	// Or probably a bug in this code, but this makes it show nicely centered
	// in the view if the widget is bigger than the map
	setTransform( QTransform( 2, 0, 0, 2, 0, 0 ) );
	resetTransform();

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
	const bool biggerThanView = (p_originalImage.width() * transform().m11() >= width()) || (p_originalImage.height() * transform().m22() >= height());
	
	emit setMoveActionEnabled( !p_automaticZoom && biggerThanView );
	emit setMoveActionChecked( !p_automaticZoom && (p_mode == Moving || p_mode == WantMove) && biggerThanView );
	emit setZoomActionChecked( p_mode == Zooming || p_mode == WantZoom );
}

#include "moc_mapwidget.cpp"
