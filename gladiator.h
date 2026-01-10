#pragma once

#include <QPixmap>
#include <QPoint>
#include <QElapsedTimer>
#include <QString>
#include <QRect>
#include <QVector>
#include <QMap>
#include <Projectile.h>
#include <QWidget>
#include "stat.h"
#include "Character.h"

class Gladiator: public QWidget , public Character
{
    Q_OBJECT

    /* ================= TYPES ================= */
public:

    void resetHp()
    {
        hp = Stat<int>(stats.maxHp, stats.maxHp);
        lastDamage = 0;
        damageTimer.invalidate();
    }

    enum class GladiatorType {
        Swordsman,
        Spear,
        Brute,
        Wizard,
        Cyclop
    };

    struct GladiatorStats
    {
        int maxHp;
        int damage;
        int attackRange;
        float moveSpeed;
    };

    enum class Animation
    {
        Idle,
        Run,
        Attack,
        Block
    };

    /* ================= CTOR ================= */
    virtual ~Gladiator() = default;
    explicit Gladiator(
        const QString& name,
        GladiatorType type,
        QWidget* parent = nullptr
    );
    Gladiator(const Gladiator&) = delete;
    Gladiator& operator=(const Gladiator&) = delete;
    /* ================= INFO ================= */

    const QString& name() const { return m_name; }
    bool isDead() const;

    /* ================= POSITION & MOVEMENT ================= */

    void setPosition(int x, int y);
    void setGroundY(int y);

    void moveLeft();
    void moveRight();
    void stop();
    void jump();

    /* ================= ACTIONS ================= */

    virtual void attack() = 0;
    bool attacking() const;

    void startBlock();
    void stopBlock();
    bool blocking() const;

    /* ================= UPDATE & RENDER ================= */

    void update();
    QPixmap currentFrame() const;
    QPoint drawPosition() const;
    virtual std::unique_ptr<Character> clone() const;
    /* ================= COMBAT ================= */

    QRect attackHitbox() const;
    QRect hurtbox() const;

    bool isAttackHitActive() const;
    void takeDamage(int dmg, const QPoint& attackerPos);

    /* ================= STATS ================= */

    int getHp() const { return  hp.get(); }
    virtual int getMaxHp() const = 0;
    int getLastDamage() const { return lastDamage; }
    bool damageRecentlyTaken() const;
    virtual int getDamage() const = 0;
    void setFacingRight(bool right) { facingRight = right; }
    GladiatorType getType() const { return type; }
    void resetForNewRound();
    bool isFacingRight() const { return facingRight; }
    signals:
    void shootProjectile(const QPoint& pos, bool facingRight);

protected:
    /* ================= ATTACK ================= */
    bool isAttacking = false;
    bool attackHitActive = false;
    virtual void loadStats();
    virtual QString typeToFolder(GladiatorType type);
    int attackCooldown = 500;
    QElapsedTimer attackTimer;

    int attackStartupFrames  = 1;
    int attackActiveFrames   = 1;
    int attackRecoveryFrames = 2;

    /* ================= BLOCK ================= */
    bool isBlocking = false;

    /* ================= ANIMATION ================= */
    int frame = 0;

    void setAnimation(Animation anim);
    void loadSheets();
    bool facingRight = true;
    QElapsedTimer projectileCooldownTimer;
    const int projectileCooldownMs = 700;
    QMap<Animation, QVector<QPixmap>> animations;
private:
    /* ================= IDENTITY ================= */

    QString m_name;
    GladiatorType type;

    /* ================= POSITION ================= */

    QPoint position{0, 0};
    int groundY = 0;
    bool isGrounded = true;

    /* ================= ORIENTATION ================= */


    /* ================= ANIMATION ================= */

    Animation currentAnim = Animation::Idle;
    int framesCount = 1;

    int frameWidth = 0;
    int frameHeight = 0;

    QElapsedTimer animTimer;
    int frameTimeMs = 100;

    float scale = 0.45f;

    /* ================= MOVEMENT PHYSICS ================= */

    float velocityX = 0.0f;
    float velocityY = 0.0f;

    float baseMaxSpeed = 6.0f;
    float maxSpeed = 6.0f;
    float acceleration = 0.8f;
    float friction = 0.6f;

    float gravity = 1.2f;
    float jumpForce = 18.0f;

    bool moveLeftPressed = false;
    bool moveRightPressed = false;


    /* ================= HEALTH ================= */

    Stat<int> hp{100, 100};
    int maxHp = 100;
    int lastDamage = 0;
    QElapsedTimer damageTimer;

    /* ================= STATS ================= */

    GladiatorStats stats;

};

