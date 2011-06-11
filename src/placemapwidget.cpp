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

#include <QColormap>
#include <QCursor>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QScrollBar>

#include <klocale.h>
#include <kdebug.h>
#include <math.h>

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
	
	// work around bug in QGraphicsView?
	QMetaObject::invokeMethod(this, "setAutomaticZoom", Qt::QueuedConnection, Q_ARG(bool, p_automaticZoom));
}

size_t placeMapWidget::nbPixels(int pixi) const
{
	return p_pixelsStats[pixi];
}

static int indexOfPair(uchar pixiMin, uchar pixiMax)
{
	const int pairIdx = pixiMin + pixiMax * (pixiMax - 1) / 2;
	return pairIdx;
}


size_t placeMapWidget::nbBorderPixels(int pixi1, int pixi2) const
{
	int pairIdx = indexOfPair(qMin(pixi1, pixi2), qMax(pixi1, pixi2));
	return p_bordersStats[pairIdx];
}

static void addOrderedPixel(QVector<uchar> *pixels, uchar pixi)
{
	int i = pixels->size();
	do { --i;} while ( i >= 0 && pixels->at(i) > pixi );;
	if ( i < 0 || pixels->at(i) < pixi )
			pixels->insert(i+1, pixi);
}

QString writeUpBorderStats(const QVector<size_t> &stats, const QVector<size_t> &histo, const QVector<QRgb> &cmap)
{
	int nbCells = stats.size(); // = n * (n*1) / 2 => 2p == n2 + n => n2 + n -2p == 0
	// delta = 1 + 8p -> n = (-1 +/- sqrt(1 + 8p))/2
	int nbRows = (sqrt(1 + 8 * nbCells) -1) /2;
	QString ret;
	ret += "\n";
	ret.reserve(17 * nbCells + nbRows * 20);
	for ( int ic = 0 ; ic < nbRows ; ic++ )
		ret += QString("%1(%2,%3,%4):%5\n").arg(ic).arg(qRed(cmap[ic])).arg(qGreen(cmap[ic])).arg(qBlue(cmap[ic])).arg(histo[ic]);
	ret += "\n";
	for ( int m = 0 ; m < nbRows ; m++ ) {
		for ( int p = 0 ; p < m ; p++ )
			ret += QString("(%1, %2):%3 ").arg(m).arg(p).arg(stats[indexOfPair(p, m)]);
		ret += "\n";
	}
	return ret;
}

void placeMapWidget::createGameMapImage()
{
	QVector<uchar> indexesToCopy;
	const QVector<QRgb> colormap = p_mapImage->colorTable();
	p_gameImage = new QImage(p_mapImage->size(), QImage::Format_RGB32);
	// So far, nobody has dedicated this color to a division :)
	// I, for one, reserve grays for non-division pixels.
	p_gameImage->fill(QColor(224,224,224).rgb());

	QList<const division*> ignoredDivisions = p_map->getIgnoredDivisions(division::eClick);
	foreach(const division *id, ignoredDivisions)
	{
		const QRgb rgb = id->getRGB();
		const int colorIdx = colormap.indexOf(rgb);
		indexesToCopy << colorIdx;
	}

	const int nbBytesPerLine = p_mapImage->bytesPerLine();
	const uchar *bits = p_mapImage->bits();

	const int width = p_mapImage->width();
	const int height = p_mapImage->height();
	const int deltaX[] = {-1,  0,  1,  1,  1,  0, -1, -1};
	const int deltaY[] = {-1, -1, -1,  0,  1,  1,  1,  0};

	const size_t nbColors = p_mapImage->colorTable().size();
	p_pixelsStats.resize(nbColors);
	p_bordersStats.resize(nbColors * (nbColors +1) / 2);
	
	for (int x = 1; x < width -1; x++)
	{
		for (int y = 1; y < height -1; y++)
		{
			const uchar pixi = bits[y * nbBytesPerLine + x];
			p_pixelsStats[pixi] += 1;

			if(indexesToCopy.contains(pixi) )
			{
				QVector<uchar> orderedNeighbours;
				bool outerFound = false;
				bool divisionColorFound = false;
				for ( int neighbourIdx = 0 ;
					  neighbourIdx < 8 ;
					  neighbourIdx ++ )
				{
					const int ox = x + deltaX[neighbourIdx];
					const int oy = y + deltaY[neighbourIdx];
					const uchar oPixi = bits[oy * nbBytesPerLine + ox];
					if (oPixi != pixi)
					{
						addOrderedPixel(&orderedNeighbours, oPixi);
						if ( indexesToCopy.contains(oPixi) )
							outerFound = true;
						else
							divisionColorFound = true;
					}
				}

				if ( outerFound || ! divisionColorFound )
					p_gameImage->setPixel(x,y,p_mapImage->pixel(x,y));

				if ( ! divisionColorFound )
					continue;
				for ( int maxIdx = orderedNeighbours.size() - 1 ; --maxIdx >= 0 ; )
				{
					for ( int minIdx = maxIdx ; minIdx >= 0 ; minIdx-- )
					{
						const uchar pixiMin = orderedNeighbours[minIdx];
						const uchar pixiMax = orderedNeighbours[maxIdx + 1];
						const int pairIdx = indexOfPair(pixiMin, pixiMax);
						p_bordersStats[pairIdx] += 1;
					}
				}
			}
		}
	}
	p_outerPixis = indexesToCopy;
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
		QImage scaledDivisionImage = p_currentDivisionImage->scaled(static_cast<int>(p_currentDivisionImage->width() * matrix().m11()),
																	static_cast<int>(p_currentDivisionImage->height() * matrix().m22()));
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
			if ( QRectF(p_gameImage->rect()).contains( p_initial ) )
			{
				QRgb rgb = p_mapImage->pixel( int(p_initial.x()), int(p_initial.y()) );
				emit clicked( rgb, e->pos(), p_initial - p_gameImage->rect().topLeft() );
			}
		}
	}
	else if (e -> button() == Qt::MidButton)
	{
		p_modeBeforeMidClick = p_mode;
		p_mode = WantMove;
		updateActions();
		p_prev = e->pos();
		//setCursor(Qt::SizeAllCursor);
		p_mode = Moving;
		updateActions();
	}
	else if ( p_mode == WantZoom )
	{
		setGameImage();
		updateActions();
	}
	else e->ignore(); // that makes the event go to mapasker and clear the popup
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

void placeMapWidget::mouseReleaseEvent(QMouseEvent *e)
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
		if ( e->button() != Qt::MidButton)
			unsetCursor();
		p_mode = p_modeBeforeMidClick;
	}
}

void placeMapWidget::resizeEvent(QResizeEvent *)
{
	resetCachedContent();
	updateZoom();
	updateActions();
	
	// Another hack to work around buginess in QGraphicsView
	if ( matrix().isIdentity() )
		QMetaObject::invokeMethod(this, "setAutomaticZoom", Qt::QueuedConnection, Q_ARG(bool, p_automaticZoom));
}

void placeMapWidget::wheelEvent(QWheelEvent *e)
{
	if ( e->modifiers() == Qt::NoModifier ) QGraphicsView::wheelEvent(e);
	else if ( e->modifiers()  == Qt::ShiftModifier )
	{
		QWheelEvent reorientedEvent(e->pos(), e->delta(), e->buttons(), Qt::NoModifier, Qt::Orientation(3 -int(e->orientation())));
		QGraphicsView::wheelEvent(&reorientedEvent);
	}
	else if ( e->modifiers()  == Qt::ControlModifier )
	{
		int delta = e->delta();
		if ( delta != 0 )
		{
			qreal rescale = pow(2, qreal(delta/120)/2.0);
			QMatrix m(rescale, 0, 0, rescale, 0, 0);
			setMatrix(m, true);
		}
	}
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
	if ( !p_automaticZoom || !p_gameImage )
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
