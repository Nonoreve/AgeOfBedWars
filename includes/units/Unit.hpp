//
// Created by nonoreve on 1/5/21.
//

#ifndef AGEOFBEDWARS_UNIT_HPP
#define AGEOFBEDWARS_UNIT_HPP

#include "Battlefield.hpp"

class Unit {
private:
    int price;
    int health;
    int strikePower;
    Position position;
    Base target;

protected:
    Unit(int price, int health, int strikePower, Position position, Base target);

public:
    virtual void attack() = 0;

    virtual void move() = 0;

    virtual bool canAfford(int money) = 0;

};


#endif //AGEOFBEDWARS_UNIT_HPP
