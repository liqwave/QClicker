cmake_minimum_required(VERSION 3.19)
project(QClicker LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()
set(CMAKE_AUTORCC ON)   # <-- добавить

qt_add_executable(QClicker
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    Clicker.h Clicker.cpp
    InputBinding.h
    KeyBindDialog.h KeyBindDialog.cpp KeyBindDialog.ui
    GlobalHook.h GlobalHook.cpp
    StyleManager.h StyleManager.cpp
    TrayManager.h TrayManager.cpp
    resources.qrc
    sources/iconDefault.rc
    ConfigManager.h ConfigManager.cpp
    ClickerEnums.h
)

target_link_libraries(QClicker
    PRIVATE
        Qt::Core
        Qt::Widgets
        winmm  # <-- Добавь эту строчку
)

install(TARGETS QClicker
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET QClicker
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})