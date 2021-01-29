//
// Created by nonoreve on 1/5/21.
//

#ifndef AGEOFBEDWARS_CATAPULT_HPP
#define AGEOFBEDWARS_CATAPULT_HPP

#include "Unit.hpp"

class Catapult : public Unit {
public:
	Catapult(Position position, Base target);

	ActionType getAction(int actionPhase) override;

	std::pair<Position, int> attack() override;

};


#endif //AGEOFBEDWARS_CATAPULT_HPP
