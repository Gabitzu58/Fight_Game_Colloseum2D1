#pragma once
#include "Gladiator.h"

class Warrior : public Gladiator
{
public:
    explicit Warrior(const QString& name,
                     Gladiator::GladiatorType type,
                     QWidget* parent = nullptr);

    void attack() override;
};
