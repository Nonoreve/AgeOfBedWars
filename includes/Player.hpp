//
// Created by nonoreve on 1/5/21.
//

#ifndef AGEOFBEDWARS_PLAYER_HPP
#define AGEOFBEDWARS_PLAYER_HPP

#include "Base.hpp"
#include "units/Unit.hpp"

#include <string>
#include <vector>
#include <units/UnitPool.hpp>

using std::string, std::vector;

class Player {
private:
	string _name;
	int _money;
	Base _base;

public:
	Player(string name, int initialMoneyAmount, Base base);

	/**
	 * Provide information on the player.
	 * @return
	 */
	string report() const;

	/**
	 * Increase the money the player has by the given amount.
	 * @param money
	 */
	void pay(int money);

	bool canAfford(int price);

	void buy(UnitType unitType);

	inline const string &getName() const {
		return _name;
	}

	const Base &getBase() const {
		return _base;
	}
};


#endif //AGEOFBEDWARS_PLAYER_HPP
