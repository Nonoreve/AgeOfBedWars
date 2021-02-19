//
// Created by nonoreve on 1/5/21.
//

#include "units/Infantryman.hpp"

#include <algorithm>

Infantryman::Infantryman(Player &owner, Base &target) : Unit(10, 4, owner, target, string("I") + owner.getMark()),
                                                        _dalekMode(false) {
}

void Infantryman::upgrade() { // TODO use
	// we don't check for multiple upgrading, just overwrite it.
	_dalekMode = true;
}

ActionType Infantryman::getAction(int actionPhase) {
	switch (actionPhase) {
		case 1:
			return ATTACK;
		case 2:
			return MOVE;
		case 3:
			if (_dalekMode)
				return ATTACK;
			return IDLE; // TODO attack if phase 1 failed
		default:
			std::cerr << "There is only 3 action phases" << std::endl;
			return IDLE;
	}
}

/**
 * A blind strike on the cell in front of him
 * @param ennemies
 * @return
 */
std::pair<vector<Position>, int> Infantryman::attack(vector<Position> ennemies) {
	Position pos = _position;
	int longest, shortest, dx1, dy1, dx2, dy2;
	std::tie(longest, shortest, dx1, dy1, dx2, dy2) = Position::prepareBresenhamValues(_position,
	                                                                                   _target.getPosition());
	int numerator = longest * 2;
	// next is in loop for full line drawing but we only are interested in the next point
	numerator += shortest;
	if (numerator >= longest) {
		numerator -= longest;
		pos.x += dx1;
		pos.y += dy1;
	} else {
		pos.x += dx2;
		pos.y += dy2;
	}
	vector<Position> v;
	// check if ennemy or ally
	if(std::find(ennemies.begin(), ennemies.end(), pos) != ennemies.end())
		v.push_back(pos);
	return std::make_pair(v, _strikePower);
}
