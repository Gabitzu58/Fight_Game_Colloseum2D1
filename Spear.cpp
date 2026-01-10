#include "Spear.h"

Spear::Spear(const QString& name)
    : Mage(name, GladiatorType::Spear)
{
}
int Spear::getDamage() const
{
    return 25;
}

int Spear::getMaxHp() const {
    return 120;
}