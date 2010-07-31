/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <klocale.h>
#include <kmessagebox.h>

#include <tqlabel.h>
#include <tqlayout.h>
#include <tqscrollbar.h>
#include <tqstring.h>

#include "map.h"
#include "mapasker.h"
#include "mapwidget.h"

mapAsker::mapAsker(TQWidget *parent, KGmap *m, TQWidget *w, bool asker, uint count) : askWidget(parent, m, w, count, asker), p_popupManager(this), p_asker(asker), p_firstShow(true)
{
	TQGridLayout *lay = new TQGridLayout(this, 2, 2);

	p_mapWidget = new mapWidget(this);
	lay -> addWidget(p_mapWidget, 0, 0);

	p_hsb = new TQScrollBar(Horizontal, this);
	lay -> addWidget(p_hsb, 1, 0);
	p_vsb = new TQScrollBar(Vertical, this);
	lay -> addWidget(p_vsb, 0, 1);

	lay -> setRowStretch(2, 1);
	lay -> setColStretch(2, 1);

	p_shouldClearPopup = false;

	connect(p_mapWidget, TQT_SIGNAL(clicked(QRgb, const TQPoint&)), this, TQT_SLOT(handleMapClick(QRgb, const TQPoint&)));
	connect(p_mapWidget, TQT_SIGNAL(setMoveActionChecked(bool)), this, TQT_SIGNAL(setMoveActionChecked(bool)));
	connect(p_mapWidget, TQT_SIGNAL(setZoomActionChecked(bool)), this, TQT_SIGNAL(setZoomActionChecked(bool)));
	connect(p_mapWidget, TQT_SIGNAL(setMoveActionEnabled(bool)), this, TQT_SIGNAL(setMoveActionEnabled(bool)));
	connect(p_mapWidget, TQT_SIGNAL(setMoveActionEnabled(bool)), this, TQT_SLOT(showScrollBars(bool)));
	connect(p_mapWidget, TQT_SIGNAL(updatePosition(int, int)), this, TQT_SLOT(setScrollBarsPosition(int, int)));
	connect(p_mapWidget, TQT_SIGNAL(updateVisibleSize(int, int)), this, TQT_SLOT(setScrollBarsVisibleSize(int, int)));
	connect(p_mapWidget, TQT_SIGNAL(updateMaximumSize(int, int)), this, TQT_SLOT(setScrollBarsMaximumSize(int, int)));

	connect(p_hsb, TQT_SIGNAL(valueChanged(int)), p_mapWidget, TQT_SLOT(updateHPosition(int)));
	connect(p_vsb, TQT_SIGNAL(valueChanged(int)), p_mapWidget, TQT_SLOT(updateVPosition(int)));

	if (asker)
	{
		TQBoxLayout *vbl = dynamic_cast<TQBoxLayout*>(w -> layout());
		p_next = new TQLabel(w);
		p_next -> setAlignment(AlignTop | AlignHCenter);
		p_fill = new TQWidget(w);
		p_fill -> show();
		vbl -> setStretchFactor(p_fill, 1);
		nextQuestion();
	}
	else
	{
		p_next = 0;
		p_fill = 0;
	}
}

mapAsker::~mapAsker()
{
	delete p_next;
	delete p_fill;
}

bool mapAsker::isAsker() const
{
	return p_answers;
}

void mapAsker::mousePressEvent(TQMouseEvent*)
{
	p_popupManager.clear();
}

void mapAsker::setMovement(bool b)
{
	p_mapWidget -> setMapMove(b);
	p_popupManager.clear();
}

void mapAsker::setZoom(bool b)
{
	p_mapWidget -> setMapZoom(b);
	p_popupManager.clear();
}

void mapAsker::setOriginalZoom()
{
	p_mapWidget -> setOriginalImage();
	p_popupManager.clear();
}

void mapAsker::handleMapClick(QRgb c, const TQPoint &p)
{
	TQString aux, cap;
	aux = p_map -> getWhatIs(c, !p_asker);
	if (aux == "nothing") KMessageBox::error(this, i18n("You have found a bug in a map. Please contact the author and tell the %1 map has nothing associated to color %2,%3,%4.").arg(p_map -> getFile()).arg(qRed(c)).arg(qGreen(c)).arg(qBlue(c)));
	else if (p_shouldClearPopup)
	{
		p_popupManager.clear();
		p_shouldClearPopup = false;
	}
	else if (!p_asker)
	{
		TQString flagFile;
		cap = p_map -> getDivisionCapital(aux);
		if (!cap.isEmpty())
		{
			flagFile = p_map -> getDivisionFlagFile(aux);
			cap = i18n(p_map -> getFileName().utf8(), cap.utf8());
		}

		aux = i18n(p_map -> getFileName().utf8(), aux.utf8());

		if (!flagFile.isEmpty()) p_popupManager.show(aux, cap, p, flagFile);
		else if (!cap.isEmpty()) p_popupManager.show(aux, cap, p);
		else p_popupManager.show(aux, p);
	}
	else if (!aux.isEmpty())
	{
		p_currentAnswer.setAnswer(TQColor(c));
		questionAnswered(aux == lastDivisionAsked());
		nextQuestion();
	}
}

void mapAsker::showScrollBars(bool b)
{
	if (b)
	{
		p_hsb -> show();
		p_vsb -> show();
	}
	else
	{
		p_hsb -> hide();
		p_vsb -> hide();
	}
}

void mapAsker::setScrollBarsPosition(int x, int y)
{
	p_hsb -> setValue(x);
	p_vsb -> setValue(y);
}

void mapAsker::setScrollBarsVisibleSize(int w, int h)
{
	p_hsb -> setMaxValue(p_hsbms - w);
	p_hsb -> setPageStep(w);
	p_vsb -> setMaxValue(p_vsbms - h);
	p_vsb -> setPageStep(h);
}

void mapAsker::setScrollBarsMaximumSize(int w, int h)
{
	p_hsbms = w;
	p_vsbms = h;
}

void mapAsker::nextQuestionHook(const TQString &division)
{
	TQString sw = i18n("There are two ways of dealing with the translation of \"Please click on: %1\". The first option simply replaces %1 with the translated name of the relevant region/city. If the grammar of your language allows this, choose this option by setting the translation of this message to 1, and leave untranslated the translations of \"Please click on: %1\" that have the placename embedded (or translate them as - if you wish to show the file as fully translated. The second option is to translate all messages in full - this is likely to be required in the case of highly-inflected languages like Russian. To choose this option, set the translation of this message to 0, and translate all the messages.", "0");
	if (sw == "1")
	{
		TQString divisionName = i18n(p_map -> getFileName().utf8(), division.utf8());
		TQString text = i18n("Please click on:\n%1");
		p_next -> setText(text.arg(divisionName));
	}
	else
	{
		TQString s = TQString("Please click on:\n%1").arg(division);
		p_next -> setText(i18n(p_map -> getFileName().utf8(), s.utf8()));
	}
	p_currentAnswer.setQuestion(i18n(p_map -> getFileName().utf8(), division.utf8()));
	p_next -> show();
	p_currentAnswer.setCorrectAnswer(p_map -> getColor(division));
}

TQString mapAsker::getQuestionHook() const
{
	return i18n("Division in Map");
}

void mapAsker::showEvent(TQShowEvent *)
{
	if (p_firstShow)
	{
		p_mapWidget -> init(p_map -> getMapFile(), p_vsb->width(), p_hsb->height());
		p_firstShow = false;
	}
}

TQSize mapAsker::mapSize() const
{
	return p_mapWidget -> mapSize();
}

#include "mapasker.moc"
