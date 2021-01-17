//
// Created by nonoreve on 1/5/21.
//

#ifndef AGEOFBEDWARS_PLAYER_HPP
#define AGEOFBEDWARS_PLAYER_HPP

#include "Base.hpp"
#include "units/Unit.hpp"

#include <string>
#include <vector>

using std::string, std::vector;

class Player {
private:
    string name;

private:
    int money;
    Base base;
    vector<Unit> units;

public:
    Player(string name, int initialMoneyAmount, Base base);

    /**
     * Provide information on the player.
     * @return
     */
    string report();

    /**
     * Current player's turn to play all his unit's actions.
     */
    void play();

    /**
     * Increase the money the player has by the given amount.
     * @param money
     */
    void pay(int money);

    /**
     * Only check for cell occupation
     * @return
     */
    bool canSummon();

    /**
     *
     * @param unitType the type name of the unit to try to summon
     * @param target the Base of the opponent
     * @return false if the summing failed. (not enough money or cell occupied)
     */
    bool summon(string unitType, Base target);

    const string &getName() const;

    int getMoney() const;
};


#endif //AGEOFBEDWARS_PLAYER_HPP
