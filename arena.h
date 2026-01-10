#pragma once

#include <QWidget>
#include <QTimer>
#include <QPixmap>
#include <QPushButton>
#include <memory>
#include "Projectile.h"
#include <vector>

#include "gladiator.h"

class Arena : public QWidget
{
    Q_OBJECT

public:
    explicit Arena(QWidget* parent = nullptr);
    ~Arena() override;
    void setPlayers(Gladiator* p1, Gladiator* p2);
    signals:
        void backToMenu();

protected:
    std::string name;
    int hp;

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void updateGame();
private slots:
    void onPlayerShoot(const QPoint& startPos);


private:
    /* ================= INIT ================= */
    void initPlayers();
    void createVictoryUI();
    /* ================= RESET ================= */
    void resetArena();
    void resetRound();

    /* ================= HUD ================= */
    void drawHpBar(QPainter& p, Gladiator* g, QRect area);

    /* ================= GAME ================= */
    QTimer gameTimer;
    QPixmap background;

    bool playersReady = false;

    /* ================= HIT CONTROL ================= */
    bool player1HitRegistered = false;
    bool player2HitRegistered = false;

    /* ================= GAME STATE ================= */
    bool gameOver = false;
    QString winnerName;

    /* ================= UI ================= */
    QPushButton* backToMenuBtn = nullptr;
    std::vector<std::unique_ptr<Projectile>> projectiles;
    QPixmap arrowSprite;
    QPixmap magicSprite;
    int roundNumber = 1;

    int player1Rounds = 0;
    int player2Rounds = 0;

    int consecutiveWinsP1 = 0;
    int consecutiveWinsP2 = 0;

    bool roundOver = false;
    bool freezeInput = false;
    bool showRoundText = false;
    QString roundText;

    Gladiator* player1 = nullptr;
    Gladiator* player2 = nullptr;
};

