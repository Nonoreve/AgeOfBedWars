//
// Created by nonoreve on 1/5/21.
//

#include <units/Infantryman.hpp>
#include "units/Unit.hpp"
#include "Battlefield.hpp"

Unit::Unit(int price, int health, int strikePower, Position position, Base target) : price(price), health(health),
                                                                                     strikePower(strikePower),
                                                                                     position(position),
                                                                                     target(target) {}
