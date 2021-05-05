//
// Created by nonoreve on 1/5/21.
//

#include "units/Catapult.hpp"

#include <iostream>
#include <algorithm>

Catapult::Catapult(Player &owner, Base &target) : Unit(12, 6, owner, target, string("C") + owner.getMark(),
                                                       UnitType::CATAPULT) {
}

ActionType Catapult::getAction(int actionPhase) {
	switch (actionPhase) {
		case 1:
			return ATTACK;
		case 2:
			return IDLE;
		case 3:
			if (!_sucessfullPhases[0])
				return MOVE;
			return IDLE;
		default:
			std::cerr << "Invalid action" << std::endl;
			return IDLE;
	}
}

std::pair<vector<Position>, int> Catapult::attack(vector<Position> ennemies) {
	vector<Position> v;
	auto it = ennemies.begin();
	// skip ennemies that are too close
	while (it != ennemies.end() && it->distance(_position) < 2) {
		++it;
	}
	_sucessfullPhases[0] = false;
	if (it != ennemies.end()) {
		if (it->distance(_position) <= 4) {
			v.push_back(*it);
			_sucessfullPhases[0] = true;
			if (it->distance(_position) <= 3) {
				Position collateral = Position::next(*it, _target.getPosition());
				if (collateral != *it)
					v.push_back(collateral);
			} else {
				// we give it our own base position to get the cell before
				Position collateral = Position::next(*it, _owner.getBase().getPosition());
				if (collateral != *it)
					v.push_back(collateral);
			}
		}
	}
	return std::make_pair(v, _strikePower);
}
