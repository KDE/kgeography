kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(kgeography_SRCS 
   main.cpp 
   mapwidget.cpp 
   division.cpp 
   map.cpp 
   mapparser.cpp 
   kgeography.cpp 
   mapchooser.cpp 
   mypopup.cpp 
   popupmanager.cpp 
   flagdivisionasker.cpp 
   askwidget.cpp 
   mapasker.cpp 
   capitaldivisionasker.cpp 
   divisioncapitalasker.cpp 
   boxasker.cpp 
   divisionflagasker.cpp 
   answer.cpp 
   answersdialog.cpp )

kde4_automoc(${kgeography_SRCS})

kde4_add_kcfg_files(kgeography_SRCS settings.kcfgc )

kde4_add_executable(kgeography ${kgeography_SRCS})

target_link_libraries(kgeography  ${QT_AND_KDECORE_LIBS} kdeui )

install_targets(/bin kgeography )


########### next target ###############

set(script_SRCS script.cpp )

kde4_automoc(${script_SRCS})

kde4_add_executable(script ${script_SRCS})

target_link_libraries(script  ${QT_AND_KDECORE_LIBS} )


########### install files ###############

install_files( ${XDG_APPS_DIR} FILES kgeography.desktop )
install_files( ${KCFG_INSTALL_DIR} FILES kgeography.kcfg )
install_files( ${DATA_INSTALL_DIR}/kgeography FILES kgeographyui.rc )

kde4_footer()



#original Makefile.am contents follow:

### Makefile.am for kgeography
#
## this is the program that gets installed.  it's name is used for all
## of the other Makefile.am variables
#bin_PROGRAMS = kgeography
#
## set the include path for X, qt and KDE
#INCLUDES         = $(all_includes)
#
## the library search path.
#kgeography_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#
## the libraries to link against.
#kgeography_LDADD = $(LIB_KDEUI)
#
## which sources should be compiled for kgeography
#kgeography_SOURCES = main.cpp mapwidget.cpp division.cpp map.cpp mapparser.cpp kgeography.cpp mapchooser.cpp mypopup.cpp popupmanager.cpp settings.kcfgc flagdivisionasker.cpp askwidget.cpp mapasker.cpp capitaldivisionasker.cpp divisioncapitalasker.cpp boxasker.cpp divisionflagasker.cpp answer.cpp answersdialog.cpp
#
## let automoc handle all of the meta source files (moc)
#METASOURCES = AUTO
#
#messages: rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/kgeography.pot
#
## this is where the kdelnk file will go
#xdg_apps_DATA = kgeography.desktop
#
#kde_kcfg_DATA = kgeography.kcfg
#
## this is where the XML-GUI resource file goes
#rcdir = $(kde_datadir)/kgeography
#rc_DATA = kgeographyui.rc
#
#debug: kgeography
#	ddd ./kgeography
#
#valgrind: kgeography
#	valgrind --tool=memcheck --leak-check=yes ./kgeography
#
#noinst_PROGRAMS = script
#script_SOURCES = script.cpp
#script_LDADD = $(LIB_QT) $(LIB_QTXML)
#script_LDFLAGS = $(all_libraries)