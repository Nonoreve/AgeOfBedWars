//
// Created by nonoreve on 1/5/21.
//

#include "units/Unit.hpp"

Unit::Unit(int health, int strikePower, Player &owner, Base &target, const char mark) : _health(health),
                                                                                        _strikePower(strikePower),
                                                                                        _position(
		                                                                                        owner.getBase().getPosition()),
                                                                                        _target(target), _owner(owner),
                                                                                        _mark(mark) {

}

Position &Unit::move() {
	int x2 = _target.getPosition().x;
	int y2 = _target.getPosition().y;
	int w = x2 - _position.x;
	int h = y2 - _position.y;
	int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
	if (w < 0) {
		dx1 = -1;
		dx2 = -1;
	} else if (w > 0) {
		dx1 = 1;
		dx2 = 1;
	}
	if (h < 0) dy1 = -1; else if (h > 0) dy1 = 1;
	int longest = std::abs(w);
	int shortest = std::abs(h);
	if (longest <= shortest) {
		std::swap(longest, shortest);
		if (h < 0) dy2 = -1; else if (h > 0) dy2 = 1;
		dx2 = 0;
	}
	int numerator = longest >> 1; // longest * 2
	// next is in loop for full line drawing but we only are interested in the next point
	numerator += shortest;
	if (numerator >= longest) {
		numerator -= longest;
		_position.x += dx1;
		_position.y += dy1;
	} else {
		_position.x += dx2;
		_position.y += dy2;
	}
	return _position;
}

void Unit::takeDamages(int damages) {
	_health -= damages;
}

bool Unit::stillAlive() const {
	return _health > 0;
}

bool Unit::targetReached() {
	return _position == _target.getPosition();
}

bool Unit::belongsTo(Player &player) const {
	return _owner == player;
}
