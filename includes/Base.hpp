//
// Created by nonoreve on 1/5/21.
//

#ifndef AGEOFBEDWARS_BASE_HPP
#define AGEOFBEDWARS_BASE_HPP


#include "Battlefield.hpp"

class Base {
private:
    int health;
    // position in cell coordinates
    Position position;

public:
    explicit Base(int health, Position position);

    /**
     * Check for cell occupation
     * @return
     */
    bool canSummon();

    const Position &getPosition() const;
};


#endif //AGEOFBEDWARS_BASE_HPP
