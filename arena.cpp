#include "Arena.h"

#include <QPainter>
#include <QKeyEvent>
#include <QFont>
#include <QDebug>
#include <QLabel>
#include "Warrior.h"
/* ================= CONSTRUCTOR ================= */

Arena::Arena(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(1000, 800);
    setFocusPolicy(Qt::StrongFocus);

    background.load(":/images/Arena.png");
    arrowSprite.load(":/images/proiectil0.png");
    magicSprite.load(":/images/proiectil1.png");

    connect(&gameTimer, &QTimer::timeout,
            this, &Arena::updateGame);

    gameTimer.start(16); // ~60 FPS

    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
}

/* ================= DESTRUCTOR ================= */

Arena::~Arena() = default;

/* ================= PLAYERS ================= */

void Arena::setPlayers(Gladiator* p1, Gladiator* p2)
{
    player1 = p1;
    player2 = p2;

    if (!player1 || !player2)
        return;

    // Qt ownership
    player1->setParent(this);
    player2->setParent(this);

    initPlayers();

    /* ===== PROJECTILES P1 ===== */
    connect(player1, &Gladiator::shootProjectile,
            this, [this](const QPoint& startPos, bool facingRight)
    {
        QPointF dir = facingRight ? QPointF(1,0) : QPointF(-1,0);

        const QPixmap& sprite =
            (player1->getType() == Gladiator::GladiatorType::Wizard)
                ? magicSprite
                : arrowSprite;

        projectiles.push_back(std::make_unique<Projectile>(
            QPointF(startPos),
            dir,
            sprite,
            player1->getDamage(),
            player1
        ));
    });

    /* ===== PROJECTILES P2 ===== */
    connect(player2, &Gladiator::shootProjectile,
            this, [this](const QPoint& startPos, bool facingRight)
    {
        QPointF dir = facingRight ? QPointF(1,0) : QPointF(-1,0);

        const QPixmap& sprite =
            (player2->getType() == Gladiator::GladiatorType::Wizard)
                ? magicSprite
                : arrowSprite;

        projectiles.push_back(std::make_unique<Projectile>(
            QPointF(startPos),
            dir,
            sprite,
            player2->getDamage(),
            player2
        ));
    });

    /* ===== INIT ROUNDS ===== */
    roundNumber = 1;
    roundOver = false;
    gameOver = false;
    freezeInput = true;

    roundText = "ROUND 1";
    showRoundText = true;

    QTimer::singleShot(1200, this, [this]()
    {
        roundText = "FIGHT!";
        QTimer::singleShot(800, this, [this]()
        {
            showRoundText = false;
            freezeInput = false;
        });
    });

    playersReady = true;
    update();
}

/* ================= INIT ================= */

void Arena::initPlayers()
{

    if (!player1 || !player2)
        return;

    const int groundY = 700;

    player1->setGroundY(groundY);
    player2->setGroundY(groundY);

    // centru pe X ținând cont de lățimea sprite-ului
    const int p1X = 200;
    const int p2X = width() - 200;

    player1->setPosition(p1X, groundY);
    player2->setPosition(p2X, groundY);
    player1->setFacingRight(true);
    player2->setFacingRight(false);

}

/* ================= GAME LOOP ================= */

void Arena::updateGame()
{
    if (!playersReady)
        return;

    player1->update();
    player2->update();
    /* ================= MELEE HIT ================= */

    // P1 -> P2
    if (!roundOver &&
        player1->attacking() &&
        player1->isAttackHitActive() &&
        !player1HitRegistered &&
        player1->attackHitbox().intersects(player2->hurtbox()))
    {
        player2->takeDamage(
            player1->getDamage(),
            player1->drawPosition()
        );
        player1HitRegistered = true;
    }

    if (!player1->attacking())
        player1HitRegistered = false;

    // P2 -> P1
    if (!roundOver &&
        player2->attacking() &&
        player2->isAttackHitActive() &&
        !player2HitRegistered &&
        player2->attackHitbox().intersects(player1->hurtbox()))
    {
        player1->takeDamage(
            player2->getDamage(),
            player2->drawPosition()
        );
        player2HitRegistered = true;
    }

    if (!player2->attacking())
        player2HitRegistered = false;

    for (auto& p : projectiles)
        p->update();

    /* ===== PROJECTILES COLLISION ===== */
    for (auto it = projectiles.begin(); it != projectiles.end(); )
    {
        Projectile* p = it->get();

        if (p->isOutOfBounds(rect()))
        {
            it = projectiles.erase(it);
            continue;
        }

        if (p->owner() == player1 &&
            p->hitbox().intersects(player2->hurtbox()))
        {
            player2->takeDamage(p->getDamage(), p->position().toPoint());
            it = projectiles.erase(it);
            continue;
        }

        if (p->owner() == player2&&
            p->hitbox().intersects(player1->hurtbox()))
        {
            player1->takeDamage(p->getDamage(), p->position().toPoint());
            it = projectiles.erase(it);
            continue;
        }

        ++it;
    }

    /* ===== ROUND SYSTEM ===== */
    if (!roundOver)
    {
        if (player1->isDead())
        {
            player2Rounds++;
            consecutiveWinsP2++;
            consecutiveWinsP1 = 0;
            roundOver = true;
        }
        else if (player2->isDead())
        {
            player1Rounds++;
            consecutiveWinsP1++;
            consecutiveWinsP2 = 0;
            roundOver = true;
        }

        if (roundOver)
        {
            freezeInput = true;
            showRoundText = true;

            if (consecutiveWinsP1 == 2 ||
                consecutiveWinsP2 == 2 ||
                roundNumber == 3)
            {
                gameOver = true;
                winnerName = (player1Rounds > player2Rounds)
                             ? player1->name()
                             : player2->name();
                gameTimer.stop();
                createVictoryUI();
                return;
            }

            roundNumber++;
            roundText = QString("ROUND %1").arg(roundNumber);

            QTimer::singleShot(1200, this, [this]()
            {
                resetRound();
                roundText = "FIGHT!";

                QTimer::singleShot(800, this, [this]()
                {
                    showRoundText = false;
                    freezeInput = false;
                });
            });
        }
    }

    update();
}

/* ================= RENDER ================= */

void Arena::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    /* ================= BACKGROUND ================= */
    painter.drawPixmap(rect(), background);

    if (!playersReady)
        return;

    /* ================= PLAYERS ================= */
    if (!gameOver) {
        painter.drawPixmap(player1->drawPosition(), player1->currentFrame());
        painter.drawPixmap(player2->drawPosition(), player2->currentFrame());
    }

    /* ================= PROJECTILES ================= */
    for (const auto& p : projectiles)
    {
        QPixmap frame = p->currentFrame();
        if (frame.isNull())
            continue;

        painter.drawPixmap(
            QPoint(
                int(p->position().x() - frame.width() / 2),
                int(p->position().y() - frame.height() / 2)
            ),
            frame
        );
    }

    if (!gameOver) {
        /* ================= HUD BACKGROUND ================= */
        QRect hudRect(0, 0, width(), 110);
        painter.fillRect(hudRect, QColor(0, 0, 0, 130));

        painter.setPen(Qt::white);

        /* ================= PLAYER 1 HUD ================= */
        painter.drawText(30, 30, player1->name());
        drawHpBar(painter, player1, QRect(30, 45, 300, 18));

        painter.drawText(30, 80,
            QString("Wins: %1").arg(player1Rounds));

        /* ================= PLAYER 2 HUD ================= */
        painter.drawText(width() - 330, 30, player2->name());
        drawHpBar(painter, player2, QRect(width() - 330, 45, 300, 18));

        painter.drawText(width() - 330, 80,
            QString("Wins: %1").arg(player2Rounds));
    }

    /* ================= ROUND TEXT ================= */
    if (showRoundText && !gameOver)
    {
        painter.setPen(Qt::white);
        QFont f;
        f.setPointSize(42);
        f.setBold(true);
        painter.setFont(f);

        painter.drawText(rect(), Qt::AlignCenter, roundText);
    }

    /* ================= VICTORY ================= */

}


/* ================= VICTORY UI ================= */

void Arena::createVictoryUI()
{
    if (backToMenuBtn)
        return;

    /* ===== OVERLAY (ASCUNDE ARENA + JUCĂTORI) ===== */
    QLabel* overlay = new QLabel(this);
    overlay->setGeometry(rect());
    overlay->setStyleSheet("background-color: rgba(0, 0, 0, 180);");
    overlay->show();

    /* ===== VICTORY TEXT ===== */
    QLabel* victoryText = new QLabel("VICTORY", this);
    victoryText->setAlignment(Qt::AlignCenter);
    victoryText->setStyleSheet(
        "color: yellow;"
        "font-size: 48px;"
        "font-weight: bold;"
    );
    victoryText->setGeometry(0, 140, width(), 60);
    victoryText->show();

    /* ===== WINNER NAME ===== */
    QLabel* winnerNameLabel = new QLabel(winnerName, this);
    winnerNameLabel->setAlignment(Qt::AlignCenter);
    winnerNameLabel->setStyleSheet(
        "color: white;"
        "font-size: 32px;"
        "font-weight: bold;"
    );
    winnerNameLabel->setGeometry(0, 200, width(), 50);
    winnerNameLabel->show();

    /* ===== WINNER IMAGE ===== */
    Gladiator* winner =
        (winnerName == player1->name())
            ? player1
            : player2;

    QLabel* winnerImage = new QLabel(this);
    winnerImage->setAttribute(Qt::WA_TransparentForMouseEvents);

    QPixmap pix = winner->currentFrame();
    if (!pix.isNull())
    {
        pix = pix.scaled(
            pix.width() * 1.4,
            pix.height() * 1.4,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        );

        winnerImage->setPixmap(pix);
        winnerImage->setFixedSize(pix.size());

        winnerImage->move(
            width() / 2 - pix.width() / 2,
            260
        );
    }
    winnerImage->show();

    /* ===== BACK TO MENU BUTTON ===== */
    backToMenuBtn = new QPushButton("Back to Menu", this);
    backToMenuBtn->setGeometry(
        width() / 2 - 110,
        height() - 160,
        220,
        50
    );

    backToMenuBtn->setStyleSheet(
        "font-size:18px;"
        "background:#222;"
        "color:white;"
        "border:2px solid white;"
    );

    connect(backToMenuBtn, &QPushButton::clicked, this,
        [this, overlay, victoryText, winnerNameLabel, winnerImage]()
    {
        overlay->deleteLater();
        victoryText->deleteLater();
        winnerNameLabel->deleteLater();
        winnerImage->deleteLater();

        resetArena();
        emit backToMenu();
    });

    backToMenuBtn->show();
}


/* ================= INPUT ================= */

void Arena::keyPressEvent(QKeyEvent* event)
{
    if (!playersReady || gameOver || freezeInput || event->isAutoRepeat())
        return;

    switch (event->key())
    {
        /* ===== PLAYER 1 ===== */
        case Qt::Key_G:
            player1->startBlock();
            break;

        case Qt::Key_F:
        {
            if (player1->blocking())
                break;

            if (player1->getType() == Gladiator::GladiatorType::Spear ||
                player1->getType() == Gladiator::GladiatorType::Wizard)
            {
                QPoint startPos =
                    player1->drawPosition() +
                    QPoint(player1->isFacingRight() ? 40 : -40, -20);

                emit player1->shootProjectile(
                    startPos,
                    player1->isFacingRight()
                );
            }
            else
            {
                player1->attack();
            }
            break;
        }

        case Qt::Key_A: player1->moveLeft(); break;
        case Qt::Key_D: player1->moveRight(); break;
        case Qt::Key_W:
            if (!player1->blocking()) player1->jump();
            break;

        /* ===== PLAYER 2 ===== */
        case Qt::Key_O:
            player2->startBlock();
            break;

        case Qt::Key_I:
        {
            if (player2->blocking())
                break;

            if (player2->getType() == Gladiator::GladiatorType::Spear ||
                player2->getType() == Gladiator::GladiatorType::Wizard)
            {
                QPoint startPos =
                    player2->drawPosition() +
                    QPoint(player2->isFacingRight() ? 40 : -40, -20);

                emit player2->shootProjectile(
                    startPos,
                    player2->isFacingRight()
                );
            }
            else
            {
                player2->attack();
            }
            break;
        }

        case Qt::Key_Left:  player2->moveLeft(); break;
        case Qt::Key_Right: player2->moveRight(); break;
        case Qt::Key_Up:
            if (!player2->blocking()) player2->jump();
            break;
    }
}

void Arena::keyReleaseEvent(QKeyEvent* event)
{
    if (!playersReady || gameOver || event->isAutoRepeat())
        return;

    switch (event->key())
    {
        case Qt::Key_A:
        case Qt::Key_D:
            player1->stop();
            break;

        case Qt::Key_G:
            player1->stopBlock();
            break;

        case Qt::Key_Left:
        case Qt::Key_Right:
            player2->stop();
            break;

        case Qt::Key_O:
            player2->stopBlock();
            break;
    }
}

/* ================= HP BAR ================= */

void Arena::drawHpBar(QPainter& p, Gladiator* g, QRect area)
{
    float ratio = float(g->getHp()) / g->getMaxHp();

    p.setBrush(Qt::black);
    p.drawRect(area);

    p.setBrush(Qt::red);
    p.drawRect(area.x(), area.y(),
               int(area.width() * ratio),
               area.height());
}

/* ================= RESET ================= */

void Arena::resetArena()
{
    gameTimer.stop();

    player1 = nullptr;
    player2 = nullptr;

    playersReady = false;

    /* ===== RESET COMBAT ===== */
    player1HitRegistered = false;
    player2HitRegistered = false;

    /* ===== RESET GAME STATE ===== */
    gameOver = false;
    roundOver = false;
    freezeInput = false;
    showRoundText = false;

    /* ===== RESET ROUNDS ===== */
    roundNumber = 1;

    player1Rounds = 0;
    player2Rounds = 0;

    consecutiveWinsP1 = 0;
    consecutiveWinsP2 = 0;

    roundText.clear();
    winnerName.clear();

    projectiles.clear();

    if (backToMenuBtn)
    {
        backToMenuBtn->deleteLater();
        backToMenuBtn = nullptr;
    }

    gameTimer.start(16);
}

void Arena::onPlayerShoot(const QPoint& startPos)
{
    Gladiator* shooter =
        (sender() == player1) ? player1 : player2;

    Gladiator* target =
        (shooter == player1) ? player2 : player1;

    QPointF dir = target->drawPosition() - startPos;

    const QPixmap& sprite =
        (shooter->getType() == Gladiator::GladiatorType::Wizard)
            ? magicSprite
            : arrowSprite;

    projectiles.push_back(
        std::make_unique<Projectile>(
            QPointF(startPos),
            dir,
            sprite,
            shooter->getDamage(),
            shooter
        )
    );
}
void Arena::resetRound()
{

    projectiles.clear();
    initPlayers();

    player1->resetForNewRound();
    player2->resetForNewRound();
    player1->resetHp();
    player2->resetHp();
    player1HitRegistered = false;
    player2HitRegistered = false;

    roundOver = false;
    update();
}
