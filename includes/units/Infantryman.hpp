//
// Created by nonoreve on 1/5/21.
//

#ifndef AGEOFBEDWARS_INFANTRYMAN_HPP
#define AGEOFBEDWARS_INFANTRYMAN_HPP

#include "Unit.hpp"

class Infantryman : public Unit {
private:
    bool dalekMode;

public:
    Infantryman(Position position, Base target);
    void upgrade();

};


#endif //AGEOFBEDWARS_INFANTRYMAN_HPP
