//
// Created by nonoreve on 1/28/21.
//

#ifndef AGEOFBEDWARS_UNITPOOL_HPP
#define AGEOFBEDWARS_UNITPOOL_HPP

#include "Unit.hpp"
#include "Player.hpp"

#include <memory>
#include <vector>
#include <unordered_map>

namespace std {
	template<>
	struct hash<Position> { // from https://en.cppreference.com/w/cpp/utility/hash
		std::size_t operator()(Position const &pos) const noexcept {
			std::size_t h1 = std::hash<int>{}(pos.x);
			std::size_t h2 = std::hash<int>{}(pos.y);
			// shift h2 the number of digits of h1
			int power = (int) std::ceil(std::log2(h1)) + 1;
			return h1 + (h2 << power);
		}
	};
}
class UnitPool {
private:
	std::unordered_map<Position, std::unique_ptr<Unit>> _pool;

public:
	UnitPool() = default;

	Unit *unitFactory(UnitType unitType, Player &player, Base &target);

	bool move(Position destination, Position origin);

	void remove(Position position);

	/**
	 * Check if the cell at the given position has no Unit on it.
	 * @param position
	 * @return
	 */
	bool isCellFree(Position position) const;

	/**
	 * Return a vector containing all the key positions
	 * @return
	 */
	std::vector<Position> getAllPositions() const;

	/**
	 * @param currentPlayer
	 * @return Only the positions were the unit belongs to the given player sorted by their distance to players's base
	 */
	vector<Position> getHisArmy(Player &currentPlayer);

	/**
	 * @param unit
	 * @return All the units that are on the same line as the given one and that have not the same owner sorted by their distance to target
	 */
	vector<Position> getHisEnnemies(Unit *unit);

	Unit *getUnit(Position position) const;
};

#endif //AGEOFBEDWARS_UNITPOOL_HPP
