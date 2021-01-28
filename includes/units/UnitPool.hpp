//
// Created by nonoreve on 1/28/21.
//

#ifndef AGEOFBEDWARS_UNITPOOL_HPP
#define AGEOFBEDWARS_UNITPOOL_HPP

#include "Unit.hpp"

#include <vector>
#include <memory>

class UnitPool {
private:
	std::vector<std::unique_ptr<Unit>> _pool;

public:
	UnitPool() {
	}

	Unit *unitFactory(const std::string &unitType, Position position, Base target);

	Unit *getUnit(int handle);
};

#endif //AGEOFBEDWARS_UNITPOOL_HPP
