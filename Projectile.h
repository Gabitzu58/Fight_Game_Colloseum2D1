#pragma once

#include <QPixmap>
#include <QPointF>
#include <QRect>
#include <QTransform>
#include <cmath>

class Gladiator;

class Projectile
{
public:
    Projectile(QPointF startPos,
               QPointF direction,
               const QPixmap& sprite,
               int damage,
               Gladiator* owner,
               float speed = 12.0f);

    void update();
    QRect hitbox() const;
    bool isOutOfBounds(const QRect& arena) const;

    QPointF position() const { return pos; }
    int getDamage() const { return damage; }
    Gladiator* owner() const { return m_owner; }
    QPixmap currentFrame() const;

private:
    QPointF pos;
    QPointF velocity;
    int damage = 0;
    QPixmap pixmap;
    float rotationAngle = 0.0f;
    Gladiator* m_owner = nullptr;
    float scale = 0.4f;
};
