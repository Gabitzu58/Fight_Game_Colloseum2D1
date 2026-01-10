#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "LobbyOverlay.h"
#include "SettingsOverlay.h"

class MenuGame : public QWidget
{
    Q_OBJECT

public:
    explicit MenuGame(QWidget* parent = nullptr);

    signals:
        void startClicked();
    void settingsClicked();
    void exitClicked();

private:
    void loadBackground();
    void createButtons();

    QLabel* backgroundLabel{};
    LobbyOverlay* lobby{};
    SettingsOverlay* settings{};

    QPushButton* btnStart{};
    QPushButton* btnSettings{};
    QPushButton* btnExit{};
};
