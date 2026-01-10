#pragma once
#include "Warrior.h"

class Ciclop final : public Warrior
{
public:
    explicit Ciclop(const QString& name);
    int getDamage() const override;
    int getMaxHp() const override;


};
