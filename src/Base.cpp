//
// Created by nonoreve on 1/5/21.
//

#include "Base.hpp"

Base::Base(int health, Position position) : health(health), position(position) {}

bool Base::canSummon() {
    // TODO
    return false;
}

const Position &Base::getPosition() const {
    return position;
}
