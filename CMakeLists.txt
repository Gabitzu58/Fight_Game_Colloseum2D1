cmake_minimum_required(VERSION 3.16)

project(joc VERSION 1.0 LANGUAGES CXX)

# ======================
# Setări generale
# ======================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ======================
# Qt
# ======================
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# ======================
# Fișiere sursă
# ======================
set(SOURCES
        main.cpp
        menugame.cpp
        gladiator.cpp
        LobbyOverlay.cpp
        SettingsOverlay.cpp
        arena.cpp
        GameWindow.cpp
)

set(HEADERS
        menugame.h
        gladiator.h
        LobbyOverlay.h
        SettingsOverlay.h
        arena.h
        GameWindow.h
)

set(RESOURCES
        src/resources.qrc
        SoundManager.cpp
        SoundManager.h
        Projectile.cpp
        Projectile.h
        Character.cpp
        Character.h
        Warrior.cpp
        Warrior.h
        GladiatorFactory.cpp
        GladiatorFactory.h
        Swordsman.cpp
        Swordsman.h
        Brute.cpp
        Brute.h
        Ciclop.cpp
        Ciclop.h
        Mage.cpp
        Mage.h
        Wizard.cpp
        Wizard.h
        Spear.cpp
        Spear.h
        stat.h
)

# ======================
# Executabil
# ======================
qt_add_executable(joc WIN32
        ${SOURCES}
        ${HEADERS}
        ${RESOURCES}
)

# ======================
# Link Qt
# ======================
target_link_libraries(joc PRIVATE Qt6::Widgets)

# ======================
# Include paths
# ======================
target_include_directories(joc PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
find_package(Qt6 REQUIRED COMPONENTS
        Core
        Widgets
        Gui
        Multimedia
)

target_link_libraries(joc PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui Qt6::Multimedia)
