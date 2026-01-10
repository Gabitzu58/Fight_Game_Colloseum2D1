#include "GladiatorFactory.h"
#include "Swordsman.h"
#include "Brute.h"
#include "Ciclop.h"
#include "Wizard.h"
#include "Spear.h"

std::unique_ptr<Gladiator>
GladiatorFactory::create(Gladiator::GladiatorType type, const QString& name)
{
    switch (type)
    {
        case Gladiator::GladiatorType::Swordsman:
            return std::make_unique<Swordsman>(name);

        case Gladiator::GladiatorType::Brute:
            return std::make_unique<Brute>(name);

        case Gladiator::GladiatorType::Cyclop:
            return std::make_unique<Ciclop>(name);

        case Gladiator::GladiatorType::Wizard:
            return std::make_unique<Wizard>(name);

        case Gladiator::GladiatorType::Spear:
            return std::make_unique<Spear>(name);   // ✅ Mage

        default:
            Q_ASSERT(false);
            return nullptr;
    }
}
