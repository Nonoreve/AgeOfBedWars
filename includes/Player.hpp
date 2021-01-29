//
// Created by nonoreve on 1/5/21.
//

#ifndef AGEOFBEDWARS_PLAYER_HPP
#define AGEOFBEDWARS_PLAYER_HPP

#include "Base.hpp"
#include "units/UnitType.hpp"

#include <string>
#include <vector>

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

	bool canAfford(int price) const;

	void buy(UnitType unitType);

	bool operator==(const Player &rhs) const;

	inline const string &getName() const {
		return _name;
	}

	const Base &getBase() const {
		return _base;
	}

	constexpr static int UNIT_PRICES[] = {10, 12, 20};
};


#endif //AGEOFBEDWARS_PLAYER_HPP
