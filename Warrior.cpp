#include "Warrior.h"

Warrior::Warrior(const QString& name,
                 Gladiator::GladiatorType type,
                 QWidget* parent)
    : Gladiator(name, type, parent)
{
}

void Warrior::attack()
{
    if (isAttacking)
        return;

    isAttacking = true;
    attackHitActive = true;
    attackTimer.restart();
    setAnimation(Animation::Attack);
}
