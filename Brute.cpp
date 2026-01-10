#include "Brute.h"

Brute::Brute(const QString& name)
    : Warrior(name, GladiatorType::Brute)
{
}
int Brute::getDamage() const { return 40; }
int Brute::getMaxHp() const { return 180; }

