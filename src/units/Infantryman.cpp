//
// Created by nonoreve on 1/5/21.
//

#include "units/Infantryman.hpp"

Infantryman::Infantryman(Position position, Base target) : Unit(10, 10, 4, position, target), dalekMode(false) {}

void Infantryman::upgrade() {
    // we don't check for multiple upgrading, just overwrite it.
    dalekMode = true;
}
