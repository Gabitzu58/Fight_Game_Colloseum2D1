#pragma once

#include <QWidget>
#include <QStackedWidget>

#include "MenuGame.h"
#include "LobbyOverlay.h"
#include "SettingsOverlay.h"
#include "Arena.h"

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget* parent = nullptr);

private:
    void setupConnections();

private:
    QStackedWidget* stack = nullptr;

    MenuGame* menu = nullptr;
    LobbyOverlay* lobby = nullptr;
    SettingsOverlay* settings = nullptr;
    Arena* arena = nullptr;
};
