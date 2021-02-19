//
// Created by nonoreve on 1/5/21.
//

#include "units/Catapult.hpp"

#include <iostream>
#include <algorithm>

Catapult::Catapult(Player &owner, Base &target) : Unit(12, 6, owner, target, string("C") + owner.getMark()) {
}

ActionType Catapult::getAction(int actionPhase) {
	switch (actionPhase) {
		case 1:
			return ATTACK;
		case 2:
			return IDLE;
		case 3:
			return MOVE;
		default:
			std::cerr << "INVALID MOVE" << std::endl;
			return IDLE;
	}
}

std::pair<vector<Position>, int> Catapult::attack(vector<Position> ennemies) {
	Position first = ennemies.front();
	// we take the closest ennemy and strike it if in range TODO adapt for catapult
	if (first.distance(_position) < 3) {
		vector<Position> v({first});
		return std::make_pair(v, _strikePower);
	}
	return std::make_pair(vector<Position>{}, 0);
}
