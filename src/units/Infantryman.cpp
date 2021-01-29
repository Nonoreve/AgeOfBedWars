//
// Created by nonoreve on 1/5/21.
//

#include "units/Infantryman.hpp"

Infantryman::Infantryman(Position position, Base target) : Unit(10, 4, position, target), _dalekMode(false) {}

void Infantryman::upgrade() {
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
			if(_dalekMode)
				return ATTACK;
			return IDLE; // TODO attack if phase 1 failed
		default:
			std::cerr << "There is only 3 action phases" << std::endl;
			return IDLE;
	}
}

std::pair<Position, int> Infantryman::attack() {
	int x1 = _position.x;
	int y1 = _position.y;
	int x2 = _target.getPosition().x;
	int y2 = _target.getPosition().y;
	int w = x2 - x1;
	int h = y2 - y1;
	int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
	if (w < 0) dx1 = -1; else if (w > 0) dx1 = 1;
	if (h < 0) dy1 = -1; else if (h > 0) dy1 = 1;
	if (w < 0) dx2 = -1; else if (w > 0) dx2 = 1;
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
		x1 += dx1;
		y1 += dy1;
	} else {
		x1 += dx2;
		y1 += dy2;
	}
	return std::make_pair(Position(x1, y1), _strikePower);
}
