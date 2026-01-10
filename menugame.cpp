#include "menugame.h"
#include <QApplication>
#include <QPixmap>

MenuGame::MenuGame(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(1000, 800);
    setWindowTitle("Colosseum 2D");

    // Cream overlay-urile
    lobby = new LobbyOverlay(this);
    lobby->hide();

    settings = new SettingsOverlay(this);
    settings->hide();

    loadBackground();
    createButtons();
}

void MenuGame::loadBackground()
{
    backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(QPixmap(":/images/menuColloseum.png"));
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setGeometry(0, 0, width(), height());
    backgroundLabel->lower();
}

void MenuGame::createButtons()
{
    QString style =
        "QPushButton {"
        "background-color:#C97A2B;"
        "color:white;"
        "font-size:24px;"
        "font-weight:bold;"
        "border-radius:8px;"
        "}"
        "QPushButton:hover { background-color:#E18B35; }";

    btnStart = new QPushButton("START", this);
    btnSettings = new QPushButton("SETTINGS", this);
    btnExit = new QPushButton("EXIT", this);

    btnStart->setGeometry(400, 320, 200, 60);
    btnSettings->setGeometry(400, 400, 200, 60);
    btnExit->setGeometry(400, 480, 200, 60);

    btnStart->setStyleSheet(style);
    btnSettings->setStyleSheet(style);
    btnExit->setStyleSheet(style);

    // Emit semnal startClicked când dai click
    connect(btnStart, &QPushButton::clicked, this, [this]() {
        emit startClicked();
    });

    // Emit semnal settingsClicked când dai click
    connect(btnSettings, &QPushButton::clicked, this, [this]() {
        emit settingsClicked();
    });

    // Emit semnal exitClicked când dai click
    connect(btnExit, &QPushButton::clicked, this, [this]() {
        emit exitClicked();
    });
}


