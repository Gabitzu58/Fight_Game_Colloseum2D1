#pragma once
#include "Warrior.h"

class Swordsman final : public Warrior
{
public:
    explicit Swordsman(const QString& name);

    int getDamage() const override;
    int getMaxHp() const override;
};
