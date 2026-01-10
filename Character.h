#include <iosfwd>
#include <memory>
#include <string>

#include "stat.h"


class Character {
public:
    Character(std::string name, int hp);
    virtual ~Character() = default;

    Character(const Character&) = default;
    Character& operator=(const Character&) = default;

    virtual void attack() = 0;   // funcție virtuală PURĂ
    virtual std::unique_ptr<Character> clone() const = 0;

    virtual void print(std::ostream& os) const;

protected:
    std::string name;
    Stat<int> hp;
};

std::ostream& operator<<(std::ostream& os, const Character& c);
