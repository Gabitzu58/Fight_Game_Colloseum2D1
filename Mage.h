#pragma once
#include "gladiator.h"

class Mage : public Gladiator
{
public:
    Mage(const QString& name, GladiatorType type);

    void attack() override;
};

