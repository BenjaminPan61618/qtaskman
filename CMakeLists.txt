cmake_minimum_required(VERSION 3.21)
project(qtaskman VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 Widgets DBus LinguistTools)

qt_standard_project_setup()

qt_add_executable(qtaskman
    src/main.cpp
    src/systeminfo.h
    src/systeminfo.cpp
    src/processsampler.h
    src/processsampler.cpp
    src/processmodel.h
    src/processmodel.cpp
    src/processcontroller.h
    src/processcontroller.cpp
    src/settings.h
    src/settings.cpp
    src/thememanager.h
    src/thememanager.cpp
    src/windowprobe.h
    src/windowprobe.cpp
    src/appsmodel.h
    src/appsmodel.cpp
    src/systemmonitor.h
    src/systemmonitor.cpp
    src/translator.h
    src/translator.cpp
)

qt_add_translations(qtaskman
    TS_FILES
        i18n/qtaskman_zh_CN.ts
        i18n/qtaskman_en_US.ts
    LUPDATE_OPTIONS -no-obsolete
)

qt_add_resources(qtaskman "translations"
    PREFIX "/i18n"
    BASE "${CMAKE_CURRENT_BINARY_DIR}"
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/qtaskman_zh_CN.qm"
        "${CMAKE_CURRENT_BINARY_DIR}/qtaskman_en_US.qm"
)

qt_add_resources(qtaskman "assets"
    PREFIX "/"
    FILES
        assets/qtaskman.svg
)

qt_add_resources(qtaskman "qml"
    PREFIX "/"
    FILES
        qml/Main.qml
        qml/Sidebar.qml
        qml/components/Panel.qml
        qml/components/InfoCard.qml
        qml/components/Sparkline.qml
        qml/components/LineChart.qml
        qml/components/Toast.qml
        qml/components/ProcessContextMenu.qml
        qml/components/UsageBar.qml
        qml/components/FloatMonitorWindow.qml
        qml/components/TabBar.qml
        qml/pages/OverviewPage.qml
        qml/pages/AppsPage.qml
        qml/pages/ProcessesPage.qml
        qml/pages/MonitorPage.qml
        qml/pages/SettingsPage.qml
        qml/pages/AboutPage.qml
)

target_link_libraries(qtaskman PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Widgets
    Qt6::DBus
)

install(TARGETS qtaskman RUNTIME DESTINATION bin)
