cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "20")
set(RELEASE_SERVICE_VERSION_MINOR "04")
set(RELEASE_SERVICE_VERSION_MICRO "1")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kgeography VERSION ${RELEASE_SERVICE_VERSION})

# minimal Qt requirement
set (QT_MIN_VERSION "5.2.0")

# ECM
find_package (ECM 1.7.0 REQUIRED NO_MODULE)

# where to look first for cmake modules
set (CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

find_package (Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED Core Widgets)

find_package (KF5 5.15 REQUIRED COMPONENTS
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

# global include directories
include_directories (${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory( doc )
add_subdirectory( src ) 
add_subdirectory( data ) 
add_subdirectory( icons ) 

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
