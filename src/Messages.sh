#! /bin/sh
$EXTRACTRC *.ui *.rc *.kcfg >> rc.cpp

cd ../data
for i in *kgm; do $EXTRACTRC --context="$i" --tag=name --tag=capital --tag=divisionsName $i; done >> ../src/rc.cpp
for i in *kgm; do $EXTRACTRC --context="@title:group" --tag=divisionToCapitalPattern --tag=capitalToDivisionPattern $i; done >> ../src/rc.cpp
for i in *kgm; do $EXTRACTRC --context="@title" --tag=capitalToDivisionTitle --tag=divisionToCapitalTitle $i; done >> ../src/rc.cpp
cd ../src

$XGETTEXT *.cpp -o $podir/kgeography.pot
rm -f rc.cpp
