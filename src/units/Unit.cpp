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

Position Unit::nextWantedPotition() {
	int longest, shortest, dx1, dy1, dx2, dy2;
	std::tie(longest, shortest, dx1, dy1, dx2, dy2) = Position::prepareBresenhamValues(_position,
	                                                                                   _target.getPosition());
	int numerator = longest * 2;
	Position pos = _position;
	if (numerator + shortest >= longest) {
		pos.x += dx1;
		pos.y += dy1;
	} else {
		pos.x += dx2;
		pos.y += dy2;
	}
	return pos;
}

void Unit::move() {
	int longest, shortest, dx1, dy1, dx2, dy2;
	std::tie(longest, shortest, dx1, dy1, dx2, dy2) = Position::prepareBresenhamValues(_position,
	                                                                                   _target.getPosition());
	int numerator = longest * 2;
	if (numerator + shortest >= longest) {
		_position.x += dx1;
		_position.y += dy1;
	} else {
		_position.x += dx2;
		_position.y += dy2;
	}
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

bool Unit::sameOwner(Unit *other) const {
	return _owner == other->_owner;
}
