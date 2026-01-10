#pragma once
#include "Mage.h"

class Spear final : public Mage
{
public:
    explicit Spear(const QString& name);
    int getDamage() const override;
    int getMaxHp() const override;
};
