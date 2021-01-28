//
// Created by nonoreve on 1/28/21.
//

#include "units/UnitPool.hpp"

#include "units/Infantryman.hpp"

#include <iostream>

Unit *UnitPool::unitFactory(const std::string &unitType, Position position, Base target) {
	if (isCellFree(position)) {
		if (unitType == "infantry") {
			_pool.insert(std::make_pair(position, std::make_unique<Infantryman>(position, target)));
			return _pool.at(position).get();
		} // TODO other cases
	}
	return nullptr;
}

void UnitPool::move(Unit *unit, Position destination) {
	if (isCellFree(destination)) {
		auto node = _pool.extract(unit->getPosition());
		if (node.empty()) {
			std::cerr << "Given unit comes from nowhere" << std::endl;
			std::exit(-1);
		} else { // https://en.cppreference.com/w/cpp/container/map/extract
			node.key() = destination;
			_pool.insert(std::move(node));
		}
	}
}

void UnitPool::remove(Position position) {
	if (isCellFree(position)) {
		std::cerr << "Given unit comes from nowhere" << std::endl;
		std::exit(-1);
	} else {
		_pool.erase(position);
	}
}

bool UnitPool::isCellFree(Position position) const {
	return _pool.count(position) == 0;
}

Unit *UnitPool::getUnit(Position position) const {
	return _pool.at(position).get();
}
