#pragma once
#include <memory>
#include "gladiator.h"

class GladiatorFactory {
public:
    static std::unique_ptr<Gladiator>
    create(Gladiator::GladiatorType type, const QString& name);
};

