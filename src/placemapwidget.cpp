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

#include <KLocalizedString>
#include <math.h>

#include "division.h"

placeMapWidget::placeMapWidget(QWidget *parent) : QGraphicsView(parent)
	, p_mode(None)
	, p_mapImage(nullptr)
	, p_gameImage(nullptr)
	, p_currentCursor(nullptr)
	, p_zoomRect(nullptr)
	, p_automaticZoom(false)
	, p_currentDivisionItem(nullptr)
	, lastMouseEvent(QPoint(0,0))
{
	setCacheMode( CacheBackground );
	p_scene = new QGraphicsScene( this );
	p_scene->setBackgroundBrush(Qt::white);
	setScene(p_scene);
	p_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
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
	p_scene->clear();
	p_scene->addPixmap(QPixmap::fromImage(*p_gameImage));
	p_scene->setSceneRect( p_gameImage->rect() );
	setMouseTracking(true);

	// work around bug in QGraphicsView?
	QMetaObject::invokeMethod(this, "setAutomaticZoom", Qt::QueuedConnection, Q_ARG(bool, p_automaticZoom));
}

size_t placeMapWidget::nbPixels(int pixelIndex) const
{
	return p_pixelsStats[pixelIndex];
}

static int indexOfPair(uchar pixelIndexMin, uchar pixelIndexMax)
{
	const int pairIndex = pixelIndexMin + pixelIndexMax * (pixelIndexMax - 1) / 2;
	return pairIndex;
}


size_t placeMapWidget::nbBorderPixels(int pixelIndex1, int pixelIndex2) const
{
	int pairIndex = indexOfPair(qMin(pixelIndex1, pixelIndex2), qMax(pixelIndex1, pixelIndex2));
	return p_bordersStats[pairIndex];
}

static void addOrderedPixel(QVector<uchar> *pixels, uchar pixelIndex)
{
	int i = pixels->size();
	do { --i;} while ( i >= 0 && pixels->at(i) > pixelIndex );;
	if ( i < 0 || pixels->at(i) < pixelIndex )
			pixels->insert(i+1, pixelIndex);
}

QString writeUpBorderStats(const QVector<size_t> &stats, const QVector<size_t> &histo, const QVector<QRgb> &cmap)
{
	int nbCells = stats.size(); // = n * (n*1) / 2 => 2p == n2 + n => n2 + n -2p == 0
	// delta = 1 + 8p -> n = (-1 +/- sqrt(1 + 8p))/2
	int nbRows = (sqrt((double)1 + 8 * nbCells) -1) /2;
	QString ret;
	ret += QLatin1Char('\n');
	ret.reserve(17 * nbCells + nbRows * 20);
	for ( int ic = 0 ; ic < nbRows ; ic++ )
		ret += QStringLiteral("%1(%2,%3,%4):%5\n").arg(ic).arg(qRed(cmap[ic])).arg(qGreen(cmap[ic])).arg(qBlue(cmap[ic])).arg(histo[ic]);
	ret += QLatin1Char('\n');
	for ( int m = 0 ; m < nbRows ; m++ ) {
		for ( int p = 0 ; p < m ; p++ )
			ret += QStringLiteral("(%1, %2):%3 ").arg(m).arg(p).arg(stats[indexOfPair(p, m)]);
		ret += QLatin1Char('\n');
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
			const uchar pixelIndex = bits[y * nbBytesPerLine + x];
			p_pixelsStats[pixelIndex] += 1;

			if(indexesToCopy.contains(pixelIndex) )
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
					const uchar oPixelIndex = bits[oy * nbBytesPerLine + ox];
					if (oPixelIndex != pixelIndex)
					{
						addOrderedPixel(&orderedNeighbours, oPixelIndex);
						if ( indexesToCopy.contains(oPixelIndex) )
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
						const uchar pixelIndexMin = orderedNeighbours[minIdx];
						const uchar pixelIndexMax = orderedNeighbours[maxIdx + 1];
						const int pairIndex = indexOfPair(pixelIndexMin, pixelIndexMax);
						p_bordersStats[pairIndex] += 1;
					}
				}
			}
		}
	}
	p_outerPixelIndices = indexesToCopy;
}

void placeMapWidget::setMapMove(bool b)
{
	if (b)
	{
		p_mode = WantMove;
		setCursor(QCursor(Qt::OpenHandCursor));
	}
	else if ( p_mode == WantMove || p_mode == Moving )
	{
		p_mode = None;
		setCursor(QCursor(Qt::BlankCursor));
	}
	updateActions();
}

void placeMapWidget::setMapZoom(bool b)
{
	if (b)
	{
		p_mode = WantZoom;
	}
	else if ( p_mode == WantZoom || p_mode == Zooming )
	{
		p_mode = None;
	}
	updateActions();
}

void placeMapWidget::setCurrentDivisionImage(QImage *divisionImage)
{
	p_currentDivisionImage = divisionImage;
	// add the pixmap and set position to the middle of the pixmap under the mouse
	p_currentDivisionItem = p_scene->addPixmap(QPixmap::fromImage(*p_currentDivisionImage));
	p_currentDivisionItem->setFlag(QGraphicsItem::ItemIsMovable, true);
	QPoint p(lastMouseEvent.x()-p_currentDivisionImage->width()/2,lastMouseEvent.y()-p_currentDivisionImage->height()/2);
	p_currentDivisionItem->setPos(mapToScene(p));
	updateCursor();
}

void placeMapWidget::updateCursor()
{
	// enable the normal cursor over the scrollbars
	viewport()->setCursor(QCursor(Qt::BlankCursor));
}

void placeMapWidget::placeDivision(QRect& position)
{
	// TODO: An animation to position
	p_currentDivisionItem->setPos(position.topLeft());
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
				// check against the topleft corner, because the image is 1x1 size smaller than the image rectangle
				QPoint p(e->pos().x()-p_currentDivisionImage->width()/2,e->pos().y()-p_currentDivisionImage->height()/2);
				emit clicked( rgb, e->pos(), mapToScene(p));
			}
		}
	}
	else if (e -> button() == Qt::MiddleButton)
	{
		p_modeBeforeMidClick = p_mode;
		p_mode = WantMove;
		updateActions();
		p_prev = e->pos();
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
	lastMouseEvent = e->pos();
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
	if (p_currentDivisionItem)
	{
		QPoint p(e->pos().x()-p_currentDivisionImage->width()/2,e->pos().y()-p_currentDivisionImage->height()/2);
		p_currentDivisionItem->setPos(mapToScene(p));
	}
}

void placeMapWidget::mouseReleaseEvent(QMouseEvent *)
{
	if ( p_mode == Zooming )
	{
		p_automaticZoom = false;
		fitInView( p_zoomRect, Qt::KeepAspectRatio );
		delete p_zoomRect;
		p_zoomRect = nullptr;

		p_mode = WantZoom;
	}
	else if ( p_mode == Moving )
	{
		p_mode = p_modeBeforeMidClick;
	}
}

void placeMapWidget::resizeEvent(QResizeEvent *)
{
	updateZoom();
	updateActions();

	// Another hack to work around buginess in QGraphicsView
	if ( transform().isIdentity() )
		QMetaObject::invokeMethod(this, "setAutomaticZoom", Qt::QueuedConnection, Q_ARG(bool, p_automaticZoom));
}

void placeMapWidget::wheelEvent(QWheelEvent *e)
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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void placeMapWidget::enterEvent(QEnterEvent*)
#else
void placeMapWidget::enterEvent(QEvent*)
#endif
{
	if (p_currentDivisionItem)
	{
		p_currentDivisionItem->show();
	}
}

void placeMapWidget::leaveEvent(QEvent*)
{
	if (p_currentDivisionItem)
	{
		p_currentDivisionItem->hide();
	}
}


void placeMapWidget::setAutomaticZoom(bool automaticZoom)
{
	if (!automaticZoom)
	{
		setGameImage();
	}
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
	setTransform( QTransform( 2, 0, 0, 2, 0, 0 ) );
	resetTransform();
	updateActions();
}

void placeMapWidget::updateZoom()
{
	if ( !p_automaticZoom || !p_gameImage )
		return;
	fitInView( p_gameImage->rect(), Qt::KeepAspectRatio );
}

QSize placeMapWidget::mapSize() const
{
	return p_gameImage->size();
}

void placeMapWidget::updateActions()
{
	if(p_gameImage)
	{
		if ( p_mode != Zooming && p_mode != WantZoom )
		{
		  p_currentDivisionItem->show();
		}
		else
		{
		  p_currentDivisionItem->hide();
		}
		// Whether the image is bigger than that viewable
		bool biggerThanView = (p_gameImage->width() * transform().m11() >= width()) || (p_gameImage->height() * transform().m22() >= height());

		emit setMoveActionEnabled( !p_automaticZoom && biggerThanView );
		emit setMoveActionChecked( !p_automaticZoom && (p_mode == Moving || p_mode == WantMove) && biggerThanView );
	}

	emit setZoomActionChecked( p_mode == Zooming || p_mode == WantZoom );
}

#include "moc_placemapwidget.cpp"
