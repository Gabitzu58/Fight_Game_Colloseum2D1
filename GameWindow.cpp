#include "GameWindow.h"
#include <QApplication>

GameWindow::GameWindow(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(1000, 800);

    // STACK = containerul REAL
    stack = new QStackedWidget(this);
    stack->setGeometry(rect());

    // ================= SCREENS =================

    menu = new MenuGame(stack);
    lobby = new LobbyOverlay(stack);
    settings = new SettingsOverlay(stack);
    arena = new Arena(stack);

    // ================= ADD TO STACK =================

    stack->addWidget(menu);
    stack->addWidget(lobby);
    stack->addWidget(settings);
    stack->addWidget(arena);

    // Start în meniu
    stack->setCurrentWidget(menu);

    setupConnections();
}

void GameWindow::setupConnections()
{
    // ===== MENU =====
    connect(menu, &MenuGame::startClicked, this, [this]() {
        stack->setCurrentWidget(lobby);
    });

    connect(menu, &MenuGame::settingsClicked, this, [this]() {
        stack->setCurrentWidget(settings);
    });

    connect(menu, &MenuGame::exitClicked,
            qApp, &QApplication::quit);

    // ===== SETTINGS =====
    connect(settings, &SettingsOverlay::backPressed, this, [this]() {
        stack->setCurrentWidget(menu);
    });

    // ===== LOBBY -> ARENA =====
   connect(
       lobby,
       &LobbyOverlay::startGame,
       this,
       [this](Gladiator* p1, Gladiator* p2)
       {

           arena->setPlayers(p1, p2);
           stack->setCurrentWidget(arena);
       }
   );


    // ===== ARENA -> MENU =====
    connect(arena, &Arena::backToMenu, this, [this]()
    {
        arena->hide();
        lobby->ResetLobby();
        stack->setCurrentWidget(menu);
    });
}
