//
// Created by nonoreve on 1/28/21.
//

#ifndef AGEOFBEDWARS_UNITPOOL_HPP
#define AGEOFBEDWARS_UNITPOOL_HPP

#include "Unit.hpp"

#include <map>
#include <memory>

class UnitPool {
private:
	std::map<Position, std::unique_ptr<Unit>> _pool;

public:
	UnitPool() {
	}

	Unit *unitFactory(const std::string &unitType, Position position, Base target);

	void move(Unit* unit, Position destination);

	void remove(Position position);

	/**
	 * Check if the cell at the given position has no Unit on it.
	 * @param position
	 * @return
	 */
	bool isCellFree(Position position) const;

	Unit *getUnit(Position position) const;
};

#endif //AGEOFBEDWARS_UNITPOOL_HPP
