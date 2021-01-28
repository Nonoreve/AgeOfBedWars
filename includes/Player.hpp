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
	string _name;
    int _money;
    Base _base;
    vector<Unit> _units; // TODO UnitPool

	void doActionPhase(int actionPhase, int index);

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
// TODO all 3 below belongs to unit system
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

	/**
	 * Check life of all its units and remove those that are dead.
	 */
	void buryCorpses();

    const string &getName() const;

    int getMoney() const;
};


#endif //AGEOFBEDWARS_PLAYER_HPP
