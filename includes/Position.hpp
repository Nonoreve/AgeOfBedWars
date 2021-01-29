//
// Created by nonoreve on 1/24/21.
//

#ifndef AGEOFBEDWARS_POSITION_HPP
#define AGEOFBEDWARS_POSITION_HPP

#include <cmath>
#include <iostream>

/**
 * Hold a 2D point in cell coordinates
 */
struct Position {
	int x;
	int y;

	/**
	 *
	 * @param x
	 * @param y default to 0 for 1 dimensional games
	 */
	explicit Position(int x, int y = 0) : x(x), y(y) {
	}

	/**
	 *
	 * @param target
	 * @return distance between current and target positions.
	 * Result is negative when target is closer to the origin
	 */
	float distance(Position target) const {
		return std::hypot(target.x - x, target.y - y);
	}

	bool operator==(const Position &rhs) const {
		return x == rhs.x && y == rhs.y;
	}

	bool operator!=(const Position &rhs) const {
		return !(rhs == *this);
	}
};

//std::ostream &operator<<(std::ostream &out, const Position &pos) {
//	return out << "(" << pos.x << ", " << pos.y << ")";
//}

#endif //AGEOFBEDWARS_POSITION_HPP
