#include "SettingsOverlay.h"

#include <QLabel>
#include <QPushButton>
#include <QFrame>

SettingsOverlay::SettingsOverlay(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(1000, 800);

    // Overlay semi-transparent
    setStyleSheet("background: rgba(0,0,0,140);");

    QLabel* bg = new QLabel(this);
    bg->setPixmap(QPixmap(":/images/menuColloseum.png"));
    bg->setScaledContents(true);
    bg->setGeometry(0, 0, width(), height());
    bg->lower();


    // =====================
    // TITLE
    // =====================
    QLabel* title = new QLabel("PLAYER CONTROLS", this);
    title->setGeometry(300, 40, 400, 60);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "background:#B37A3C;"
        "color:#FFE8B0;"
        "font-size:26px;"
        "font-weight:900;"
        "border-radius:12px;"
    );

    // =====================
    // PLAYER HEADERS
    // =====================
    QLabel* p1Title = new QLabel("PLAYER 1", this);
    p1Title->setGeometry(150, 130, 200, 40);
    p1Title->setAlignment(Qt::AlignCenter);
    p1Title->setStyleSheet(
        "background:#3B7DDD;"
        "color:white;"
        "font-weight:bold;"
        "border-radius:8px;"
    );

    QLabel* p2Title = new QLabel("PLAYER 2", this);
    p2Title->setGeometry(650, 130, 200, 40);
    p2Title->setAlignment(Qt::AlignCenter);
    p2Title->setStyleSheet(
        "background:#C15A5A;"
        "color:white;"
        "font-weight:bold;"
        "border-radius:8px;"
    );

    // =====================
    // STYLES
    // =====================
    const QString p1Style =
        "color:#7FC9FF;"
        "font-size:16px;"
        "font-weight:bold;";

    const QString p2Style =
        "color:#FF9C9C;"
        "font-size:16px;"
        "font-weight:bold;";

    // =====================
    // PLAYER 1 CONTROLS
    // =====================
    int y1 = 200;
    auto addP1 = [&](const QString& text)
    {
        QLabel* l = new QLabel(text, this);
        l->setGeometry(170, y1, 260, 30);
        l->setStyleSheet(p1Style);
        y1 += 36;
    };

    addP1("Jump: ⭡");
    addP1("Move Left: ⭠");
    addP1("Move Right: ⭢");
    addP1("Attack: I");
    addP1("Block: O");

    // =====================
    // PLAYER 2 CONTROLS
    // =====================
    int y2 = 200;
    auto addP2 = [&](const QString& text)
    {
        QLabel* l = new QLabel(text, this);
        l->setGeometry(570, y2, 260, 30);
        l->setStyleSheet(p2Style);
        y2 += 36;
    };

    addP2("Jump: W");
    addP2("Move Left: A");
    addP2("Move Right: D");
    addP2("Attack: F");
    addP2("Block: G");


    // =====================
    // SEPARATOR
    // =====================
    QFrame* line = new QFrame(this);
    line->setGeometry(width() / 2 - 2, 150, 4, 260);
    line->setStyleSheet("background:#D4AF37;");

    // =====================
    // BACK BUTTON
    // =====================
    QPushButton* backBtn = new QPushButton("BACK", this);
    backBtn->setGeometry(400, 650, 200, 55);
    backBtn->setStyleSheet(
        "background:#B37A3C;"
        "color:white;"
        "font-size:22px;"
        "font-weight:900;"
        "border-radius:12px;"
    );

    connect(backBtn, &QPushButton::clicked,
            this, &SettingsOverlay::backPressed);
}
