#pragma once
#include "Mage.h"

class Wizard final : public Mage
{
public:
    explicit Wizard(const QString& name);
    int getDamage() const override;
    int getMaxHp() const override;

};


