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

Position next(Position origin, Position target) { // TODO move to Postition.h
	int longest, shortest, dx1, dy1, dx2, dy2;
	std::tie(longest, shortest, dx1, dy1, dx2, dy2) = Position::prepareBresenhamValues(origin, target);
	int numerator = longest * 2;
	Position result = origin;
	numerator += shortest;
	if (numerator >= longest) {
		result.x += dx1;
		result.y += dy1;
	} else {
		result.x += dx2;
		result.y += dy2;
	}
	return result;
}

std::pair<vector<Position>, int> Catapult::attack(vector<Position> ennemies) {
	vector<Position> v;
	auto it = ennemies.begin();
	// skip ennemies that are too close
	while (it != ennemies.end() && it->distance(_position) < 2) {
		++it;
	}
	if (it != ennemies.end()) {
		if (it->distance(_position) <= 3) {
			v.push_back(*it);
			v.push_back(next(*it, _target.getPosition()));
		} else if (it->distance(_position) <= 4) {
			v.push_back(*it);
			// we give it our own base position to get the previous case
			v.push_back(next(*it, _owner.getBase().getPosition()));
		}
	}
	return std::make_pair(v, _strikePower);
}
