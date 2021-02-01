//
// Created by nonoreve on 1/28/21.
//

#include "units/UnitPool.hpp"

#include "units/Infantryman.hpp"
#include "units/Archer.hpp"
#include "units/Catapult.hpp"

#include <iostream>

Unit *UnitPool::unitFactory(UnitType unitType, Player &player, Base &target) {
	Position position = player.getBase().getPosition();
	if (isCellFree(position)) {
		switch (unitType) {
			case INFANTRYMAN:
				_pool.insert(std::make_pair(position, std::make_unique<Infantryman>(player, target)));
				return _pool.at(position).get();
			case ARCHER:
				_pool.insert(std::make_pair(position, std::make_unique<Archer>(player, target)));
				return _pool.at(position).get();
			case CATAPULT:
				_pool.insert(std::make_pair(position, std::make_unique<Catapult>(player, target)));
				return _pool.at(position).get();
		}
	}
	return nullptr;
}

void UnitPool::move(Position destination, Position origin) {
	if (isCellFree(destination)) {
		auto node = _pool.extract(origin);
		if (node.empty()) {
			std::cerr << "No unit at position (" << origin.x << ", " << origin.y << ")" << std::endl;
			std::exit(-1);
		} else { // https://en.cppreference.com/w/cpp/container/map/extract
			node.key() = destination;
			_pool.insert(std::move(node));
		}
	}
}

void UnitPool::remove(Position position) {
	if (isCellFree(position)) {
		std::cerr << "No unit at position (" << position.x << ", " << position.y << ")" << std::endl;
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
	for (const auto &current : _pool)
		positions.push_back(current.first);
	return positions;
}

vector<Position> UnitPool::filter(Player &currentPlayer) {
	vector<Position> playerPos;
	for (const auto &current : _pool) {
		if (current.second->belongsTo(currentPlayer))
			playerPos.push_back(current.first);
	}
	return playerPos;
}

Unit *UnitPool::getUnit(Position position) const {
	if (isCellFree(position)) {
		std::cerr << "No unit at position (" << position.x << ", " << position.y << ")" << std::endl;
		std::exit(-1);
	} else {
		return _pool.at(position).get();
	}
}
