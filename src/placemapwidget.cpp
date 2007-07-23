/***************************************************************************
 *   Copyright (C) 2004-2007 by Albert Astals Cid                          *
 *   aacid@kde.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "placemapwidget.h"

#include <QCursor>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QScrollBar>
#include <QTimer>

#include <klocale.h>

#include "division.h"

placeMapWidget::placeMapWidget(QWidget *parent) : QGraphicsView(parent)
{
	p_mode = None;
	p_zoomRect = 0;
	p_automaticZoom = false;
	p_mapImage = 0;
	p_gameImage = 0;
	p_currentCursor = 0;
	
	setCacheMode( CacheBackground );
	p_scene = new QGraphicsScene( this );
	setScene(p_scene);
}

placeMapWidget::~placeMapWidget()
{
	delete p_currentCursor;
	delete p_gameImage;
}

void placeMapWidget::init(KGmap *map, QImage *mapImage)
{
        p_map = map;
	p_mapImage = mapImage;
        createGameMapImage();
	p_scene->setSceneRect( p_gameImage->rect() );
	resetCachedContent();
	setGameImage();
	
	// work around bug in QGraphicsView?
	QTimer::singleShot( 0, this, SLOT(setGameImage()) );
}

void placeMapWidget::createGameMapImage()
{
	QList<QRgb> colorsToCopy;

	p_gameImage = new QImage(p_mapImage->size(), QImage::Format_RGB32);
	p_gameImage->fill(QColor(255,255,255).rgb());

	QList<division*> ignoredDivisions = p_map->getIgnoredDivisions(division::eClick);
	foreach(division *id, ignoredDivisions)
	{
		QRgb color = id->getRGB();
		// do not include black (used for frontiers)
		if (color != qRgb(0,0,0)) {
			colorsToCopy << id->getRGB();
		}
	}

	int width = p_mapImage->width();
	int height = p_mapImage->height();
	
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if(colorsToCopy.contains(p_mapImage->pixel(x,y)))
			{
				p_gameImage->setPixel(x,y,p_mapImage->pixel(x,y));
			}
		}
	}
}

void placeMapWidget::setMapMove(bool b)
{
	if (b)
		p_mode = WantMove;
	else if ( p_mode == WantMove || p_mode == Moving )
		p_mode = None;
	updateActions();
}

void placeMapWidget::setMapZoom(bool b)
{
	if (b)
		p_mode = WantZoom;
	else if ( p_mode == WantZoom || p_mode == Zooming )
		p_mode = None;
	updateActions();
}

void placeMapWidget::setCurrentDivisionImage(QImage *divisionImage)
{
	p_currentDivisionImage = divisionImage;
	updateCursor();
}

void placeMapWidget::updateCursor()
{
	unsetCursor();
	delete p_currentCursor;
	if (matrix().isIdentity())
	{
		p_currentCursor = new QCursor(QPixmap::fromImage(*p_currentDivisionImage));
	}
	else
	{
		QImage scaledDivisionImage = p_currentDivisionImage->scaled(static_cast<int>(p_currentDivisionImage->width() * matrix().m11()),static_cast<int>(p_currentDivisionImage->height() * matrix().m22()));
		p_currentCursor = new QCursor(QPixmap::fromImage(scaledDivisionImage));
	}
	setCursor(*p_currentCursor);
}

void placeMapWidget::placeDivision(QImage *divisionImage, QRect& position)
{
	QImage copied = divisionImage->copy();	
	QPainter painter( p_gameImage );
	painter.drawImage( position.topLeft(), copied );
	resetCachedContent();
}

void placeMapWidget::drawBackground(QPainter *painter, const QRectF &_rect)
{
	QRect rect = _rect.toRect().adjusted( -2, -2, 2, 2 ) & p_gameImage->rect();
	
	QImage copied = p_gameImage->copy( rect );
	painter->drawImage( rect.topLeft(), copied );
}

void placeMapWidget::mousePressEvent(QMouseEvent *e)
{
	p_initial = mapToScene( e->pos() );
	
	if (e -> button() == Qt::LeftButton)
	{
		if ( p_mode == WantZoom )
		{
			p_zoomRect = p_scene->addRect( QRectF( p_initial, QSize( 0, 0 ) ) );
			p_mode = Zooming;
		}
		else if ( p_mode == WantMove )
		{
			p_prev = e->pos();
			setCursor(Qt::SizeAllCursor);
			p_mode = Moving;
		}
		else
		{
			if ( QRectF(p_gameImage->rect()).contains( p_initial ) )
			{
				QRgb rgb = p_mapImage->pixel( int(p_initial.x()), int(p_initial.y()) );
				emit clicked( rgb, e->pos(), p_initial - p_gameImage->rect().topLeft() );
			}
		}
	}
	else if ( p_mode == WantZoom )
	{
		setGameImage();
	}
	else e->ignore(); // that makes the event go to mapasker and clear the popup
	
	updateActions();
}

void placeMapWidget::mouseMoveEvent(QMouseEvent *e)
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

void placeMapWidget::mouseReleaseEvent(QMouseEvent *)
{
	if ( p_mode == Zooming )
	{
		p_automaticZoom = false;
		fitInView( p_zoomRect, Qt::KeepAspectRatio );
		updateCursor();
		delete p_zoomRect;
		p_zoomRect = 0;
		
		p_mode = WantZoom;
	}
	else if ( p_mode == Moving )
	{
		unsetCursor();
		p_mode = WantMove;
	}
}

void placeMapWidget::resizeEvent(QResizeEvent *)
{
	resetCachedContent();
	updateZoom();
	updateActions();
	
	// Another hack to work around buginess in QGraphicsView
	if ( matrix().isIdentity() )
		QTimer::singleShot( 0, this, SLOT(setGameImage()) );
}

void placeMapWidget::setAutomaticZoom(bool automaticZoom)
{
	
	if (!automaticZoom) setGameImage();
	else
	{
		p_automaticZoom = true;
		updateZoom();
		updateActions();
	}
}

void placeMapWidget::setGameImage()
{
	p_automaticZoom = false;
	
	// Possibly bug in QGraphicsView? The view isn't updated properly
	// if the matrix isn't set to something non-identity first
	setMatrix( QMatrix( 2, 0, 0, 2, 0, 0 ) );
	resetMatrix();
	updateCursor();
	
	resetCachedContent();
	updateActions();
}

void placeMapWidget::updateZoom()
{
	if ( !p_automaticZoom )
		return;
	fitInView( p_gameImage->rect(), Qt::KeepAspectRatio );
	updateCursor();
}

QSize placeMapWidget::mapSize() const
{
	return p_gameImage->size();
}

void placeMapWidget::updateActions()
{
	if(p_gameImage)
	{
		// Whether the image is bigger than that viewable
		bool biggerThanView = (p_gameImage->width() * matrix().m11() >= width()) || (p_gameImage->height() * matrix().m22() >= height());
	
		emit setMoveActionEnabled( !p_automaticZoom && biggerThanView );
		emit setMoveActionChecked( !p_automaticZoom && (p_mode == Moving || p_mode == WantMove) && biggerThanView );
	}
	emit setZoomActionChecked( p_mode == Zooming || p_mode == WantZoom );
}

#include "placemapwidget.moc"
