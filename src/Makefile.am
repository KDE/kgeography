## Makefile.am for kgeography

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = kgeography

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# the library search path.
kgeography_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# the libraries to link against.
kgeography_LDADD = $(LIB_KDEUI)

# which sources should be compiled for kgeography
kgeography_SOURCES = main.cpp mapwidget.cpp division.cpp map.cpp mapparser.cpp kgeography.cpp mapchooser.cpp mypopup.cpp popupmanager.cpp settings.kcfgc flagdivisionasker.cpp askwidget.cpp mapasker.cpp capitaldivisionasker.cpp divisioncapitalasker.cpp boxasker.cpp divisionflagasker.cpp answer.cpp answersdialog.cpp

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kgeography.pot

# this is where the kdelnk file will go
xdg_apps_DATA = kgeography.desktop

kde_kcfg_DATA = kgeography.kcfg

# this is where the XML-GUI resource file goes
rcdir = $(kde_datadir)/kgeography
rc_DATA = kgeographyui.rc

debug: kgeography
	ddd ./kgeography

valgrind: kgeography
	valgrind --tool=memcheck --leak-check=yes ./kgeography

noinst_PROGRAMS = script
script_SOURCES = script.cpp
script_LDADD = $(LIB_QT)
script_LDFLAGS = $(all_libraries)
