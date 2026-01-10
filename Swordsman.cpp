#include "Swordsman.h"

Swordsman::Swordsman(const QString& name)
    : Warrior(name, GladiatorType::Swordsman)
{

}

int Swordsman::getDamage() const
{
    return 25;
}

int Swordsman::getMaxHp() const
{
    return 120;
}

