#! /bin/sh
$EXTRACTRC *.ui *.rc *.kcfg >> rc.cpp
$XGETTEXT *.cpp -o $podir/kgeography.pot
