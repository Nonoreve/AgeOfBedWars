//
// Created by nonoreve on 1/5/21.
//

#ifndef AGEOFBEDWARS_ARCHER_HPP
#define AGEOFBEDWARS_ARCHER_HPP

#include "Unit.hpp"

class Archer : public Unit {
public:
	Archer(Player& owner, Base &target);

	ActionType getAction(int actionPhase) override;

	std::pair<Position, int> attack() override;
};


#endif //AGEOFBEDWARS_ARCHER_HPP
