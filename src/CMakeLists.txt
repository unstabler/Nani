set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(KF5_MIN_VERSION "5.50")

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)

find_package(Qt5 COMPONENTS Core Gui Network REQUIRED)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS CoreAddons I18n Runner)

set(NANI_SRCS
    nani_plugin.cpp
    NaniRunner.cpp
    NaverDict.cpp
)

kcoreaddons_add_plugin(nani SOURCES ${NANI_SRCS} INSTALL_NAMESPACE "kf5/krunner")
target_link_libraries(nani PUBLIC Qt5::Core Qt5::Gui Qt5::Network KF5::CoreAddons KF5::I18n KF5::Runner)
