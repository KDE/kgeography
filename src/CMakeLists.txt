



########### next target ###############

remove_definitions(-DQT3_SUPPORT_WARNINGS)
remove_definitions(-DQT3_SUPPORT)

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
   placeasker.cpp 
   placemapwidget.cpp 
   capitaldivisionasker.cpp 
   divisioncapitalasker.cpp 
   boxasker.cpp 
   divisionflagasker.cpp 
   answer.cpp 
   answersdialog.cpp )

kde4_automoc(kgeography ${kgeography_SRCS})

kde4_add_kcfg_files(kgeography_SRCS settings.kcfgc )

kde4_add_executable(kgeography ${kgeography_SRCS})

target_link_libraries(kgeography  ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} )

install(TARGETS kgeography  DESTINATION ${BIN_INSTALL_DIR} )


########### next target ###############

set(script_SRCS script.cpp )

kde4_automoc(script ${script_SRCS})

kde4_add_executable(script NOGUI ${script_SRCS})

target_link_libraries(script ${KDE4_KDECORE_LIBS} ${QT_QTXML_LIBRARY} )


########### install files ###############

install( FILES kgeography.desktop  DESTINATION  ${XDG_APPS_DIR} )
install( FILES kgeography.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kgeographyui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kgeography )



