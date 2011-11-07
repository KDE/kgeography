



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
   placeasker.cpp 
   placemapwidget.cpp 
   capitaldivisionasker.cpp 
   divisioncapitalasker.cpp 
   boxasker.cpp 
   divisionflagasker.cpp 
   integerinputdialog.cpp
   answer.cpp 
   answersdialog.cpp )


kde4_add_ui_files(kgeography_SRCS KGeographyOptions.ui )

kde4_add_kcfg_files(kgeography_SRCS settings.kcfgc )

#kde4_add_app_icon(kgeography_SRCS "${KDE4_ICON_DIR}/oxygen/*/apps/kgeography.png")
#kde4_add_app_icon(kgeography_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/ox*-apps-kgeography.png")
kde4_add_app_icon(kgeography_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-apps-kgeography.png")

kde4_add_executable(kgeography ${kgeography_SRCS})

target_link_libraries(kgeography   ${KDE4_KDEUI_LIBS} ${QT_QTXML_LIBRARY})

install(TARGETS kgeography  ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### next target ###############

set(script_SRCS script.cpp )


kde4_add_executable(script NOGUI ${script_SRCS})

target_link_libraries(script ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY} )


########### install files ###############

install( PROGRAMS kgeography.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES kgeography.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kgeographyui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kgeography )



