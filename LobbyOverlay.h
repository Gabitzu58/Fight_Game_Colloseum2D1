#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVector>
#include <QPixmap>
#include <memory>

#include "gladiator.h"

class LobbyOverlay : public QWidget
{
    Q_OBJECT
public:
    explicit LobbyOverlay(QWidget* parent = nullptr);
    void ResetLobby();
    signals:
     void startGame(Gladiator* p1, Gladiator* p2);

private:
    // UI build
    void buildUI();
    void onStartClicked();
    QLabel* createStatusLabel(const QString& text, int x, const QString& color);
    QLabel* createPreview(int x);
    QLabel* createNameLabel(int x);
    void createArrow(int x, int y, int player, int dir);

    // Logic
    void updatePreview(int player);
    void selectGladiator(int player);


    QLabel* title = nullptr;
    QLabel* s1 = nullptr;
    QLabel* s2 = nullptr;

    QLabel* p1Preview = nullptr;
    QLabel* p2Preview = nullptr;

    QLabel* p1Name = nullptr;
    QLabel* p2Name = nullptr;

    QPushButton* p1SelectBtn = nullptr;
    QPushButton* p2SelectBtn = nullptr;
    QPushButton* startBtn = nullptr;

    // Data
    std::vector<QString> names;
    std::vector<QPixmap> pixmaps;

    std::unique_ptr<Gladiator> p1;
    std::unique_ptr<Gladiator> p2;
    int p1Index = 0;
    int p2Index = 0;

    bool p1Selected = false;
    bool p2Selected = false;

};

