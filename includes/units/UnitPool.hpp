//
// Created by nonoreve on 1/28/21.
//

#ifndef AGEOFBEDWARS_UNITPOOL_HPP
#define AGEOFBEDWARS_UNITPOOL_HPP

#include "Unit.hpp"

#include <memory>
#include <vector>
#include <unordered_map>

namespace std {
	template<>
	struct hash<Position> { // from https://en.cppreference.com/w/cpp/utility/hash
		std::size_t operator()(Position const &pos) const noexcept {
			std::size_t h1 = std::hash<int>{}(pos.x);
			std::size_t h2 = std::hash<int>{}(pos.y);
			return h1 ^ (h2 << 1);
		}
	};
}
class UnitPool {
private:
	std::unordered_map<Position, std::unique_ptr<Unit>> _pool;

public:
	UnitPool() {
	}

	Unit *unitFactory(UnitType unitType, Position position, Base target);

	void move(Unit *unit, Position destination);

	void remove(Position position);

	/**
	 * Check if the cell at the given position has no Unit on it.
	 * @param position
	 * @return
	 */
	bool isCellFree(Position position) const;

	std::vector<Position> getAllPositions() const;

	Unit *getUnit(Position position) const;
};

#endif //AGEOFBEDWARS_UNITPOOL_HPP
