//
// Created by nonoreve on 1/5/21.
//

#ifndef AGEOFBEDWARS_INFANTRYMAN_HPP
#define AGEOFBEDWARS_INFANTRYMAN_HPP

#include "Unit.hpp"

class Infantryman : public Unit {
private:
	bool _dalekMode;

public:
	Infantryman(Player &owner, Base &target);

	void upgrade();

	ActionType getAction(int actionPhase) override;

	std::pair<vector<Position>, int> attack(vector<Position> ennemies) override;

};


#endif //AGEOFBEDWARS_INFANTRYMAN_HPP
