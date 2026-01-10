#include "Gladiator.h"
#include <QTransform>

/* ================= CTOR ================= */

Gladiator::Gladiator(const QString& name,
                     GladiatorType t,
                     QWidget* parent)
    : QWidget(parent),
      Character(name.toStdString(), 200),
      m_name(name),
      type(t)
{
    projectileCooldownTimer.start();

    loadStats();
    loadSheets();

    hp = Stat<int>(stats.maxHp, stats.maxHp);

    setAnimation(Animation::Idle);

    animTimer.start();
    attackTimer.start();
    damageTimer.invalidate();
}


/* ================= INFO ================= */

bool Gladiator::isDead() const
{
    return hp.get() <= 0;
}

/* ================= POSITION ================= */

void Gladiator::setGroundY(int y)
{
    groundY = y;
    position.setY(groundY);
}

void Gladiator::setPosition(int x, int y)
{
    position.setX(x);
    position.setY(y);
}

/* ================= MOVEMENT ================= */

void Gladiator::moveLeft()
{
    facingRight = false;
    moveLeftPressed = true;
    moveRightPressed = false;

    if (!isBlocking)
        setAnimation(Animation::Run);
}

void Gladiator::moveRight()
{
    facingRight = true;
    moveRightPressed = true;
    moveLeftPressed = false;

    if (!isBlocking)
        setAnimation(Animation::Run);
}

void Gladiator::stop()
{
    if (isBlocking)
        return;

    moveLeftPressed = moveRightPressed = false;
    velocityX = 0;
    setAnimation(Animation::Idle);
}


void Gladiator::jump()
{
    if (!isGrounded || isBlocking)
        return;

    velocityY = -jumpForce;
    isGrounded = false;
}

/* ================= ACTIONS ================= */



bool Gladiator::attacking() const
{
    return isAttacking;
}

void Gladiator::startBlock()
{
    if (isAttacking)
        return;

    isBlocking = true;

    // 🔒 îngheață mișcarea
    moveLeftPressed = false;
    moveRightPressed = false;
    velocityX = 0;

    setAnimation(Animation::Block);
}


void Gladiator::stopBlock()
{
    isBlocking = false;

    // 🔒 reset input de mișcare
    moveLeftPressed = false;
    moveRightPressed = false;
    velocityX = 0;

    setAnimation(Animation::Idle);
}

bool Gladiator::blocking() const
{
    return isBlocking;
}

/* ================= UPDATE ================= */

void Gladiator::update()
{
    /* ===== BLOCK LOGIC ===== */
    if (isBlocking)
    {
        velocityX = 0;

        // 👉 permitem doar schimbarea direcției
        if (moveLeftPressed)
            facingRight = false;
        else if (moveRightPressed)
            facingRight = true;
    }

    /* ===== PHYSICS ===== */

    if (!isBlocking)   // 🛡️ blocarea îngheață mișcarea
    {
        if (moveLeftPressed)
            velocityX = -stats.moveSpeed;
        else if (moveRightPressed)
            velocityX = stats.moveSpeed;
        else
            velocityX *= friction;
    }
    else
    {
        velocityX = 0;
    }

    position.rx() += int(velocityX);

    velocityY += gravity;
    position.ry() += int(velocityY);

    if (position.y() >= groundY)
    {
        position.setY(groundY);
        velocityY = 0;
        isGrounded = true;
    }

    /* ===== ATTACK TIMING ===== */

    if (isAttacking && attackTimer.elapsed() > attackCooldown)
    {
        isAttacking = false;
        attackHitActive = false;
        setAnimation(Animation::Idle);
    }

    /* ===== ANIMATION ===== */

    if (animTimer.elapsed() > frameTimeMs)
    {
        frame = (frame + 1) % framesCount;
        animTimer.restart();
    }
    /* ===== SCREEN CLAMP ===== */

    const int halfWidth = int(frameWidth * scale) / 2;

    // stânga
    if (position.x() < halfWidth)
        position.setX(halfWidth);

    // dreapta (1000 = lățimea arenei)
    const int screenWidth = 1000;
    if (position.x() > screenWidth - halfWidth)
        position.setX(screenWidth - halfWidth);

}


/* ================= RENDER ================= */

QPixmap Gladiator::currentFrame() const
{
    if (!animations.contains(currentAnim))
        return QPixmap();

    const auto& frames = animations[currentAnim];
    if (frames.isEmpty())
        return QPixmap();

    int safeFrame = frame % frames.size();
    QPixmap pix = frames[safeFrame];

    if (pix.isNull())
        return QPixmap();

    pix = pix.scaled(
        int(frameWidth * scale),
        int(frameHeight * scale),
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation
    );

    if (!facingRight)
        pix = pix.transformed(QTransform().scale(-1, 1));

    return pix;
}


QPoint Gladiator::drawPosition() const
{
    return QPoint(
        position.x() - int(frameWidth * scale) / 2,
        position.y() - int(frameHeight * scale)
    );
}

/* ================= COMBAT ================= */

QRect Gladiator::hurtbox() const
{
    return QRect(
        drawPosition(),
        QSize(
            int(frameWidth * scale),
            int(frameHeight * scale)
        )
    );
}

QRect Gladiator::attackHitbox() const
{
    int width = stats.attackRange;
    int height = int(frameHeight * scale * 0.5f);

    int x = facingRight
        ? position.x()
        : position.x() - width;

    int y = position.y() - height;

    return QRect(x, y, width, height);
}

bool Gladiator::isAttackHitActive() const
{
    return attackHitActive;
}

void Gladiator::takeDamage(int dmg, const QPoint& attackerPos)
{
    if (isDead())
        return;

    bool attackFromFront =
        (facingRight && attackerPos.x() > position.x()) ||
        (!facingRight && attackerPos.x() <  position.x());

    // 🛡️ BLOCK ACTIV + atac din FAȚĂ → zero damage
    if (isBlocking && attackFromFront)
    {
        lastDamage = 0;
        damageTimer.restart();
        return;
    }

    // ❌ atac din SPATE sau nu blochează → damage normal
    hp.decrease(dmg);

    lastDamage = dmg;
    damageTimer.restart();

    if (hp.get() < 0)
        hp.decrease(dmg);

}

/* ================= DAMAGE ================= */

bool Gladiator::damageRecentlyTaken() const
{
    return damageTimer.isValid() && damageTimer.elapsed() < 400;
}

/* ================= INTERNAL ================= */

void Gladiator::setAnimation(Animation anim)
{
    if (currentAnim == anim)
        return;
    if (!animations.contains(anim) || animations[anim].isEmpty())
        return; // 🔴 prevenim crash / pixmap null
    currentAnim = anim;
    frame = 0;
    framesCount = animations[currentAnim].size();
    animTimer.restart();
}

void Gladiator::loadStats()
{
    switch (type)
    {
    case GladiatorType::Swordsman:
        stats = { 150, 15, 60, 6.0f };
        scale = 0.45f;
        break;


        case GladiatorType::Brute:
            stats = { 220, 30, 50, 4.0f };
            scale = 0.60f;
            break;

            case GladiatorType::Spear:
            stats = { 80, 20, 50, 5.5f };
            scale = 0.45f;
            break;


        case GladiatorType::Wizard:
            stats = { 100, 15, 60, 6.0f };
            scale = 0.45f;
            break;

        case GladiatorType::Cyclop:
            stats = { 220, 30, 90, 4.0f };
            scale = 0.60f;
            break;

    default:
        stats = { 120, 18, 65, 5.0f };
        scale = 0.45f;
        break;
    }
}

QString Gladiator::typeToFolder(GladiatorType type )
{
    switch (type)
    {
        case GladiatorType::Swordsman: return "Eroul1";
        case GladiatorType::Spear:     return "Eroul3";
        case GladiatorType::Brute:     return "Eroul2";
        case GladiatorType::Wizard:    return "Eroul4";
        case GladiatorType::Cyclop:    return "Eroul5";
    }
    return "Eroul1";
}

void Gladiator::loadSheets()
{
    QString base = ":/images/" + typeToFolder(type);

    animations[Animation::Idle] = {
        QPixmap(base + "/idle/idle0.png"),
        QPixmap(base + "/idle/idle1.png"),
        QPixmap(base + "/idle/idle2.png"),
        QPixmap(base + "/idle/idle3.png")
    };

    animations[Animation::Run] = {
        QPixmap(base + "/run/run0.png"),
        QPixmap(base + "/run/run1.png"),
        QPixmap(base + "/run/run2.png"),
        QPixmap(base + "/run/run3.png")
    };

    animations[Animation::Attack] = {
        QPixmap(base + "/attack/attack0.png"),
        QPixmap(base + "/attack/attack1.png"),
        QPixmap(base + "/attack/attack2.png"),
        QPixmap(base + "/attack/attack3.png")
    };

    animations[Animation::Block] = {
        QPixmap(base + "/block/block0.png"),
        QPixmap(base + "/block/block1.png")
    };

    frameWidth  = animations[Animation::Idle][0].width();
    frameHeight = animations[Animation::Idle][0].height();
}

void Gladiator::resetForNewRound()
{
    hp = Stat<int>(hp.getMax(), hp.getMax());

    isAttacking = false;
    isBlocking = false;
    attackHitActive = false;


    frame = 0;
}
std::unique_ptr<Character> Gladiator::clone() const
{
    Q_ASSERT_X(false, "Gladiator::clone",
               "clone() called on base Gladiator");
    return nullptr;
}