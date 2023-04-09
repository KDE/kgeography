

ecm_setup_version(${RELEASE_SERVICE_VERSION} VARIABLE_PREFIX KGEOGRAPHY VERSION_HEADER kgeography_version.h)

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

target_link_libraries(kgeography KF${QT_MAJOR_VERSION}::XmlGui
                                 KF${QT_MAJOR_VERSION}::WidgetsAddons
                                 KF${QT_MAJOR_VERSION}::CoreAddons
                                 KF${QT_MAJOR_VERSION}::ConfigWidgets
                                 KF${QT_MAJOR_VERSION}::Crash
                                 KF${QT_MAJOR_VERSION}::I18n
                                 KF${QT_MAJOR_VERSION}::ItemViews
                                 KF${QT_MAJOR_VERSION}::IconThemes
)
install(TARGETS kgeography EXPORT KGeography ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS org.kde.kgeography.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )
install( FILES org.kde.kgeography.appdata.xml DESTINATION  ${KDE_INSTALL_METAINFODIR})
install( FILES kgeography.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR} )
install( FILES kgeographyui.rc  DESTINATION  ${KDE_INSTALL_KXMLGUIDIR}/kgeography )



