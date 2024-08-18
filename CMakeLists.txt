cmake_minimum_required (VERSION 3.16 FATAL_ERROR)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "24")
set(RELEASE_SERVICE_VERSION_MINOR "11")
set(RELEASE_SERVICE_VERSION_MICRO "70")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kgeography VERSION ${RELEASE_SERVICE_VERSION})

# minimal Qt requirement
set(QT_MIN_VERSION "6.5.0")
set(KF_REQUIRED_VERSION "5.240.0")

# ECM
find_package (ECM ${KF_REQUIRED_VERSION} REQUIRED NO_MODULE)

# where to look first for cmake modules
set (CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(FeatureSummary)
include(ECMAddAppIcon)
include(ECMInstallIcons)
include(ECMSetupVersion)
include(ECMDeprecationSettings)

find_package (Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED Core Widgets)

find_package (KF6 ${KF_REQUIRED_VERSION} REQUIRED COMPONENTS
    XmlGui WidgetsAddons CoreAddons ConfigWidgets I18n ItemViews IconThemes Crash DocTools
)

# global include directories
include_directories (${CMAKE_CURRENT_BINARY_DIR})
ecm_set_disabled_deprecation_versions(QT 5.15  KF 6.5.0)
add_subdirectory( doc )
add_subdirectory( src ) 
add_subdirectory( data ) 
add_subdirectory( icons ) 

ki18n_install(po)
kdoctools_install(po)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
