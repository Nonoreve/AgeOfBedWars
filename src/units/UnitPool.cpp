//
// Created by nonoreve on 1/28/21.
//

#include "units/UnitPool.hpp"

#include "units/Infantryman.hpp"
#include "units/Archer.hpp"
#include "units/Catapult.hpp"

#include <iostream>
#include <algorithm>

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
			case UNKNOWN:
				std::cerr << "Invalid UnitType" << std::endl;
				std::exit(-1);
		}
	}
	return nullptr;
}

bool UnitPool::move(Position destination, Position origin) {
	if (isCellFree(destination)) {
		auto node = _pool.extract(origin);
		if (node.empty()) {
			std::cerr << "No unit at position (" << origin.x << ", " << origin.y << ")" << std::endl;
			std::exit(-1);
		} else { // https://en.cppreference.com/w/cpp/container/map/extract
			node.key() = destination;
			_pool.insert(std::move(node));
			return true;
		}
	}
	return false;
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

vector<Position> UnitPool::getHisArmy(Player &currentPlayer) {
	vector<Position> playerPos;
	for (const auto &current : _pool) {
		if (current.second->belongsTo(currentPlayer))
			playerPos.push_back(current.first);
	}
	std::sort(playerPos.begin(), playerPos.end(), [&](auto i1, auto i2) {
		return currentPlayer.getBase().getPosition().distance(i1) < currentPlayer.getBase().getPosition().distance(i2);
	});
	return playerPos;
}

bool isOnLine(Unit *unit, Position p1, Position p2) {
	int longest, shortest, dx1, dy1, dx2, dy2;
	std::tie(longest, shortest, dx1, dy1, dx2, dy2) = Position::prepareBresenhamValues(p1, p2);
	int numerator = longest * 2;
	int i = 0;
	bool found = false;
	while (!found && i <= longest) {
		found = unit->getPosition().distance(p1) < 1;
		if (!found) {
			numerator += shortest;
			if (numerator >= longest) {
				p1.x += dx1;
				p1.y += dy1;
			} else {
				p1.x += dx2;
				p1.y += dy2;
			}
			++i;
		}
	}
	return found;
}

vector<Position> UnitPool::getHisEnnemies(Unit *unit) {
	vector<Position> result;
	for (const auto &current : _pool) {
		if (!current.second->sameOwner(unit) &&
		    isOnLine(current.second.get(), unit->getPosition(), unit->getTargetPostion()))
			result.push_back(current.first);
	}
	std::sort(result.begin(), result.end(), [unit](auto i1, auto i2) {
		return unit->getPosition().distance(i1) < unit->getPosition().distance(i2);
	});
	return std::move(result);
}

Unit *UnitPool::getUnit(Position position) const {
	if (isCellFree(position)) {
		std::cerr << "No unit at position (" << position.x << ", " << position.y << ")" << std::endl;
		std::exit(-1);
	} else {
		return _pool.at(position).get();
	}
}
