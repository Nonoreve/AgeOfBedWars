//
// Created by nonoreve on 1/5/21.
//

#include "Base.hpp"

Base::Base(int health, Position position) : _health(health), _position(position) {
}

const Position &Base::getPosition() const {
	return _position;
}

void Base::takeDamage(int damages) {
	_health -= damages;
}

bool Base::stillAlive() const {
	return _health > 0;
}

bool Base::operator==(const Base &rhs) const {
	return _position == rhs._position && _mark == rhs._mark;
}
