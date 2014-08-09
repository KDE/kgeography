



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


kde4_add_ui_files(kgeography_SRCS kgeographyoptions.ui )

kde4_add_kcfg_files(kgeography_SRCS settings.kcfgc )

#kde4_add_app_icon(kgeography_SRCS "${KDE4_ICON_DIR}/oxygen/*/apps/kgeography.png")
#kde4_add_app_icon(kgeography_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/ox*-apps-kgeography.png")
kde4_add_app_icon(kgeography_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-apps-kgeography.png")

add_executable(kgeography ${kgeography_SRCS})

target_link_libraries(kgeography   ${KDE4_KDEUI_LIBS} ${QT_QTXML_LIBRARY})
target_link_libraries(kgeography KF5::XmlGui
                                 KF5::WidgetsAddons
                                 KF5::CoreAddons
                                 KF5::ConfigWidgets
                                 KF5::ItemViews
                                 KF5::Service
)
install(TARGETS kgeography EXPORT KGeography ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS kgeography.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES kgeography.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kgeographyui.rc  DESTINATION  ${KXMLGUI_INSTALL_DIR}/kgeography )



