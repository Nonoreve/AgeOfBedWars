//
// Created by nonoreve on 1/24/21.
//

#ifndef AGEOFBEDWARS_POSITION_HPP
#define AGEOFBEDWARS_POSITION_HPP

#include <cmath>
#include <iostream>
#include <tuple>

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

	/**
	 * To factorize intial code of Bresenham's line algorithm
	 * @return longest, shortest, dx1, dy1, dx2, dy2
	 */
	static std::tuple<int, int, int, int, int, int> prepareBresenhamValues(const Position &p1, const Position &p2) {
		int x2 = p2.x;
		int y2 = p2.y;
		int w = x2 - p1.x;
		int h = y2 - p1.y;
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
		return std::make_tuple(longest, shortest, dx1, dy1, dx2, dy2);
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
