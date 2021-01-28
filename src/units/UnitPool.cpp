//
// Created by nonoreve on 1/28/21.
//

#include "units/UnitPool.hpp"

#include "units/Infantryman.hpp"
#include "units/Archer.hpp"
#include "units/Catapult.hpp"

#include <iostream>

Unit *UnitPool::unitFactory(UnitType unitType, Position position, Base target) {
	if (isCellFree(position)) {
		switch (unitType) {
			case INFANTRYMAN:
				_pool.insert(std::make_pair(position, std::make_unique<Infantryman>(position, target)));
				return _pool.at(position).get();
			case ARCHER:
				_pool.insert(std::make_pair(position, std::make_unique<Archer>(position, target)));
				return _pool.at(position).get();
			case CATAPULT:
				_pool.insert(std::make_pair(position, std::make_unique<Catapult>(position, target)));
				return _pool.at(position).get();
		}
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

std::vector<Position> UnitPool::getAllPositions() const {
	std::vector<Position> positions;
	for(const auto & current : _pool)
		positions.push_back(current.first);
	return positions;
}

Unit *UnitPool::getUnit(Position position) const {
	return _pool.at(position).get();
}
