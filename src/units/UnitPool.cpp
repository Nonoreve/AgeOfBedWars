//
// Created by nonoreve on 1/28/21.
//

#include "units/UnitPool.hpp"
#include "units/Infantryman.hpp"

Unit *UnitPool::unitFactory(const std::string &unitType, Position position, Base target) {
	if(unitType == "infantry"){
		_pool.push_back(std::make_unique<Infantryman>(position, target));
		return _pool.back().get();
	} // TODO other cases
	return nullptr;
}

Unit *UnitPool::getUnit(int handle) {
	return _pool.at(handle).get();
}
