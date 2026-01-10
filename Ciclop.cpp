#include "Ciclop.h"

Ciclop::Ciclop(const QString& name)
    : Warrior(name, GladiatorType::Cyclop)
{
}
int Ciclop::getDamage() const { return 40; }
int Ciclop::getMaxHp() const { return 180; }
