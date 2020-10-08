cmake_minimum_required (VERSION 3.13 FATAL_ERROR)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "20")
set(RELEASE_SERVICE_VERSION_MINOR "11")
set(RELEASE_SERVICE_VERSION_MICRO "70")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kgeography VERSION ${RELEASE_SERVICE_VERSION})

# minimal Qt requirement
set(QT_MIN_VERSION "5.12.0") # Remember to update the QT_DEPRECATED_WARNINGS_SINCE below
set(KF5_REQUIRED_VERSION "5.62.0") # Remember to update the KF_DEPRECATED_WARNINGS_SINCE below

# ECM
find_package (ECM ${KF5_REQUIRED_VERSION} REQUIRED NO_MODULE)

# where to look first for cmake modules
set (CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

find_package (Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED Core Widgets)

find_package (KF5 ${KF5_REQUIRED_VERSION} REQUIRED COMPONENTS
    XmlGui WidgetsAddons CoreAddons ConfigWidgets I18n ItemViews IconThemes Crash DocTools
)

include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(FeatureSummary)
include(ECMAddAppIcon)
include(ECMInstallIcons)
include(ECMSetupVersion)

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)
add_definitions(-DQT_DEPRECATED_WARNINGS_SINCE=0x050C00)
add_definitions(-DKF_DEPRECATED_WARNINGS_SINCE=0x053E00)

# global include directories
include_directories (${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory( doc )
add_subdirectory( src ) 
add_subdirectory( data ) 
add_subdirectory( icons ) 

ki18n_install(po)
if (KF5DocTools_FOUND)
    kdoctools_install(po)
endif()

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
