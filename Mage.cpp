#include "Mage.h"

Mage::Mage(const QString& name, GladiatorType type)
    : Gladiator(name, type)
{
}

void Mage::attack()
{
    if (isAttacking)
        return;

    if (projectileCooldownTimer.elapsed() < projectileCooldownMs)
        return;

    isAttacking = true;
    attackTimer.restart();
    setAnimation(Animation::Attack);

    emit shootProjectile(drawPosition(), facingRight);
    projectileCooldownTimer.restart();
}
