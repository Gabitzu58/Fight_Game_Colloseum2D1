#include "Wizard.h"

Wizard::Wizard(const QString& name)
    : Mage(name, GladiatorType::Wizard)
{
}
int Wizard::getDamage() const
{
    return 25;
}

int Wizard::getMaxHp() const {
    return 150;
}
