#include "LobbyOverlay.h"
#include "GladiatorFactory.h"

#include <QLabel>
#include <QPushButton>

/* ================= CONSTRUCTOR ================= */

LobbyOverlay::LobbyOverlay(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(1000, 800);
    setFocusPolicy(Qt::StrongFocus);

    names = {
        "Swordsman",
        "Spear",
        "Brute",
        "Wizard",
        "Ciclop"
    };

    pixmaps = {
        QPixmap(":/images/Eroul1.png"),
        QPixmap(":/images/Eroul3.png"),
        QPixmap(":/images/Eroul2.png"),
        QPixmap(":/images/Eroul4.png"),
        QPixmap(":/images/Eroul5.png")
    };

    buildUI();
    updatePreview(1);
    updatePreview(2);
}

/* ================= UI BUILD ================= */

void LobbyOverlay::buildUI()
{
    auto* bg = new QLabel(this);
    bg->setPixmap(QPixmap(":/images/menuColloseum.png"));
    bg->setScaledContents(true);
    bg->setGeometry(0, 0, width(), height());
    bg->lower();

    title = new QLabel("LOBBY", this);
    title->setGeometry(350, 20, 300, 70);
    title->setAlignment(Qt::AlignCenter);

    s1 = createStatusLabel("PLAYER 1 : SELECTING", 150, "#2E86DE");
    s2 = createStatusLabel("PLAYER 2 : SELECTING", 600, "#C0392B");

    p1Preview = createPreview(150);
    p2Preview = createPreview(600);

    p1Name = createNameLabel(150);
    p2Name = createNameLabel(600);

    createArrow(90, 330, 1, -1);
    createArrow(420, 330, 1, +1);
    createArrow(540, 330, 2, -1);
    createArrow(870, 330, 2, +1);

    p1SelectBtn = new QPushButton("SELECT", this);
    p1SelectBtn->setGeometry(190, 560, 170, 45);
    connect(p1SelectBtn, &QPushButton::clicked,
            this, [this]() { selectGladiator(1); });

    p2SelectBtn = new QPushButton("SELECT", this);
    p2SelectBtn->setGeometry(640, 560, 170, 45);
    connect(p2SelectBtn, &QPushButton::clicked,
            this, [this]() { selectGladiator(2); });

    startBtn = new QPushButton("ENTER GAME", this);
    startBtn->setGeometry(390, 690, 220, 60);
    startBtn->setEnabled(false);
    connect(startBtn, &QPushButton::clicked,
            this, &LobbyOverlay::onStartClicked);
}

/* ================= HELPERS ================= */

QLabel* LobbyOverlay::createStatusLabel(const QString& text, int x, const QString& color)
{
    auto* lbl = new QLabel(text, this);
    lbl->setGeometry(x, 100, 250, 45);
    lbl->setAlignment(Qt::AlignCenter);
    lbl->setStyleSheet("background:" + color + "; color:white;");
    return lbl;
}

QLabel* LobbyOverlay::createPreview(int x)
{
    auto* lbl = new QLabel(this);
    lbl->setGeometry(x, 180, 250, 320);
    lbl->setScaledContents(true);
    return lbl;
}

QLabel* LobbyOverlay::createNameLabel(int x)
{
    auto* lbl = new QLabel(this);
    lbl->setGeometry(x, 510, 250, 30);
    lbl->setAlignment(Qt::AlignCenter);
    return lbl;
}

void LobbyOverlay::createArrow(int x, int y, int player, int dir)
{
    auto* btn = new QPushButton(dir < 0 ? "<" : ">", this);
    btn->setGeometry(x, y, 50, 50);

    connect(btn, &QPushButton::clicked, this, [this, player, dir]()
    {
        if (player == 1 && !p1Selected) {
            p1Index = (p1Index + dir + pixmaps.size()) % pixmaps.size();
            updatePreview(1);
        }
        if (player == 2 && !p2Selected) {
            p2Index = (p2Index + dir + pixmaps.size()) % pixmaps.size();
            updatePreview(2);
        }
    });
}

/* ================= LOGIC ================= */

void LobbyOverlay::updatePreview(int player)
{
    if (player == 1) {
        p1Preview->setPixmap(pixmaps[p1Index]);
        p1Name->setText(names[p1Index]);
    } else {
        p2Preview->setPixmap(pixmaps[p2Index]);
        p2Name->setText(names[p2Index]);
    }
}

void LobbyOverlay::selectGladiator(int player)
{
    if (player == 1 && !p1Selected)
    {
        p1 = GladiatorFactory::create(
            static_cast<Gladiator::GladiatorType>(p1Index),
            names[p1Index]
        );
        p1Selected = true;
        s1->setText("PLAYER 1 : READY");

    }

    if (player == 2 && !p2Selected)
    {
        p2 = GladiatorFactory::create(
            static_cast<Gladiator::GladiatorType>(p2Index),
            names[p2Index]
        );
        p2Selected = true;
        s2->setText("PLAYER 2 : READY");

    }

    if (p1Selected && p2Selected)
        startBtn->setEnabled(true);
}

void LobbyOverlay::onStartClicked()
{
    if (p1 && p2) {
        emit startGame(p1.release(), p2.release());
        qDebug() << "Emit startGame";
    }
}

void LobbyOverlay::ResetLobby()
{
    p1.reset();
    p2.reset();

    p1Index = 0;
    p2Index = 0;

    p1Selected = false;
    p2Selected = false;

    s1->setText("PLAYER 1 : SELECTING");
    s2->setText("PLAYER 2 : SELECTING");
    title->setText("LOBBY");

    p1SelectBtn->setEnabled(true);
    p2SelectBtn->setEnabled(true);
    startBtn->setEnabled(false);

    updatePreview(1);
    updatePreview(2);
}
