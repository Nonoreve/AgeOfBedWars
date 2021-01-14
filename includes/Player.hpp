//
// Created by nonoreve on 1/5/21.
//

#ifndef AGEOFBEDWARS_PLAYER_HPP
#define AGEOFBEDWARS_PLAYER_HPP

#include "Base.hpp"

#include <string>

using std::string;

class Player {
private:
    string name;
    int money;
    Base base;

public:
    Player(string name, int initialMoneyAmount, Base base);
};


#endif //AGEOFBEDWARS_PLAYER_HPP
