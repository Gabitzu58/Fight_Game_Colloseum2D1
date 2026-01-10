#pragma once
#include "Warrior.h"

class Brute final : public Warrior
{
public:
    explicit Brute(const QString& name);
    int getDamage() const override;
    int getMaxHp() const override;

};
