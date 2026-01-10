
#include "Character.h"
#include <iostream>
Character::Character(std::string name, int hp) : name(name), hp(hp,hp){

}

void Character::print(std::ostream& os) const {
    os << "Name: " << name << ", HP: " << hp;
}

std::ostream& operator<<(std::ostream& os, const Character& c) {
    c.print(os);
    return os;
}
