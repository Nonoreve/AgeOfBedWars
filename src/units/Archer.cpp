//
// Created by nonoreve on 1/5/21.
//

#include "units/Archer.hpp"

#include <iostream>
#include <ffi.h>

Archer::Archer(Player &owner, Base &target) : Unit(8, 3, owner, target, string("A") + owner.getMark(),
                                                   UnitType::ARCHER) {
}

ActionType Archer::getAction(int actionPhase) {
	switch (actionPhase) {
		case 1:
			return ATTACK;
		case 2:
			return MOVE;
		case 3:
			return IDLE;
		default:
			std::cerr << "Invalid action" << std::endl;
			return IDLE;
	}
}

std::pair<vector<Position>, int> Archer::attack(vector<Position> ennemies) {
	if (!ennemies.empty()) {
		Position first = ennemies.front();
		// we take the closest ennemy and strike it if in range
		if (first.distance(_position) <= 3) {
			vector<Position> v({first});
			return std::make_pair(v, _strikePower);
		}
	}
	return std::make_pair(vector<Position>{}, _strikePower);
}
