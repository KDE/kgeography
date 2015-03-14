



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


ki18n_wrap_ui(kgeography_SRCS kgeographyoptions.ui )

kconfig_add_kcfg_files(kgeography_SRCS settings.kcfgc )

add_executable(kgeography ${kgeography_SRCS})

target_link_libraries(kgeography KF5::XmlGui
                                 KF5::WidgetsAddons
                                 KF5::CoreAddons
                                 KF5::ConfigWidgets
                                 KF5::I18n
                                 KF5::ItemViews
                                 KF5::IconThemes
                                 KF5::Service
)
install(TARGETS kgeography EXPORT KGeography ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS org.kde.kgeography.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )
install( FILES kgeography.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR} )
install( FILES kgeographyui.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/kgeography )



