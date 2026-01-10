#include "Projectile.h"

Projectile::Projectile(QPointF startPos,
                       QPointF direction,
                       const QPixmap& sprite,
                       int dmg,
                       Gladiator* owner,
                       float speed)
    : pos(startPos),
      pixmap(sprite),
      damage(dmg),
      m_owner(owner)
{
    // normalize direction
    float len = std::sqrt(direction.x()*direction.x()
                        + direction.y()*direction.y());

    if (len != 0.0f)
        direction /= len;

    velocity = direction * speed;

    // rotație
    rotationAngle = std::atan2(direction.y(), direction.x()) * 180.0f / M_PI;
}

void Projectile::update()
{
    pos += velocity;
}

QRect Projectile::hitbox() const
{
    int w = int(pixmap.width() * scale);
    int h = int(pixmap.height() * scale);

    return QRect(
        int(pos.x() - w / 2),
        int(pos.y() - h / 2),
        w,
        h
    );
}

QPixmap Projectile::currentFrame() const
{
    if (pixmap.isNull())
        return QPixmap();

    QTransform t;
    t.scale(scale, scale);
    t.rotate(rotationAngle);

    return pixmap.transformed(t, Qt::SmoothTransformation);
}

bool Projectile::isOutOfBounds(const QRect& arena) const
{
    return !arena.intersects(hitbox());
}
