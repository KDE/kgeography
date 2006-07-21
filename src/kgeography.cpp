/***************************************************************************
 *   Copyright (C) 2004-2005 by Albert Astals Cid                          *
 *   tsdgeos@terra.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <kaction.h>
#include <kdialog.h>
#include <klocale.h>
#include <kinputdialog.h>
#include <kmessagebox.h>
#include <kpushbutton.h>
#include <kstdaction.h>
#include <ktoggleaction.h>
#include <ktoolbar.h>
#include <kmenubar.h>

#include <qlabel.h>
#include <qlayout.h>
#include <qsize.h>
#include <qtimer.h>

#include "answersdialog.h"
#include "capitaldivisionasker.h"
#include "divisioncapitalasker.h"
#include "divisionflagasker.h"
#include "flagdivisionasker.h"
#include "kgeography.h"
#include "settings.h"
#include "mapasker.h"
#include "mapchooser.h"
#include "mapparser.h"
#include "map.h"

kgeography::kgeography() : KMainWindow(), p_firstShow(true), p_mustShowResultsDialog(false)
{
	p_map = 0;
	p_askWidget = 0;

	p_bigWidget = new QWidget(this);
	QHBoxLayout *bigWidgetLayout = new QHBoxLayout(p_bigWidget);
	bigWidgetLayout -> setSpacing(0);
	bigWidgetLayout -> setMargin(0);
	
	QWidget *leftWidget = new QWidget(p_bigWidget);
	QVBoxLayout *leftWidgetLayout = new QVBoxLayout(leftWidget);
	leftWidgetLayout -> setSpacing(0);
	leftWidgetLayout -> setMargin(0);

	p_currentMap = new QLabel(leftWidget);
	p_currentMap -> setAlignment(Qt::AlignCenter);
	p_consult = new KPushButton(i18n("&Browse Map"), leftWidget);
	p_consult->setWhatsThis(i18n("In this section left click on any part of the map to learn about the divisions" ));
	p_askMap = new KPushButton(i18n("&Click Division in Map..."), leftWidget);
	p_askMap->setWhatsThis(i18n("In this challenge you are given a division name on the left under the menu and you must find it on the map and click on it"));
	p_askCapitalDivisions = new KPushButton(i18n("Guess Division From Its &Capital..."), leftWidget);
	p_askCapitalDivisions->setWhatsThis(i18n("In this quiz you have to guess the division name given its capital"));
	p_askDivisionCapitals = new KPushButton(i18n("Guess Capital of &Division..."), leftWidget);
	p_askDivisionCapitals->setWhatsThis(i18n("In this quiz you have to guess the capital of a given division name"));
	p_askFlagDivisions = new KPushButton(i18n("&Guess Division From Its Flag..."), leftWidget);
	p_askFlagDivisions->setWhatsThis(i18n("In this quiz you have to guess the division name given its flag"));
	p_askDivisionFlags = new KPushButton(i18n("G&uess Flag of Division..."), leftWidget);
	p_askDivisionFlags->setWhatsThis(i18n("In this quiz you have to guess the flag of a division given its name"));
	p_underLeftWidget = new QWidget(leftWidget);
	QVBoxLayout *underLeftWidgetLayout = new QVBoxLayout(p_underLeftWidget);
	underLeftWidgetLayout -> layout() -> setSpacing(KDialog::spacingHint());
	underLeftWidgetLayout -> layout() -> setMargin(KDialog::marginHint());
	leftWidgetLayout -> addWidget(p_currentMap);
	leftWidgetLayout -> addWidget(p_consult);
	leftWidgetLayout -> addWidget(p_askMap);
	leftWidgetLayout -> addWidget(p_askCapitalDivisions);
	leftWidgetLayout -> addWidget(p_askDivisionCapitals);
	leftWidgetLayout -> addWidget(p_askFlagDivisions);
	leftWidgetLayout -> addWidget(p_askDivisionFlags);
	leftWidgetLayout -> addWidget(p_underLeftWidget, 1);

	bigWidgetLayout -> addWidget(leftWidget);
	setCentralWidget(p_bigWidget);

	connect(p_consult, SIGNAL(clicked()), this, SLOT(consult()));
	connect(p_askMap, SIGNAL(clicked()), this, SLOT(askMap()));
	connect(p_askCapitalDivisions, SIGNAL(clicked()), this, SLOT(askCapitalDivisions()));
	connect(p_askDivisionCapitals, SIGNAL(clicked()), this, SLOT(askDivisionCapitals()));
	connect(p_askFlagDivisions, SIGNAL(clicked()), this, SLOT(askFlagDivisions()));
	connect(p_askDivisionFlags, SIGNAL(clicked()), this, SLOT(askDivisionFlags()));

	KAction *a = KStdAction::open(this, SLOT(openMap()), actionCollection(), "openMap");
	a -> setText(i18n("&Open Map..."));
	KStdAction::quit(this, SLOT(close()), actionCollection(), "quit");

	p_zoom = new KToggleAction(KIcon("viewmagfit"), i18n("&Zoom"), actionCollection(), "zoom_select");
	p_zoom -> setEnabled(false);
	
	p_zoomOriginal = new KAction(KIcon("viewmag1"), i18n("&Original Size"), actionCollection(), "zoom_original");
	p_zoomOriginal -> setEnabled(false);

	p_move = new KToggleAction(KIcon("move"), i18n("&Move"), actionCollection(), "move");
	p_move -> setEnabled(false);

	a = new KAction(i18n("Disclaimer"), actionCollection(), "disclaimer");
	connect(a, SIGNAL(triggered()), this, SLOT(disclaimer()));

	setupGUI(Keys | ToolBar | Save | Create);

	show();
}

kgeography::~kgeography()
{
	delete p_askWidget;
	delete p_map;
}

void kgeography::showEvent(QShowEvent *)
{
	if (p_firstShow)
	{
		QString file = kgeographySettings::self() -> lastMap();
		
		if (QFile::exists(file))
		{
			mapReader reader;
			if (reader.parseMap(file))
			{
				setMap(reader.getMap());
			}
			else
			{
				KMessageBox::error(this, i18n("Could not open last used map. Error parsing %1: %2", file, reader.getError()));
				delete reader.getMap();
				openMap();
			}
		}
		else openMap();

		if (!p_map)
		{
			p_currentMap -> setText(i18n("There is no current map"));
			p_consult -> setEnabled(false);
			p_askMap -> setEnabled(false);
			p_askFlagDivisions -> setEnabled(false);
			p_askDivisionFlags -> setEnabled(false);
			p_askCapitalDivisions -> setEnabled(false);
			p_askDivisionCapitals -> setEnabled(false);
		}
		// if anyone can explain why with the slot works and now without
		// i'll be glad to learn
		QTimer::singleShot(0, this, SLOT(resizeMainWindow()));
// 		resizeMainWindow();
		
		p_firstShow = false;
	}
}

void kgeography::openMap()
{
	mapChooser mp(this);
	if (mp.exec() == mapChooser::Accepted)
	{
		showResultsDialog();
		setMap(mp.getMap());
		// if anyone can explain why with the slot works and now without
		// i'll be glad to learn
		QTimer::singleShot(0, this, SLOT(resizeMainWindow()));
// 		resizeMainWindow();
	}
}

void kgeography::consult()
{
	// No need to create another map viewer if we are already in it
	mapAsker *ma = dynamic_cast<mapAsker*>(p_askWidget);
	if (ma && !ma -> isAsker()) return;
	
	showResultsDialog();
	removeOldAskWidget();
	p_askWidget = new mapAsker(p_bigWidget, p_map, p_underLeftWidget, false);
	p_zoom -> setEnabled(true);
	p_zoomOriginal -> setEnabled(true);
	putAskWidget();
}

void kgeography::askCapitalDivisions()
{
	int i;
	bool ok;
	showResultsDialog();
	i = KInputDialog::getInteger(i18n("Number of Questions"), i18n("How many questions do you want? (1 to %1)", p_map -> count(false)), 1, 1, p_map -> count(false), 1, &ok);
	if (ok)
	{
		removeOldAskWidget();
		p_askWidget = new capitalDivisionAsker(p_bigWidget, p_map, p_underLeftWidget, i);
		putAskWidget();
		p_mustShowResultsDialog = true;
	}
	else consult();
}

void kgeography::askDivisionCapitals()
{
	int i;
	bool ok;
	showResultsDialog();
	i = KInputDialog::getInteger(i18n("Number of Questions"), i18n("How many questions do you want? (1 to %1)", p_map -> count(false)), 1, 1, p_map -> count(false), 1, &ok);
	if (ok)
	{
		removeOldAskWidget();
		p_askWidget = new divisionCapitalAsker(p_bigWidget, p_map, p_underLeftWidget, i);
		putAskWidget();
		p_mustShowResultsDialog = true;
	}
	else consult();
}

void kgeography::askMap()
{
	int i;
	bool ok;
	showResultsDialog();
	i = KInputDialog::getInteger(i18n("Number of Questions"), i18n("How many questions do you want? (1 to %1)", p_map -> count(true)), 1, 1, p_map -> count(true), 1, &ok);
	if (ok)
	{
		removeOldAskWidget();
		p_askWidget = new mapAsker(p_bigWidget, p_map, p_underLeftWidget, true, i);
		p_zoom -> setEnabled(true);
		p_zoomOriginal -> setEnabled(true);
		putAskWidget();
		p_mustShowResultsDialog = true;
	}
	else consult();
}

void kgeography::askFlagDivisions()
{
	int i;
	bool ok;
	showResultsDialog();
	i = KInputDialog::getInteger(i18n("Number of Questions"), i18n("How many questions do you want? (1 to %1)", p_map -> count(false)), 1, 1, p_map -> count(false), 1, &ok);
	if (ok)
	{
		removeOldAskWidget();
		p_askWidget = new flagDivisionAsker(p_bigWidget, p_map, p_underLeftWidget, i);
		putAskWidget();
		p_mustShowResultsDialog = true;
	}
	else consult();
}

void kgeography::askDivisionFlags()
{
	int i;
	bool ok;
	showResultsDialog();
	i = KInputDialog::getInteger(i18n("Number of Questions"), i18n("How many questions do you want? (1 to %1)", p_map -> count(false)), 1, 1, p_map -> count(false), 1, &ok);
	if (ok)
	{
		removeOldAskWidget();
		p_askWidget = new divisionFlagAsker(p_bigWidget, p_map, p_underLeftWidget, i);
		putAskWidget();
		p_mustShowResultsDialog = true;
	}
	else consult();
}

void kgeography::removeOldAskWidget()
{
	p_askWidget->deleteLater();
	p_askWidget = 0;
	p_zoom -> setEnabled(false);
	p_zoomOriginal -> setEnabled(false);
	p_move -> setEnabled(false);
	p_zoom -> setChecked(false);
	p_move -> setChecked(false);
}

QSize kgeography::getPreferredSize()
{
	int ySize = 0;
	
	ySize = menuBar() -> size().height() + toolBar() -> size().height() + ((mapAsker*) p_askWidget)->mapSize().height();
	return QSize(p_underLeftWidget -> size().width() + ((mapAsker*) p_askWidget)->mapSize().width() + 3, ySize + 3);
}

void kgeography::putAskWidget()
{
	static_cast<QBoxLayout*>(p_bigWidget -> layout()) -> addWidget(p_askWidget, 1);
	p_askWidget -> show();
	connect(p_askWidget, SIGNAL(setZoomActionChecked(bool)), p_zoom, SLOT(setChecked(bool)));
	connect(p_zoom, SIGNAL(toggled(bool)), p_askWidget, SLOT(setZoom(bool)));
	connect(p_zoomOriginal, SIGNAL(triggered()), p_askWidget, SLOT(setOriginalZoom()));
	connect(p_askWidget, SIGNAL(setMoveActionEnabled(bool)), p_move, SLOT(setEnabled(bool)));
	connect(p_askWidget, SIGNAL(setMoveActionChecked(bool)), p_move, SLOT(setChecked(bool)));
	connect(p_move, SIGNAL(toggled(bool)), p_askWidget, SLOT(setMovement(bool)));
	connect(p_askWidget, SIGNAL(questionsEnded()), this, SLOT(questionsEnded()));
}

void kgeography::setMap(KGmap *m)
{
	removeOldAskWidget();
	kgeographySettings *set = kgeographySettings::self();
	set -> setLastMap(m -> getFile());
	set -> writeConfig();
	delete p_map;
	p_map = m;
	
	QString sw = i18nc("There are two ways of dealing with the translation of \"Current map: %1\". The first option simply replaces %1 with the translated name of the relevant region. If the grammar of your language allows this, choose this option by setting the translation of this message to 1, and leave untranslated the translations of \"Current map: %1\" that have the placename embedded (or translate them as - if you wish to show the file as fully translated. The second option is to translate all messages in full - this is likely to be required in the case of highly-inflected languages like Russian. To choose this option, set the translation of this message to 0, and translate all the messages.", "0");
	if (sw == "1")
	{
		QString mapName = i18nc(p_map -> getFileName().toUtf8(), p_map -> getName().toUtf8());
		p_currentMap -> setText(i18n("<qt>Current map:<br><b>%1<b></qt>", mapName));
	}
	else
	{
		QString s = QString("<qt>Current map:<br><b>%1</b></qt>").arg(p_map -> getName());
		p_currentMap -> setText(i18nc(p_map -> getFileName().toUtf8(), s.toUtf8()));
	}
	p_consult -> setEnabled(true);
	p_askMap -> setEnabled(true);
	p_askFlagDivisions -> setEnabled(m -> hasAllFlags());
	p_askDivisionFlags -> setEnabled(m -> hasAllFlags());
	p_askCapitalDivisions -> setEnabled(true);
	p_askDivisionCapitals -> setEnabled(true);
	consult();
}

void kgeography::disclaimer()
{
	KMessageBox::information(this, i18n("Maps, flags, translations, etc. are as accurate as their respective authors could achieve, but KGeography should not be taken as an authoritative source."), i18n("Disclaimer"));
}

void kgeography::resizeMainWindow()
{
	if (p_askWidget) resize(getPreferredSize());
}

void kgeography::questionsEnded()
{
	showResultsDialog();
	consult();
}

void kgeography::showResultsDialog()
{
	if (p_mustShowResultsDialog)
	{
		p_mustShowResultsDialog = false;
		int ca = p_askWidget -> correctAnswers();
		QString q = p_askWidget -> getQuestionHook();
		QVector<userAnswer> ua = p_askWidget -> userAnswers();
	
		answersDialog *ad = new answersDialog(this, ua, q, ca);
		ad -> exec();
		delete ad;
	}
}

#include "kgeography.moc"
